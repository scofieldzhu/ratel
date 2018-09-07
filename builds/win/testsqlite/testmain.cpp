#include <iostream>
#include "db.h"
#include "table.h"
#include "tablecol.h"
#include "tablerow.h"
using namespace std;
USING_RATEL

int main()
{    
    DB* db = DB::OpenDB("test.db", SQLITE_OPEN_READWRITE);
    if (db == nullptr) {
        cout << "Open test.db failed!" << endl;
        return 0;
    }

    Table t("Directory");
    t.addColumn(IntCol("dirid").setPrimaryKey(true).setUnique(true).setNotNull(true));
    t.addColumn(StrCol("name", 30));
    t.addColumn(IntCol("fileid").setNotNull(true));
    t.addColumn(IntCol("childdirid").setNotNull(true));

    RString cursql = t.makeDropSql(); 
    cursql = t.makeCreateSql();
    sqlite3_stmt* stmt = db->prepareStatement(cursql);
    int32 rescode = db->stepExec(stmt);
    db->destroyStatement(stmt);

    Table files("File");
    files.addColumn(IntCol("fileid").setPrimaryKey(true).setUnique(true).setNotNull(true));
    files.addColumn(StrCol("name", 30));
    files.addColumn(StrCol("diskfileuid", 100));
    cursql = files.makeCreateSql();
    cout << cursql.cstr() << endl;
    stmt = db->prepareStatement(cursql);
    rescode = db->stepExec(stmt);
    db->destroyStatement(stmt);

    RString testsql = "insert into Company(id, name, manager, age, factor) values(1, \"chenjj\", \"zhucg\", 31, 0.36);";
    stmt = db->prepareStatement(testsql);
    rescode = db->stepExec(stmt);
    db->destroyStatement(stmt);

    testsql = "insert into Company(id, name, manager, age, factor) values(2, \"zhhb\", \"zhucg\", 31, 1.26);" \
        "insert into Company(id, name, manager, age, factor) values(3, \"libing\", \"zhucg\", 32, 0.37);";
    stmt = db->prepareStatement(testsql);
    rescode = db->exec(stmt, testsql, nullptr, nullptr);
    db->destroyStatement(stmt);

    testsql = "select * from Company;";
    stmt = db->prepareStatement(testsql);
    while ((rescode = db->stepExec(stmt)) == SQLITE_ROW) {
        TableRow row(stmt);
        int32 id = row.queryInteger(0);
        RString name = row.queryText(1);
        RString manager = row.queryText(2);
        int32 age = row.queryInteger(3);
        double factor = row.queryReal(4);
        cout << "id:" << id << " name:" << name.cstr() << " manager:" << manager.cstr() << " age:" << age << " factor:" << factor << endl;
    }
    db->destroyStatement(stmt);

    delete db;

    return 0;
}
