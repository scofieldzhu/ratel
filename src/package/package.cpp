/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: package.cpp 
CreateTime: 2018-9-16 21:54
=========================================================================*/
#include "package.h"
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

RATEL_NAMESPACE_BEGIN

#define RESET_LASTERR() impl_->lasterr = "noerr";

const Path kTempDir = "./temp";

namespace {
    const char* kIdField = "id";
    const char* kNameField = "name";
    const char* kParentField = "parent";
    const char* kChildFiled = "child";
    const char* kDirField = "dir";
    const char* kGuidField = "guid";
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
    Impl()
        :dirtable("Directory"),
        filetable("File") 
    {
        dirtable.addColumn(IntCol(kIdField).primaryKeyOn().uniqueOn().notNullOn());
        dirtable.addColumn(StrCol(kNameField, 50).notNullOn());
        dirtable.addColumn(IntCol(kParentField).setDefaultValue(-1).defaultOn());

        filetable.addColumn(IntCol(kIdField).primaryKeyOn().uniqueOn().notNullOn());
        filetable.addColumn(StrCol(kNameField, 100).notNullOn());
        filetable.addColumn(IntCol(kDirField).setDefaultValue(-1).defaultOn());
        filetable.addColumn(StrCol(kGuidField, 36).notNullOn());
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
    RString sql = impl_->dirtable.makeInsertSql({kNameField, kParentField}, "'%s', %d", name.cstr(), newnode->parent->dbid);
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
    sql = impl_->dirtable.makeQueryRowWhenSql("%s=%d and %s='%s'", kParentField, newnode->parent->dbid, kNameField, name.cstr());    
    newstmt = impl_->db->createStatement(sql);
    if(newstmt->stepExec() != RESCODE_DONE){
        delete newstmt;
        impl_->lasterr = RString::FormatString("stepExec(sql:%s) failed! ", sql.cstr());
        return false;                
    }
    newnode->dbid = newstmt->fetchIntColumn(0);
    delete newstmt;
    return true;
}

bool Package::importDir(const Path& parentdir, const Path& localdir)
{
    return false;
}

bool Package::removeDir(const Path& dir)
{
    return false;
}

bool Package::exportDirTo(const Path& sourcedir, const Path& local_targetdir)
{
    return false;
}

bool Package::addFile(const Path& parentdir, const Path& local_sourcefilepath)
{
    return false;
}

bool Package::removeFile(const Path& filepath)
{
    return false;
}

bool Package::exportFileTo(const Path& sourcefilepath, const Path& local_targetfilepath)
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
    slog_debug(pkglogger) << createsql.cstr() << endl;
    Statement* stat = impl_->db->createStatement(createsql);
    int32 result = stat->stepExec();
    if(result != RESCODE_DONE){
        impl_->lasterr = RString::FormatString("create dirtable failed!(details:%s)", stat->errMsg().cstr());
        delete impl_->db;
        impl_->db = nullptr;
        return false;
    }
    delete stat;
    createsql = impl_->filetable.makeCreateSql();
    slog_debug(pkglogger) << createsql.cstr() << endl;
    stat = impl_->db->createStatement(createsql);
    result = stat->stepExec();
    if(result != RESCODE_DONE){
        impl_->lasterr = RString::FormatString("create filetable failed!(details:%s)", stat->errMsg().cstr());
        delete impl_->db;
        impl_->db = nullptr;
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
