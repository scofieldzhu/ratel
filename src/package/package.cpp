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
#include "db.h"
#include "dirtree.h"
#include "dirnode.h"
#include "dirwalker.h"
#include "path.h"
#include "pathremover.h"
#include "pkglogger.h"
#include "pkgfilereader.h"
#include "pkgfilewriter.h"
#include "rstrutil.h"
#include "sqlite3.h"
#include "statement.h"
#include "dbtablecol.h"
#include "dbtablerecord.h"
using namespace std;

RATEL_NAMESPACE_BEGIN

#define RESET_LASTERR() lasterr_ = "noerr";

const int32_t kReadBufferSize = 1024 * 10; //10K

int32_t g_DataFileIdSeed = 0x0000000010000000;

Package::Package(const Path& workdir)    
{
    if(workdir.isDirectory())
        workdir_ = workdir;



}

Package::~Package()
{
   // releaseResources();
}

void Package::releaseResources()
{
    releaseDB();
    if(tmpdatafilewriter_){
        tmpdatafilewriter_->close();
        rtdelete(tmpdatafilewriter_);
    }
    tmpdatafilewriter_ = nullptr;    
    if(dbfile_.exists())
        PathRemover().perform(dbfile_);
    if(tmpdatafile_.exists())
        PathRemover().perform(tmpdatafile_);
}

void Package::releaseDB()
{
    if(db_)
        rtdelete(db_);
    db_ = nullptr;
}

int32_t Package::writeNewFileData(const Path& sourcefile)
{
    if(tmpdatafilewriter_ == nullptr){
        tmpdatafile_ = generateTmpDataFilePath();
        std::string localefn = tmpdatafile_.rstring().decodeToLocale();
        tmpdatafilewriter_ = new ofstream(localefn.c_str(), ios::out | ios::binary);
        bool ok = tmpdatafilewriter_->is_open();
        logverify(pkglogger, ok);
    }
    tmpdatafilewriter_->seekp(0, ios_base::end);
    int32_t newfileid = g_DataFileIdSeed++;
    (*tmpdatafilewriter_) << newfileid; //write 'file_id' value
    ifstream ifs(sourcefile.cstr(), ios::in | ios::binary);
    ifs.seekg(0, ios_base::beg);
    (*tmpdatafilewriter_) << ifs.tellg(); //write 'file_data_size' value
    while(ifs){
        char databuffer[kReadBufferSize + 1] = {'\0'};
        ifs.read(databuffer, kReadBufferSize);
        tmpdatafilewriter_->write(databuffer, ifs.gcount());
        if(ifs.gcount() < kReadBufferSize)
            break; //it's eof
    }
    ifs.close();
    return newfileid;
}

// int32_t Package::addFileRecordToDB(const RString& filename, int32_t dirid, int32_t fuid)
// {
// 	DbTableRecord record;
// 	record.addColumn(FileTable::kNameKey, filename.cstr()).addColumn(FileTable::kDirIdKey, dirid);
// 	bool isok = filetab_.insertRow(*db_, record);
// 	logverify(pkglogger, isok);
//     RString sql = filetab_.makeQueryRowWhenSql("%s='%s' and %s=%d", FileTable::kNameKey, filename.cstr(), FileTable::kDirIdKey, dirid);
//     Statement* stat = db_->createStatement(sql);
//     logverify(pkglogger, stat);
//     int32_t rc = stat->stepExec();
//     logverify(pkglogger, rc == SQLITE_ROW);
//     int32_t newrecordid = stat->fetchIntColumn(0);
//     delete stat;
//     return newrecordid;
// }

Path Package::generateDBFilePath() const
{
    return workdir_.join(rstrutil::NewGuid()+".db");
}

Path Package::generateTmpDataFilePath() const
{
    return workdir_.join(rstrutil::NewGuid()+".DAT");
}

bool Package::initDB()
{
    logverify(pkglogger, db_);
	dirtab_.setDB(db_);
    if(!dirtab_.create()){
		slog_err(pkglogger) << "create dirtable failed!" << endl;
		return false;
	}
	filetab_.setDB(db_);
	if(!filetab_.create()){
		slog_err(pkglogger) << "create filetable failed!" << endl;
		return false;
	}    
    return true;
}

bool Package::createDir(const RString& name, const Path& location)
{
	logverify(pkglogger, !name.null());
    if(!opened()){
		slog_err(pkglogger) << "not opened!" << endl;
        return false;
    }
	int32_t parentid = 0;
	if(location.rstring() == "./")
		parentid = -1; //root dir
	else{
		parentid = dirtab_.queryId(location.cstr());
		if(parentid == -1){
			slog_err(pkglogger) << "dir(" << location.cstr() << ") not exists!" << endl;
			return false;
		}
		//check if exists redundant dir
	}
	Path newdirpath = location.join(name);
	if(!dirtab_.insertRow(DbTableRecord({
							{DirTable::kPathKey, newdirpath.cstr()},
							{DirTable::kParentKey, parentid},
							{DirTable::kStatusKey, DirTable::NORMAL}
						  })
						 )){
		slog_err(pkglogger) << "insertRow(newdir:" << newdirpath.cstr() << " parentid:" << parentid << ") failed!" << endl;
		return false;
	}    
    return true;
}

bool Package::importDir(const Path& location, const Path& localdir)
{
    if(!opened()){
        slog_err(pkglogger) << "package not opened yet!" << endl;
        return false;
    }
    if(!localdir.exists() || !localdir.isDirectory()){
        slog_err(pkglogger) << "invalid local directory[" << localdir.cstr() << "]!" << endl;
        return false;
    }
    const RString dirname = localdir.filename().rstring();
    if(!createDir(dirname, location)){
        slog_err(pkglogger) << "create directory[" << dirname.cstr() << "] at location[" <<location.cstr() <<"] failed!" << endl;
        return false;
    }
    Path rootdir = location.join(dirname);
    std::vector<Path> subfiles, subdirs;
    auto pathmeetfunc = [&subfiles, &subdirs](const Path& p) { p.isRegularFile() ? subfiles.push_back(p) : subdirs.push_back(p); };
    DirWalker walker(localdir);
    walker.walk(pathmeetfunc);
    for(auto f : subfiles){
        if(!importFile(rootdir, f)){
            slog_err(pkglogger) << "import file[" << f.cstr() << "] to location[" << rootdir.cstr() << "] failed!" << endl;
            return false;
        }
    }
    for(auto dir : subdirs){              
        if(!importDir(rootdir, dir)) {
            slog_err(pkglogger) << "import subdir[" << dir.cstr() << "] to location[" << rootdir.cstr() << "] failed!" << endl;
            continue;
        }
    }
    return true;
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
//     FileNode* filenode = dirtree_->createFile(dirlocation, sourcefilename);
//     if(filenode == nullptr) {
//         slog_err(pkglogger) << "create file:" << sourcefilename.cstr() << " at dir:" << dirlocation.cstr() << " failed!" << endl;
//         return false;
//     }
//     int32_t newfileid = writeNewFileData(sourcefile);
//     filenode->datafileid = newfileid;
//     filenode->init = 1;    
//     filenode->dbid = addFileRecordToDB(sourcefilename, dirtree_->locateDir(dirlocation)->dbid);
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
    if(!pkgpath.exists()){
		log_err(pkglogger, "pkg file(%s) not exists!", pkgpath.cstr());
        return false;
    }
    PkgFileReader pkgreader(pkgpath);
    if(!pkgreader.open()){
		log_err(pkglogger, "pkg file(%s) isn't well-formed!", pkgpath.cstr());
        return false;
    }
    dbfile_ = generateDBFilePath();
    ofstream ofs(dbfile_.toLocale().c_str(), ios::out | ios::binary);
    if(pkgreader.readDBDataToFile(ofs)){
        ofs.close();
        return true;
    }    
    return false;
}

bool Package::createNew(const Path& newpkgpath)
{
    if(!workdir_.isDirectory()){
		log_err(pkglogger, "workdir(%s) is not valid directory!", workdir_.cstr());
        return false;
    }
    pkgfile_ = newpkgpath;
    if(!pkgfile_.parentPath().exists()){
		log_err(pkglogger, "new package filepath(%s) is invalid!", pkgfile_.cstr());
        return false;
    }
    dbfile_ = generateDBFilePath();
    db_ = DB::OpenDB(dbfile_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    if(db_ == nullptr){
		log_err(pkglogger, "create db file(%s) failed!", dbfile_.cstr());
        return false;
    }
    if(!initDB()){
		log_err(pkglogger, "init created db(%s) failed!", dbfile_.cstr());
        releaseDB();
        return false;
    }    
    return true;
}

void Package::commit()
{
    if(!opened()){
        slog_err(pkglogger) << "not opened yet!" << endl;
        return;
    }    
    tmpdatafilewriter_->flush();
    PkgFileWriter pkgwriter(pkgfile_);
    if(!pkgwriter.beginWrite()){
        slog_err(pkglogger) << "PkgFileWriter(" << pkgfile_.cstr() << ") beginWrite failed!" << endl;
        return;
    }
    logverify(pkglogger, dbfile_.exists());
    if(!pkgwriter.writeFileData(dbfile_)){
        slog_err(pkglogger) << "PkgFileWriter writeFileData " << pkgfile_.cstr() << " failed!" << endl;
        //do cleanup
        return;
    }
    if(tmpdatafile_.exists()){
        if(!pkgwriter.writeFileData(tmpdatafile_)){
            slog_err(pkglogger) << "PkgFileWriter writeFileData " << tmpdatafile_.cstr() << " failed!" << endl;
            //do cleanup
            return;
        }        
    }
}

bool Package::opened() const
{
    return db_ != nullptr;
}

void Package::close()
{
    if(opened()){
        rtdelete(db_);
        db_ = nullptr;
    }
}

void Package::setWorkDir(const Path & dir)
{
    if(dir.isDirectory())
        workdir_ = dir;
}

RATEL_NAMESPACE_END
