/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: pkgdb.cpp 
CreateTime: 2019-3-6 21:30
=========================================================================*/
#include "pkgdb.h"
#include "path.h"
#include "rowdatadict.h"

RATEL_NAMESPACE_BEGIN

PKGDB::PKGDB(const Path& dbfile, int32_t flags, const char* zvfs /*= nullptr*/)
	:DB(dbfile, flags, zvfs)
{
	dirtable_.connectDB(*this);
	filetable_.connectDB(*this);
}

PKGDB::~PKGDB()
{}

int32_t PKGDB::queryFileId(const RString& filename, const RString& parentdir)
{
	RString sql = RString::FormatString("SELECT %s.%s, %s.%s FROM ", 
										filetable_.name().cstr(),
										FileTable::kIdKey.cstr(), 
										filetable_.name().cstr(),
										FileTable::kFileUIDKey.cstr());
	sql += RString::FormatString("%s INNER JOIN %s ON %s.%s=%s.%s ",
								dirtable_.name().cstr(),
								filetable_.name().cstr(),
								dirtable_.name().cstr(),
								DirTable::kIdKey.cstr(),
								filetable_.name().cstr(),
								FileTable::kDirIdKey.cstr());
	sql += RString::FormatString("WHERE %s.%s='%s' and %s.%s='%s';",
								dirtable_.name().cstr(),
								DirTable::kPathKey.cstr(),
								filetable_.name().cstr(),
								FileTable::kNameKey.cstr());		
	Variant data(Variant::kIntType);
	return queryColumnValueOfFirstResultRow(sql, 0, data) ? data.convertToInt32() : -1;
}

bool PKGDB::queryFile(const Path& absfilepath, RowDataDict& resultdata)
{
	RString filename = absfilepath.filename().rstring();
	RString parentpath = absfilepath.parentPath().rstring();
	bool existidkey = resultdata.existKey(DirTable::kIdKey);
	if(!existidkey)
		resultdata.insertPair(DirTable::kIdKey, Variant(Variant::kIntType));
	if(!dirtable_.queryDir(parentpath, resultdata))
		return false;
	int32_t dirid = resultdata.getValue(DirTable::kIdKey).convertToInt32();
	if(!existidkey)
		resultdata.remove(DirTable::kIdKey);
	return filetable_.queryFile(filename, dirid, resultdata);
}

RATEL_NAMESPACE_END
