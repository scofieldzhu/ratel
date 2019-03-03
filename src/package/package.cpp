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
#include "rowdatadict.h"
#include "datablockfile.h"
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
	dirtab_.connectDB(*db_);    
	filetab_.connectDB(*db_);	
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
		parentid = dirtab_.queryDirId(location.cstr());
		if(parentid == -1){
			slog_err(pkglogger) << "dir(" << location.cstr() << ") not exists!" << endl;
			return false;
		}
		//check if exists redundant dir
		if(dirtab_.queryDirId(name, parentid) != -1){
			slog_err(pkglogger) << "sub dir(" << name.cstr() << ") already exists!" << endl;
			return false;
		}
	}
	Path newdirpath = location.join(name);
	if(!dirtab_.insertRow(RowDataDict({
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
	if(!sourcefile.exists() || !sourcefile.isRegularFile()){
		slog_err(pkglogger) << "source file[" << sourcefile.cstr() << "] is invalid file!" << endl;
		return false;
	}
	const RString srcfn = sourcefile.filename().rstring();
    if(!opened()){
        slog_err(pkglogger) << "package not opened yet!" << endl;
        return false;
    }  
	int32_t dirid = dirtab_.queryDirId(dirlocation.rstring());
	if(dirid == -1){
		slog_err(pkglogger) << "dirlocation(" << dirlocation.cstr() << ") not exists!" << endl;
		return false;
	}
	if(filetab_.existsFile(srcfn, dirid)){
		slog_err(pkglogger) << "same file(" << srcfn.cstr() << ") already exists in location directory(" << dirlocation.cstr() << ")!" << endl;
		return false;
	}
	AgileFileOperator filereader(sourcefile.toWString());
	uint32_t wholedatasize = 0;
	char* wholedata = filereader.readWholeData(wholedatasize);
	if(wholedata == nullptr){
		slog_err(pkglogger) << "read whole data from source file failed!" << endl;
		return false;
	}
	DataBlockFile::UID newfileuid = DataBlockFile::NewUID();	
	filedatastream_->appendDataBlock(newfileuid, wholedata, wholedatasize);
	RowDataDict newrow({
		{FileTable::kNameKey, srcfn.cstr()},
		{FileTable::kDirIdKey, dirid},
		{FileTable::kFileUIDKey, newfileuid.c_str()},
		{FileTable::kStatusKey, FileTable::NORMAL}
		});
	if(!filetab_.insertRow(newrow)){
		filedatastream_->removeDataBlock(newfileuid); //rollback data
		slog_err(pkglogger) << "insert new row data to FileTable failed!" << endl;
		return false;
	}
	return true;
}

bool Package::removeFile(const Path& filepath)
{
	if(!filepath.isRegularFile()){
		slog_err(pkglogger) << "filepath[" << filepath.cstr() <<"] is not regular file!" << endl;
		return false;
	}

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
	tmpdatafile_ = generateTmpDataFilePath();
	filedatastream_ = new DataBlockFile(tmpdatafile_.toWString());
	filedatastream_->initEmpty();
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
