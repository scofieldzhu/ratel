/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: package.cpp 
CreateTime: 2018-9-16 21:54
=========================================================================*/
#include "package.h"
#include "dirwalker.h"
#include "datablockstorage.h"
#include "pkglogger.h"
#include "pkgreader.h"
#include "pkgwriter.h"
#include "pathop.h"
#include "rowdatadict.h"
#include "sqlite3.h"
using namespace std;

#define RETURN_FALSE_IFNOT_OPENED() \
	if(!opened()){ \
		slog_err(pkglogger) << "package not opened yet!" << endl; \
		return false; \
	}
#define RETURN_IFNOT_OPENED() \
	if(!opened()){ \
		slog_err(pkglogger) << "package not opened yet!" << endl; \
		return; \
	}

RATEL_NAMESPACE_BEGIN

Package::Package(const Path& workdir)    
{
    if(workdir.isDirectory())
        workdir_ = workdir;
}

Package::~Package()
{
    close();
}

Path Package::obtainDBFilePath() const
{
    return workdir_.join(RString::NewUID() + ".db");
}

Path Package::obtainDataStorageFilePath() const
{
    return workdir_.join(RString::NewUID() + ".fds");
}

bool Package::createDir(const RString& name, const Path& location)
{
	logverify(pkglogger, !name.null());
	RETURN_FALSE_IFNOT_OPENED();
	int32_t parentid = 0;
	if(location.rstring() == "./")
		parentid = -1; //root dir
	else{
		parentid = pkgdb_->dirTable().queryDirId(location.cstr());
		if(parentid == -1){
			slog_err(pkglogger) << "dir(" << location.cstr() << ") not exists!" << endl;
			return false;
		}
		//check if exists redundant dir
		if(pkgdb_->dirTable().queryDirId(name, parentid) != -1){
			slog_err(pkglogger) << "sub dir(" << name.cstr() << ") already exists!" << endl;
			return false;
		}
	}
	Path newdirpath = location.join(name);
	if(!pkgdb_->dirTable().insertRow(RowDataDict({{DirTable::kPathKey, newdirpath.cstr()},
												{DirTable::kParentKey, parentid},
												{DirTable::kStatusKey, DirTable::NORMAL}}))){
		slog_err(pkglogger) << "insertRow(newdir:" << newdirpath.cstr() << " parentid:" << parentid << ") failed!" << endl;
		return false;
	}    
    return true;
}

bool Package::importDir(const Path& location, const Path& localdir)
{
	RETURN_FALSE_IFNOT_OPENED();
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
    vector<Path> subfiles, subdirs;
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

bool Package::exportDir(const Path& sourcedir, const Path& localdir)
 {
	RETURN_FALSE_IFNOT_OPENED();
	if(!localdir.exists()){
		slog_err(pkglogger) << "localdir(" << localdir.cstr() << ") not exists!" << endl;
		return false;
	}	
	int32_t dirid = pkgdb_->dirTable().queryDirId(sourcedir.rstring());
	if(dirid == -1){
		slog_err(pkglogger) << "sourcedir(" << sourcedir.cstr() << ") is invalid dirpath!" << endl;
		return false;
	}
	Path dirname = sourcedir.filename();
	Path newlocaldir = localdir.join(dirname);
	CreateDir(newlocaldir);
	return exportDirId(dirid, newlocaldir);
}

bool Package::exportDirId(int32_t dirid, const Path& localdir)
{
	std::vector<RowDataDict> resultrows;
	RowDataDict referencerow({{FileTable::kIdKey, Variant(Variant::kIntType)}, 
							{FileTable::kNameKey, Variant(Variant::kStringType)},
							{FileTable::kFileUIDKey, Variant(Variant::kStringType)}});
	if(!pkgdb_->fileTable().queryFilesOfDir(dirid, resultrows, referencerow)){
		slog_err(pkglogger) << "queryFilesOfDir dirid:" << dirid << " failed!" << endl;
		return false;
	}
	for(auto rowdata : resultrows){
		RString fn = rowdata[FileTable::kNameKey].convertToStr().cstr();
		Path localfilepath = localdir.join(fn);
		DataBlockStorage::UID fid = rowdata[FileTable::kFileUIDKey].convertToStr().cstr();
		if(!filedatastorage_->exportDataBlock(fid, localfilepath)){
			slog_err(pkglogger) << "exportDataBlock fid:" << fid.c_str() << " failed!" << endl;
			return false;
		}
	}	
	resultrows.clear();
	RowDataDict reference({{DirTable::kIdKey, Variant(Variant::kIntType)}, {DirTable::kPathKey, Variant(Variant::kStringType)}});	
	if(!pkgdb_->dirTable().querySubDirs(dirid, resultrows, reference))
		return false;
	for(auto rowdata : resultrows){
		int32_t did = rowdata[DirTable::kIdKey].convertToInt32();
		Path pkgsubdir = rowdata[DirTable::kPathKey].convertToStr();
		Path newsubdir = localdir.join(pkgsubdir.filename());
		CreateDir(newsubdir);
		if(!exportDirId(did, newsubdir)){
			slog_err(pkglogger) << "exportDirId did:" << dirid << " to local dir:" << newsubdir.cstr() << " failed!" << endl;
			return false;
		}
	}
	return true;
}

bool Package::importFile(const Path& dirlocation, const Path& sourcefile)
{
	if(!sourcefile.exists() || !sourcefile.isRegularFile()){
		slog_err(pkglogger) << "source file[" << sourcefile.cstr() << "] is invalid file!" << endl;
		return false;
	}	
	RETURN_FALSE_IFNOT_OPENED();
	int32_t dirid = pkgdb_->dirTable().queryDirId(dirlocation.rstring());
	if(dirid == -1){
		slog_err(pkglogger) << "dirlocation(" << dirlocation.cstr() << ") not exists!" << endl;
		return false;
	}
	const RString srcfn = sourcefile.filename().rstring();
	if(pkgdb_->fileTable().existsFile(srcfn, dirid)){
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
	DataBlockStorage::UID newfileuid = DataBlockStorage::NewUID();	
	filedatastorage_->appendDataBlock(newfileuid, wholedata, wholedatasize);
	RowDataDict newrow({
		{FileTable::kNameKey, srcfn.cstr()},
		{FileTable::kDirIdKey, dirid},
		{FileTable::kFileUIDKey, newfileuid.c_str()},
		{FileTable::kStatusKey, FileTable::NORMAL}
		});
	if(!pkgdb_->fileTable().insertRow(newrow)){
		filedatastorage_->removeDataBlock(newfileuid); //rollback data
		slog_err(pkglogger) << "insert new row data to FileTable failed!" << endl;
		return false;
	}
	return true;
}

bool Package::removeFile(const Path& filepath)
{	
	RETURN_FALSE_IFNOT_OPENED();
	RowDataDict resultdata({
		{FileTable::kIdKey, Variant(Variant::kIntType)},
		{DirTable::kIdKey, Variant(Variant::kIntType)},
		{FileTable::kFileUIDKey, Variant(Variant::kStringType)},
		});
	if(!pkgdb_->queryFile(filepath, resultdata)){
		slog_err(pkglogger) << "filepath(" << filepath.cstr() <<") is invalid!" << endl;
		return false;
	}
	DataBlockStorage::UID fid = resultdata[FileTable::kFileUIDKey].convertToStr().cstr();
	filedatastorage_->removeDataBlock(fid);
	return pkgdb_->fileTable().removeFile(resultdata[FileTable::kIdKey].convertToInt32());
}

bool Package::exportFile(const Path& sourcefile, const Path& localfile)
{
	RETURN_FALSE_IFNOT_OPENED();
	Path sourcefilename = sourcefile.filename();
	Path sourcedir = sourcefile.parentPath();
	RowDataDict resrowdata({{FileTable::kIdKey, Variant(Variant::kIntType)}, {FileTable::kFileUIDKey, Variant(Variant::kStringType)}});
	if(!pkgdb_->queryFile(sourcefile, resrowdata)){
		slog_err(pkglogger) << "sourcefile(" << sourcefile.cstr() << ") not exists!" << endl;
		return false;
	}
	DataBlockStorage::UID fid = resrowdata[FileTable::kFileUIDKey].convertToStr().cstr();
	return filedatastorage_->exportDataBlock(fid, localfile);
}

bool Package::load(const Path& pkgpath)
{
    Path dbfilepath = obtainDBFilePath();
	Path storagefilepath = obtainDataStorageFilePath();
	PKGReader reader;
	if(!reader.loadFile(pkgpath, dbfilepath, storagefilepath)){
		slog_err(pkglogger) << "load pkg path(" << pkgpath.cstr() << ") failed and it's invalid package file!";
		return false;
	}
	pkgdb_ = new PKGDB(dbfilepath, SQLITE_OPEN_READWRITE);    
	filedatastorage_ = new DataBlockStorage(storagefilepath.toWString());
	if(!filedatastorage_->load()){
		close();
		slog_err(pkglogger) << "invalid data storage file(" << storagefilepath.cstr() << ")!" << endl;
		return false;
	}
	pkgfile_ = pkgpath;
    return true;
}

bool Package::createNew(const Path& newpkgpath)
{
	if(opened()){
		slog_err(pkglogger) <<"package is already opened!" << endl;
		return false;
	}
    if(!workdir_.isDirectory()){
		log_err(pkglogger, "workdir(%s) is not valid directory!", workdir_.cstr());
        return false;
    }
    pkgfile_ = newpkgpath;
    if(!pkgfile_.parentPath().exists()){
		log_err(pkglogger, "new package filepath(%s) is invalid!", pkgfile_.cstr());
        return false;
    }
    Path dbfilepath = obtainDBFilePath();
    pkgdb_ = new PKGDB(dbfilepath, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);    
	Path datafilepath = obtainDataStorageFilePath();
	filedatastorage_ = new DataBlockStorage(datafilepath.toWString());
	filedatastorage_->initEmpty();
    return true;
}

void Package::commit()
{
	RETURN_IFNOT_OPENED(); 	
	Path datafilepath = filedatastorage_->filePath();
    PKGWriter pkgwriter(pkgfile_, pkgdb_->dbFilePath(), datafilepath);
	if(!pkgwriter.write()){
		slog_err(pkglogger) << "write data to package file failed!" << endl;
		return;
	}
}

bool Package::opened() const
{
    return pkgdb_ != nullptr;
}

void Package::close()
{
	if(!opened())
		return;
	rtdelete(pkgdb_);
	pkgdb_ = nullptr;
	rtdelete(filedatastorage_);
	filedatastorage_ = nullptr;
	RemoveDir(workdir_);
	CreateDir(workdir_);//clean work directory
}

RATEL_NAMESPACE_END
