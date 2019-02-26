#include <iostream>
#include "db.h"
#include "statement.h"
#include "sqlite3.h"
#include "path.h"
#include "dbtable.h"
#include "dbtablecol.h"
#include "sqldatameta.h"
#include "dbtablerecord.h"
using namespace std;
USING_RATEL

namespace dircols{
	const RString kDirId = "dirid";
	const RString kName = "name";
	const RString kFileId = "fileid";
	const RString kChildDirId = "childdirid";
	const RString kDiskFileUid = "diskfileuid";
}

int main()
{    
    DB* db = DB::OpenDB("test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    if (db == nullptr) {
        cout << "Open test.db failed!" << endl;
        return 0;
    }

    DbTable t("Directory");
	PropDict* colprops = &(t.addNewColumn(dircols::kDirId).setDataMeta(IntSqlDataMeta()).propDict());
	colprops->setPropStatusOn(sqlkw::kPrimaryKey).setPropStatusOn(sqlkw::kUnique).setPropStatusOn(sqlkw::kNotNull);
	t.addNewColumn(dircols::kName).setDataMeta(StrSqlDataMeta(30)).propDict();
	t.addNewColumn(dircols::kFileId).setDataMeta(IntSqlDataMeta()).propDict().setPropStatusOn(sqlkw::kNotNull);
	t.addNewColumn(dircols::kChildDirId).setDataMeta(IntSqlDataMeta()).propDict().setPropStatusOn(sqlkw::kNotNull);

    RString cursql = t.makeCreateSql();
	cout << "cursql:" << cursql.cstr() << endl;
    Statement* stat = db->createStatement(cursql);
    int32_t rescode = stat->stepExec();
    delete stat;

    DbTable filetable("File");
	colprops = &(filetable.addNewColumn(dircols::kFileId).setDataMeta(IntSqlDataMeta()).propDict());
	colprops->setPropStatusOn(sqlkw::kPrimaryKey).setPropStatusOn(sqlkw::kUnique).setPropStatusOn(sqlkw::kNotNull);
	filetable.addNewColumn(dircols::kName).setDataMeta(StrSqlDataMeta(30));
	filetable.addNewColumn(dircols::kDiskFileUid).setDataMeta(StrSqlDataMeta(100));
    cursql = filetable.makeCreateSql();
    cout << cursql.cstr() << endl;
    stat = db->createStatement(cursql);
    rescode = stat->stepExec();
    delete stat;

	DbTableRecord record({
		{dircols::kDirId, Variant(1)},
		{dircols::kName, Variant("testdir")},
		{dircols::kFileId, Variant(1)}, 
		{dircols::kChildDirId, Variant(11)}
		});		
	cursql = t.makeInsertRowSql(record);
	cout << cursql.cstr() << endl;
    stat = db->createStatement(cursql);
    rescode = stat->stepExec();
    delete stat;

//     cursql = t.makeInsertRowSql({ "name", "fileid", "childdirid" }, "'%s',%d,%d", "testdir1", 2, 111);
//     stat = db->createStatement(cursql);
//     rescode = stat->stepExec();
//     delete stat;
//     cursql = t.makeInsertRowSql({ "name", "fileid", "childdirid" }, "'%s',%d,%d", "testdir2", 22, 1112);
//     stat = db->createStatement(cursql);
//     rescode = stat->stepExec();
//     delete stat;
// 
//     cursql = files.makeInsertRowSql({"name", "diskfileuid" }, "'%s','%s'", "chjj", "kkdkk-fdsf--sdf");
//     stat = db->createStatement(cursql);
//     rescode = stat->stepExec();
//     delete stat;
// 
//     testsql = "insert into Company(id, name, manager, age, factor) values(2, \"zhhb\", \"zhucg\", 31, 1.26);" \
//         "insert into Company(id, name, manager, age, factor) values(3, \"libing\", \"zhucg\", 32, 0.37);";
//     stat = db->createStatement(testsql);
//     int rsc = stat->exec(testsql, nullptr, nullptr);
//     delete stat;
// 
//     cursql = "select * from Directory;";
//     stat = db->createStatement(cursql);
//     while((rescode = stat->stepExec()) == SQLITE_ROW) {
//         int32_t id = stat->fetchIntColumn(0);
//         RString name = stat->fetchTextColumn(1);
//         int32_t fid = stat->fetchIntColumn(2);
//         int32_t cfid = stat->fetchIntColumn(3);
//         cout << "id:" << id << " name:" << name.cstr() << " fid:" << fid << " cfid:" << cfid << endl;
//     }
//     delete stat;
// 
    delete db;

    return 0;
}
