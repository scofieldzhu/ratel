/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: tinypack
Module: tinypack.cpp
CreateTime: 2019-04-09 21:32
=========================================================================*/
#include <iostream>
#include <ShlObj.h>
#include "package.h"
#include "pathop.h"
using namespace std;
USING_RATEL

/*
the following commands can be used:
	--help		display help information.
	--version   display version information.
	new			create new package file and open it.
	export		export file/dir to local system.
	import		import local file/dir to package.
	rmdir		remove specific package directory.
	rmfile      remove specific package file.	
the following command flags can be used:
	-lf			local file path
	-ld			local directory
	-pf         package file path
	-pd         package directory
*/

namespace cmdkey{
	const char* kHelp = "--help";
	const char* kVersion = "--version";
	const char* kNew = "new";
	const char* kExport = "export";
	const char* kImport = "import";
	const char* kRmdir = "rmdir";
	const char* kRmfile = "rmfile";
	const RStrings kAllCmdKeys = { kHelp, kVersion, kNew, kExport, kImport, kRmdir, kRmfile };	
}

namespace cmdflag{
	const char* kLF = "-lf";
	bool IsLF(const RString& str){ return str == kLF; }
	const char* kLD = "-ld";
	bool IsLD(const RString& str){ return str == kLD; }
	const char* kPF = "-pf";
	bool IsPF(const RString& str){ return str == kPF; }
	const char* kPD = "-pd";
	bool IsPD(const RString& str){ return str == kPD; }	
	const RStrings kAllCmdFlags = { kLF, kLD, kPF, kPD };
}

void HandleHelpCmd(const RStrings& cmdparams);
void HandleVersionCmd(const RStrings& cmdparams);
void HandleNewCmd(const RStrings& cmdparams);
void HandleExportCmd(const RStrings& cmdparams);
void HandleImportCmd(const RStrings& cmdparams);
void HandleRmDirCmd(const RStrings& cmdparams);
void HandleRmFileCmd(const RStrings& cmdparams);

#define INVALID_CMDLINE_FORMAT() cout << "invalid cmdline format. please hit '--help'!" << endl;

int main(int argc, const char* argv[])
{
	RStrings cmdparams;
	for(auto i = 1; i < argc; ++i)
		cmdparams.push_back(RString::FromLocale(argv[i]));
	if(cmdparams.empty()){
		INVALID_CMDLINE_FORMAT();
		return 0;
	}	
	RString thecmdkey = cmdparams[0];
	cmdparams.erase(cmdparams.begin());
	if(thecmdkey == cmdkey::kHelp){
		HandleHelpCmd(cmdparams);
		return 0;
	}
	if(thecmdkey == cmdkey::kVersion){
		HandleVersionCmd(cmdparams);
		return 0;
	}
	if(thecmdkey == cmdkey::kNew){
		HandleNewCmd(cmdparams);
		return 0;
	}
	if(thecmdkey == cmdkey::kExport){
		HandleExportCmd(cmdparams);
		return 0;
	}
	if(thecmdkey == cmdkey::kImport){
		HandleImportCmd(cmdparams);
		return 0;
	}
	if(thecmdkey == cmdkey::kRmdir){
		HandleRmDirCmd(cmdparams);
		return 0;
	}
	if(thecmdkey == cmdkey::kRmfile){
		HandleRmFileCmd(cmdparams);
		return 0;
	}
	return 0;
}

void HandleHelpCmd(const RStrings& cmdparams)
{
	cout << "the following commands can be used :\r\n"
			"--help		display help information.\r\n"
			"--version  display version information.\r\n"
			"new		create new package file and open it.\r\n"
			"export		export file / dir to local system.\r\n"
			"import		import local file / dir to package.\r\n"
			"rmdir		remove specific package directory.\r\n"
			"rmfile     remove specific package file.\r\n"
			"the following flags can be combined to use:\r\n"
			"-lf		local file path\r\n"
			"-ld		local directory\r\n"
			"-pf        package file path\r\n"
			"-pd        package directory\r\n";
}

void HandleVersionCmd(const RStrings& cmdparams)
{
	cout << "======================\r\n";
	cout << "TinyPack(1.0)\r\n";
	cout << "A simple compress tool!\r\n";
	cout << "======================\r\n";
}

Path GetCurrentUserDirectory()
{
	wchar_t path[MAX_PATH] = {L'\0'};
	if(!SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))){
		cout << "Obtain current user directory failed!" << endl;
		return Path("./");
	}
	return path;
}

Path ObtainWorkDirectory()
{
	Path workdir = GetCurrentUserDirectory().join("tinypack");
	if(workdir.exists()) //clean up work directory
		RemoveDir(workdir);		
	CreateDir(workdir);
	return workdir;
}

//cmdline format:
//new package file command like "TinyPack new -lf NewPkgFilePath"
void HandleNewCmd(const RStrings& cmdparams)
{
	do{
		if(cmdparams.size() != 2)
			break;
		if(!cmdflag::IsLF(cmdparams[0]))
			break;		
		Path pkgfilepath = RString::FromLocale(cmdparams[1].cstr());
		Package pkg(ObtainWorkDirectory());
		if(!pkg.createNew(pkgfilepath)) //auto commit after operation
			cout << "create package file[" << pkgfilepath.cstr() << "] failed!" << endl;		
		return;
	}while(0);
	INVALID_CMDLINE_FORMAT();
}

//cmdline format:
//export directory like this:" TinyPack export -lf ExistingPkgFilePath -pd pkgdir -ld localdir"
//export file like this:" TinyPack export -lf ExistingPkgFilePath -pf pkgfilepath -lf localfile"
void HandleExportCmd(const RStrings& cmdparams)
{
	RStrings leftparams = cmdparams;
	do{
		if(leftparams.size() != 6)
			break;
		RString curkeystr = leftparams.front();
		if(!cmdflag::IsLF(curkeystr))
			break;
		leftparams.erase(leftparams.begin());
		Path pkgfilepath = leftparams.front();
		leftparams.erase(leftparams.begin());
		if(cmdflag::IsPD(leftparams[0])){
			if (!cmdflag::IsLD(leftparams[2]))
				break;
			//export package directory
			Path targetpkgdir = leftparams[1];
			Path targetlocaldir = leftparams[3];
			Package pkg(ObtainWorkDirectory());
			if(!pkg.load(pkgfilepath)){
				cout << "load package file[" << pkgfilepath.cstr() << "] failed!" << endl;
				return;
			}
			if(!pkg.exportDir(targetpkgdir, targetlocaldir)){
				cout << "export package directory[" << targetpkgdir.cstr() << "] to local directory[" << targetlocaldir.cstr() << "] failed!" << endl;
				return;
			}
			return;
		} else if(cmdflag::IsLF(cmdparams[0])){
			if(!cmdflag::IsLF(leftparams[2]))
				break;
			//export local directory
			Path targetpkgfile = leftparams[1];
			Path targetlocalfile = leftparams[3];
			Package pkg(ObtainWorkDirectory());
			if(!pkg.load(pkgfilepath)){
				cout << "load package file[" << pkgfilepath.cstr() << "] failed!" << endl;
				return;
			}
			if(!pkg.exportFile(targetpkgfile, targetlocalfile)){
				cout << "export package file[" << targetpkgfile.cstr() << "] to local file[" << targetlocalfile.cstr() << "] failed!" << endl;
				return;
			}
			return;
		} else
			break;
	} while (0);
	INVALID_CMDLINE_FORMAT();
}

//cmdline format:
//import directory like this:" TinyPack import -lf ExistingPkgFilePath -pd pkgdir -ld localdir"
//import file like this:" TinyPack import -lf ExistingPkgFilePath -pf pkgfilepath -lf localfile"
void HandleImportCmd(const RStrings& cmdparams)
{
	RStrings leftparams = cmdparams;
	do{
		if(leftparams.size() != 6)
			break;
		RString curkeystr = leftparams.front();
		if(!cmdflag::IsLF(curkeystr))
			break;
		leftparams.erase(leftparams.begin());
		Path pkgfilepath = leftparams.front();
		leftparams.erase(leftparams.begin());
		if(cmdflag::IsPD(leftparams[0])){
			if(!cmdflag::IsLD(leftparams[2]))
				break;
			//import local directory
			Path targetpkgdir = leftparams[1];
			Path targetlocaldir = leftparams[3];
			Package pkg(ObtainWorkDirectory());
			if(!pkg.load(pkgfilepath)){
				cout << "load package file[" << pkgfilepath.cstr() << "] failed!" << endl;
				return;
			}
			if(!pkg.importDir(targetpkgdir, targetlocaldir)){
				cout << "import local directory[" << targetlocaldir.cstr() << "] to package directory[" << targetpkgdir.cstr() <<"] failed!" << endl;
				return;
			}
			pkg.commit();
			return;
		}else if(cmdflag::IsLD(cmdparams[0])){
			if(!cmdflag::IsLF(leftparams[2]))
				break;
			//import local directory
			Path targetpkgdir = leftparams[1];
			Path targetlocalfile = leftparams[3];
			Package pkg(ObtainWorkDirectory());
			if(!pkg.load(pkgfilepath)){
				cout << "load package file[" << pkgfilepath.cstr() << "] failed!" << endl;
				return;
			}
			if(!pkg.importFile(targetpkgdir, targetlocalfile)){
				cout << "import local file[" << targetlocalfile.cstr() << "] to package directory[" << targetpkgdir.cstr() << "] failed!" << endl;
				return;
			}
			pkg.commit();
			return;
		}else
			break;

	} while (0);
	INVALID_CMDLINE_FORMAT();
}

//cmdline format:
//remove package directory like this:" TinyPack rmdir -lf ExistingPkgFilePath -pd pkgdir"
void HandleRmDirCmd(const RStrings& cmdparams)
{
	RStrings leftparams = cmdparams;
	do{
		if(leftparams.size() != 4)
			break;
		RString curkeystr = leftparams.front();
		if(!cmdflag::IsLF(curkeystr))
			break;
		leftparams.erase(leftparams.begin());
		Path pkgfilepath = leftparams.front();
		leftparams.erase(leftparams.begin());
		if(!cmdflag::IsPD(leftparams[0]))
			break;
		Path pkgdir = leftparams[1];
		Package pkg(ObtainWorkDirectory());
		if(!pkg.load(pkgfilepath)){
			cout << "load package file[" << pkgfilepath.cstr() << "] failed!" << endl;
			return;
		}
		if(!pkg.removeDir(pkgdir))
			cout << "remove package dir[" << pkgdir.cstr() << "] failed!" << endl;		
		return;
	}while (0);
	INVALID_CMDLINE_FORMAT();
}

//cmdline format:
//remove package file like this:" TinyPack rmfile -lf ExistingPkgFilePath -pf pkgfile"
void HandleRmFileCmd(const RStrings& cmdparams)
{
	RStrings leftparams = cmdparams;
	do{
		if(leftparams.size() != 4)
			break;
		RString curkeystr = leftparams.front();
		if(!cmdflag::IsLF(curkeystr))
			break;
		leftparams.erase(leftparams.begin());
		Path pkgfilepath = leftparams.front();
		leftparams.erase(leftparams.begin());
		if(!cmdflag::IsPF(leftparams[0]))
			break;
		Path pkgfile = leftparams[1];
		Package pkg(ObtainWorkDirectory());
		if(!pkg.load(pkgfilepath)){
			cout << "load package file[" << pkgfilepath.cstr() << "] failed!" << endl;
			return;
		}
		if(!pkg.removeFile(pkgfile))
			cout << "remove package file[" << pkgfile.cstr() << "] failed!" << endl;
		return;
	}while(0);
	INVALID_CMDLINE_FORMAT();
}
