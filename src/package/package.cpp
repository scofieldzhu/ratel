/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: package.cpp 
CreateTime: 2018-9-16 21:54
=========================================================================*/
#include "package.h"
#include <fstream>
#include "path.h"
#include "pkgfilereader.h"
#include "db.h"
#include "openflag.h"
#include "statement.h"
#include "rstrutil.h"
#include "table.h"
#include "tablecol.h"
#include "pkglogger.h"
#include "dirtree.h"
#include "dirnode.h"
#include "pkglogger.h"
#include "filescanner.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

#define RESET_LASTERR() impl_->lasterr = "noerr";

const Path kTempDir = "./temp";

const int32 kReadBufferSize = 1024 * 10; //10K

int32 g_DataFileIdSeed = 0x0000000010000000;

namespace {
    #define ID_FIELD "id"
    #define NAME_FIELD "name"
    #define PARENT_FIELD "parent"
    #define STATUS_FIELD "status"
    #define CHILD_FIELD "child"
    #define DIR_FIELD "dir"
    #define GUID_FIELD "guid"
}

struct Package::Impl 
{
    RString lasterr;
    Path* curdbpath = nullptr;
    Path* curpkgpath = nullptr;
    DB* db = nullptr;
    DirTree* dirtree = nullptr;
    Table dirtable;
    Table filetable;
    ofstream* datafilewriter = nullptr;
    
    Impl()
        :dirtable("Directory"),
        filetable("File") 
    {
        dirtable.addColumn(IntCol(ID_FIELD).primaryKeyOn().uniqueOn().notNullOn());
        dirtable.addColumn(StrCol(NAME_FIELD, 50).notNullOn());
        dirtable.addColumn(IntCol(PARENT_FIELD).setDefaultValue(-1).defaultOn());
        dirtable.addColumn(IntCol(STATUS_FIELD).setDefaultValue(1).defaultOn());

        filetable.addColumn(IntCol(ID_FIELD).primaryKeyOn().uniqueOn().notNullOn());
        filetable.addColumn(StrCol(NAME_FIELD, 100).notNullOn());
        filetable.addColumn(IntCol(DIR_FIELD).setDefaultValue(-1).defaultOn());
        filetable.addColumn(IntCol(STATUS_FIELD).setDefaultValue(1).defaultOn());

        datafilewriter = new ofstream("tmp.dat", ios::out | ios::binary);
        bool ok = datafilewriter->is_open();
        logverify(pkglogger, ok);
    }
    
    void releaseDB()
    {
        if(db)
            delete db;
        db = nullptr;
    }

    int32 addFileToDataFileSet(const Path& sourcefile)
    {
        datafilewriter->seekp(0, ios_base::end);
        int32 newfileid = g_DataFileIdSeed++;
        (*datafilewriter) << newfileid; //write 'file_id' value
        ifstream ifs(sourcefile.cstr(), ios::in | ios::binary);
        ifs.seekg(0, ios_base::beg);
        (*datafilewriter) << ifs.tellg(); //write 'file_data_size' value
        while(ifs){
            char databuffer[kReadBufferSize + 1] = { '\0' };
            ifs.read(databuffer, kReadBufferSize);
            datafilewriter->write(databuffer, ifs.gcount());
            if(ifs.gcount() < kReadBufferSize)
                break; //it's eof
        }
        ifs.close();
        return newfileid;
    }

    int32 addFileRecordToDB(const RString& filename, int32 dirid)
    {
        RString sql = filetable.makeInsertSql({NAME_FIELD, DIR_FIELD}, "'%s',%d", filename.cstr(), dirid);
        Statement* stat = db->createStatement(sql);
        logverify(pkglogger, stat);
        ResultCode rc = stat->stepExec();
        logverify(pkglogger, rc == RESCODE_DONE);
        delete stat;
        sql = filetable.makeQueryRowWhenSql("%s='%s' and %s=%d", NAME_FIELD, filename.cstr(), DIR_FIELD, dirid);
        stat = db->createStatement(sql);
        logverify(pkglogger, stat);
        rc = stat->stepExec();
        logverify(pkglogger, rc == RESCODE_ROW);
        int32 newrecordid = stat->fetchIntColumn(0);
        delete stat;
        return newrecordid;
    }
};

Package::Package()
    :impl_(new Impl)
{}

Package::~Package()
{
    delete impl_;
}

const RString& Package::lastError() const
{
    return impl_->lasterr;
}

bool Package::createDir(const RString& name, const Path& location)
{
    RESET_LASTERR();
    if(!opened()){
        impl_->lasterr = "package is not opened!";
        return false;
    }
    DirNode* newnode = impl_->dirtree->createDir(name, location);
    if(!newnode){
        impl_->lasterr = RString::FormatString("dirtree->createDir(%s, %s) failed! ", name.cstr(), location.cstr());
        return false;
    }
    RString sql = impl_->dirtable.makeInsertSql({NAME_FIELD, PARENT_FIELD}, "'%s', %d", name.cstr(), newnode->parent->dbid);
    Statement* newstmt = impl_->db->createStatement(sql);
    if(newstmt == nullptr){
        impl_->lasterr = RString::FormatString("createStatement(sql:%s) failed! ", sql.cstr());
        return false;
    }
    if(newstmt->stepExec() != RESCODE_DONE){
        delete newstmt;
        impl_->lasterr = RString::FormatString("stepExec(sql:%s) failed! ", sql.cstr());
        return false;
    }
    delete newstmt;
    //query new id
    sql = impl_->dirtable.makeQueryRowWhenSql("%s=%d and %s='%s'", PARENT_FIELD, newnode->parent->dbid, NAME_FIELD, name.cstr());    
    newstmt = impl_->db->createStatement(sql);
    ResultCode rc = newstmt->stepExec();
    if(rc != RESCODE_ROW){
        delete newstmt;
        impl_->lasterr = RString::FormatString("stepExec(sql:%s) failed! ", sql.cstr());
        return false;                
    }
    newnode->dbid = newstmt->fetchIntColumn(0);
    delete newstmt;
    return true;
}

bool Package::importDir(const Path& location, const Path& localdir)
{
    if(!opened()){
        slog_err(pkglogger) << "package not opened yet!" << endl;
        return false;
    }
    if(!localdir.exists() || !localdir.isDirectory()){
        slog_err(pkglogger) << "local directory[" << localdir.cstr() << "] is invalid!" << endl;
        return false;
    }
    //FileScanner::PathSet files

    return false;
}

bool Package::removeDir(const Path& dir)
{
    return false;
}

bool Package::exportDir(const Path& sourcedir, const Path& local_targetdir)
{
    return false;
}

bool Package::importFile(const Path& dirlocation, const Path& sourcefile)
{
    if(!opened()){
        slog_err(pkglogger) << "package not opened yet!" << endl;
        return false;
    }
    if(!sourcefile.exists() || !sourcefile.isRegularFile()){
        slog_err(pkglogger) << "source file[" << sourcefile.cstr() << "] is invalid file!" << endl;
        return false;
    }
    const RString sourcefilename = sourcefile.filename().rstring();
    FileNode* filenode = impl_->dirtree->createFile(dirlocation, sourcefilename);
    if(filenode == nullptr) {
        slog_err(pkglogger) << "create file:" << sourcefilename.cstr() << " at dir:" << dirlocation.cstr() << " failed!" << endl;
        return false;
    }
    int32 newfileid = impl_->addFileToDataFileSet(sourcefile);
    filenode->datafileid = newfileid;
    filenode->init = 1;    
    filenode->dbid = impl_->addFileRecordToDB(sourcefilename, impl_->dirtree->locateDir(dirlocation)->dbid);
    return true;
}

bool Package::removeFile(const Path& filepath)
{
    return false;
}

bool Package::exportFile(const Path& sourcefilepath, const Path& local_targetfilepath)
{
    return false;
}

bool Package::load(const Path& pkgpath)
{
    return false;
}

bool Package::createNew(const Path& newpkgpath)
{
    RESET_LASTERR();
    impl_->curpkgpath = new Path(newpkgpath);
    Path pkgdir = newpkgpath.parentPath();
    if(!pkgdir.exists()){
        impl_->lasterr = RString::FormatString("new package filepath(%s) is invalid!", newpkgpath.rstring().cstr());
        return false;
    }
    Path newdbpath = pkgdir.join(rstrutil::NewGuid() + ".db");
    impl_->db = DB::OpenDB(newdbpath, RATEL_DB_OPEN_READWRITE | RATEL_DB_OPEN_CREATE);
    if(impl_->db == nullptr){
        impl_->lasterr = RString::FormatString("create db file(%s) failed!", newdbpath.rstring().cstr());
        return false;
    }
    RString createsql = impl_->dirtable.makeCreateSql();
    Statement* stat = impl_->db->createStatement(createsql);
    if(stat == nullptr){
        impl_->lasterr = RString::FormatString("create statement failed! sql:%s err:%s", createsql.cstr(), impl_->db->errMsg().cstr());
        impl_->releaseDB();
        return false;
    }
    int32 result = stat->stepExec();
    if(result != RESCODE_DONE){
        impl_->lasterr = RString::FormatString("create dirtable stepExec failed!(sql:%s details:%s)", createsql.cstr(), stat->errMsg().cstr());
        impl_->releaseDB();
        return false;
    }
    delete stat;
    createsql = impl_->filetable.makeCreateSql();
    stat = impl_->db->createStatement(createsql);
    if(stat == nullptr){
        impl_->lasterr = RString::FormatString("create statement failed! sql:%s err:%s", createsql.cstr(), impl_->db->errMsg().cstr());
        impl_->releaseDB();
        return false;
    }
    result = stat->stepExec();
    if(result != RESCODE_DONE){
        impl_->lasterr = RString::FormatString("create filetable stepExec failed!(sql:%s details:%s)", createsql.cstr(), stat->errMsg().cstr());
        impl_->releaseDB();
        return false;
    }
    delete stat;
    impl_->dirtree = new DirTree();
    return true;
}

bool Package::opened() const
{
    return impl_->db != nullptr;
}

RATEL_NAMESPACE_END
