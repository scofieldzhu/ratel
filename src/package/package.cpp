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

Path Package::newDBFilePath() const
{
    return workdir_.join(RString::NewUID() + ".db");
}

Path Package::newDataStorageFilePath() const
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
	RString fn = RString::NewUID();		
	{
		//create new data file
		DataBlockStorage newds(workdir_.join(fn).toWString());
		newds.initEmpty();
	}	
	if(!pkgdb_->dirTable().insertRow(RowDataDict({{DirTable::kPathKey, newdirpath.cstr()},
												{DirTable::kParentKey, parentid},
												{DirTable::kDataFileUIDKey, fn.cstr()},
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
	RETURN_FALSE_IFNOT_OPENED();
	int32_t dirid = pkgdb_->dirTable().queryDirId(dir.rstring());
	if(dirid == -1){
		slog_err(pkglogger) << "package directory[" << dir.cstr() << "] not exists!" << endl;
		return false;
	}
    return doRemoveDir(dirid);
}

bool Package::doRemoveDir(int32_t dirid)
{
	vector<int32_t> subids;
	pkgdb_->dirTable().querySubDirIds(dirid, subids);
	pkgdb_->dirTable().removeDir(dirid);
	pkgdb_->fileTable().removeDirFiles(dirid);
	for(auto sid : subids){
		if(!doRemoveDir(sid))
			return false;
	}
	return true;
}

bool Package::exportDir(const Path& sourcedir, const Path& localdir)
 {
	RETURN_FALSE_IFNOT_OPENED();
	if(!localdir.exists()){
		slog_err(pkglogger) << "localdir(" << localdir.cstr() << ") not exists!" << endl;
		return false;
	}	
	RowDataDict resrowdata({
		{DirTable::kIdKey, Variant(Variant::kIntType)},
		{DirTable::kDataFileUIDKey, Variant(Variant::kStringType)}
	});
	if(!pkgdb_->dirTable().queryDir(sourcedir.rstring(), resrowdata)){
		slog_err(pkglogger) << "sourcedir(" << sourcedir.cstr() << ") is invalid dirpath!" << endl;
		return false;
	}
	int32_t dirid = resrowdata[DirTable::kIdKey].toInt32();
	RString dbsfn = resrowdata[DirTable::kDataFileUIDKey].toStr();
	DataBlockStorage newds(workdir_.join(dbsfn).toWString());
	logverify(pkglogger, newds.load());
	Path newlocaldir = localdir.join(sourcedir.filename());
	CreateDir(newlocaldir);
	return exportDirId(dirid, newds, newlocaldir);
}

bool Package::exportDirId(int32_t dirid, DataBlockStorage& dbs, const Path& localdir)
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
		RString fn = rowdata[FileTable::kNameKey].toStr().cstr();
		DataBlockStorage::UID fid = rowdata[FileTable::kFileUIDKey].toStr().cstr();
		if(!dbs.exportDataBlock(fid, localdir.join(fn))){
			slog_err(pkglogger) << "exportDataBlock fid:" << fid.c_str() << " failed!" << endl;
			return false;
		}
	}	
	resultrows.clear();
	RowDataDict reference({
		{DirTable::kIdKey, Variant(Variant::kIntType)},
		{DirTable::kDataFileUIDKey, Variant(Variant::kStringType)},
		{DirTable::kPathKey, Variant(Variant::kStringType)}
	});	
	if(!pkgdb_->dirTable().querySubDirs(dirid, resultrows, reference))
		return false;
	for(auto rowdata : resultrows){
		RString dbsfn = rowdata[DirTable::kDataFileUIDKey].toStr();
		DataBlockStorage dirdbs(workdir_.join(dbsfn).toWString());
		logverify(pkglogger, dirdbs.load());
		int32_t did = rowdata[DirTable::kIdKey].toInt32();		
		Path pkgdir = rowdata[DirTable::kPathKey].toStr();		
		Path localsubdir = localdir.join(pkgdir.filename());
		CreateDir(localsubdir);
		if(!exportDirId(did, dirdbs, localsubdir)){
			slog_err(pkglogger) << "exportDirId did:" << dirid << " to local dir:" << localsubdir.cstr() << " failed!" << endl;
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
	RowDataDict dirdata({
		{DirTable::kIdKey, Variant(Variant::kIntType)},
		{DirTable::kDataFileUIDKey, Variant(Variant::kStringType)},
	});
	if(!pkgdb_->dirTable().queryDir(dirlocation.rstring(), dirdata)){
		slog_err(pkglogger) << "dirlocation(" << dirlocation.cstr() << ") not exists!" << endl;
		return false;
	}
	const int32_t kTheDirId = dirdata[DirTable::kIdKey].toInt32();
	RString dsfn = dirdata[DirTable::kDataFileUIDKey].toStr();
	const Path dsfilepath = workdir_.join(dsfn);
	DataBlockStorage thedbs(dsfilepath.toWString());
	if(!thedbs.load()){
		slog_err(pkglogger) << "load data block storage file(" << dsfilepath.cstr() << ") failed!" << endl;
		return false;
	}
	return doImportFile(thedbs, kTheDirId, sourcefile);
}

bool Package::doImportFile(DataBlockStorage& dbs, int32_t dirid, const Path& sourcefile)
{
	const RString kSrcFn = sourcefile.filename().rstring();
	if(pkgdb_->fileTable().existsFile(kSrcFn, dirid)){
		slog_err(pkglogger) << "same file(" << kSrcFn.cstr() << ") already exists in location dirid(" << dirid << ")!" << endl;
		return false;
	}
	DataBlockStorage::UID newfileuid = DataBlockStorage::NewUID();
	dbs.appendDataBlock(newfileuid, sourcefile);
	RowDataDict newrow({
		{FileTable::kNameKey, kSrcFn.cstr()},
		{FileTable::kDirIdKey, dirid},
		{FileTable::kFileUIDKey, newfileuid.c_str()},
		{FileTable::kStatusKey, FileTable::NORMAL}
	});
	if(!pkgdb_->fileTable().insertRow(newrow)){
		dbs.removeDataBlock(newfileuid); //rollback data
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
		{DirTable::kDataFileUIDKey, Variant(Variant::kStringType)},
		{FileTable::kFileUIDKey, Variant(Variant::kStringType)},
		});
	if(!pkgdb_->queryFile(filepath, resultdata)){
		slog_err(pkglogger) << "file(" << filepath.cstr() <<") not exists!" << endl;
		return false;
	}
	RString dbsfn = resultdata[DirTable::kDataFileUIDKey].toStr();
	Path dbsfilepath = workdir_.join(dbsfn);
	DataBlockStorage dbs(dbsfilepath.toWString());
	if(!dbs.load()){
		slog_err(pkglogger) << "load data block file(" << dbsfilepath.cstr() <<") failed!" << endl;
		return false;
	}
	DataBlockStorage::UID fid = resultdata[FileTable::kFileUIDKey].toStr().cstr();
	dbs.removeDataBlock(fid);
	return pkgdb_->fileTable().removeFile(resultdata[FileTable::kIdKey].toInt32());
}

bool Package::exportFile(const Path& sourcefile, const Path& localfile)
{
	RETURN_FALSE_IFNOT_OPENED();
	Path sourcefilename = sourcefile.filename();
	Path sourcedir = sourcefile.parentPath();
	RowDataDict resrowdata({
		{FileTable::kIdKey, Variant(Variant::kIntType)},
		{DirTable::kDataFileUIDKey, Variant(Variant::kStringType)},
		{FileTable::kFileUIDKey, Variant(Variant::kStringType)}
		});
	if(!pkgdb_->queryFile(sourcefile, resrowdata)){
		slog_err(pkglogger) << "sourcefile(" << sourcefile.cstr() << ") not exists!" << endl;
		return false;
	}
	RString dbsfn = resrowdata[DirTable::kDataFileUIDKey].toStr();
	Path dbsfilepath = workdir_.join(dbsfn);
	DataBlockStorage dbs(dbsfilepath.toWString());
	if(!dbs.load()){
		slog_err(pkglogger) << "load data block file(" << dbsfilepath.cstr() << ") failed!" << endl;
		return false;
	}
	DataBlockStorage::UID fid = resrowdata[FileTable::kFileUIDKey].toStr().cstr();
	return dbs.exportDataBlock(fid, localfile);
}

bool Package::load(const Path& pkgpath)
{    
	Path dbfn;
	PKGReader reader;	
 	if(!reader.load(pkgpath, workdir_, dbfn)){
 		slog_err(pkglogger) << "load package file(" << pkgpath.cstr() << ") failed!";
 		return false;
 	}
	pkgdb_ = new PKGDB(workdir_.join(dbfn), SQLITE_OPEN_READWRITE);    
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
    pkgdb_ = new PKGDB(workdir_.join(RString::NewUID() + ".db"), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);    
    return true;
}

void Package::commit()
{
	RETURN_IFNOT_OPENED(); 	
    PKGWriter w;
	if(!w.reInit(pkgfile_, pkgdb_->dbFilePath())){
		slog_err(pkglogger) << " PKGWriter reInit failed! pkgfile[" << pkgfile_.cstr() << "]" << endl;
		return;
	}
	RStrings fns = pkgdb_->dirTable().queryAllDataFiles();
	for(auto fn : fns)
		w.writeDataFile(workdir_.join(fn));
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
	RemoveDir(workdir_);
	CreateDir(workdir_);//clean work directory
}

RATEL_NAMESPACE_END
