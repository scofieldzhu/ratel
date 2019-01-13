#include <iostream>
#include "db.h"
#include "statement.h"
#include "sqlite3.h"
#include "path.h"
#include "dbtable.h"
#include "dbtablecol.h"
using namespace std;
USING_RATEL

int main()
{    
    DB* db = DB::OpenDB("test.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    if (db == nullptr) {
        cout << "Open test.db failed!" << endl;
        return 0;
    }

    DbTable t("Directory");
    t.addColumn(IntTabCol("dirid").setPrimaryKey(true).setUnique(true).setNotNull(true));
    t.addColumn(StrTabCol("name", 30));
    t.addColumn(IntTabCol("fileid").setNotNull(true));
    t.addColumn(IntTabCol("childdirid").setNotNull(true));

    RString cursql = t.makeCreateSql();
    Statement* stat = db->createStatement(cursql);
    int32 rescode = stat->stepExec();
    delete stat;

    DbTable files("File");
    files.addColumn(IntTabCol("fileid").setPrimaryKey(true).setUnique(true).setNotNull(true));
    files.addColumn(StrTabCol("name", 30));
    files.addColumn(StrTabCol("diskfileuid", 100));
    cursql = files.makeCreateSql();
    cout << cursql.cstr() << endl;
    stat = db->createStatement(cursql);
    rescode = stat->stepExec();
    delete stat;

    cursql = t.makeInsertSql({ "name", "fileid", "childdirid"}, "'%s',%d,%d", "testdir", 1, 11);        
    stat = db->createStatement(cursql);
    rescode = stat->stepExec();
    delete stat;
    cursql = t.makeInsertSql({ "name", "fileid", "childdirid" }, "'%s',%d,%d", "testdir1", 2, 111);
    stat = db->createStatement(cursql);
    rescode = stat->stepExec();
    delete stat;
    cursql = t.makeInsertSql({ "name", "fileid", "childdirid" }, "'%s',%d,%d", "testdir2", 22, 1112);
    stat = db->createStatement(cursql);
    rescode = stat->stepExec();
    delete stat;

    cursql = files.makeInsertSql({"name", "diskfileuid" }, "'%s','%s'", "chjj", "kkdkk-fdsf--sdf");
    stat = db->createStatement(cursql);
    rescode = stat->stepExec();
    delete stat;
// 
//     testsql = "insert into Company(id, name, manager, age, factor) values(2, \"zhhb\", \"zhucg\", 31, 1.26);" \
//         "insert into Company(id, name, manager, age, factor) values(3, \"libing\", \"zhucg\", 32, 0.37);";
//     stat = db->createStatement(testsql);
//     int rsc = stat->exec(testsql, nullptr, nullptr);
//     delete stat;
// 
    cursql = "select * from Directory;";
    stat = db->createStatement(cursql);
    while((rescode = stat->stepExec()) == SQLITE_ROW) {
        int32 id = stat->fetchIntColumn(0);
        RString name = stat->fetchTextColumn(1);
        int32 fid = stat->fetchIntColumn(2);
        int32 cfid = stat->fetchIntColumn(3);
        cout << "id:" << id << " name:" << name.cstr() << " fid:" << fid << " cfid:" << cfid << endl;
    }
    delete stat;

    delete db;

    return 0;
}
