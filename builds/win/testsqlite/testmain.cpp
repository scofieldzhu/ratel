#include <iostream>
#include "db.h"
#include "statement.h"
#include "openflag.h"
#include "path.h"
#include "table.h"
#include "tablecol.h"
using namespace std;
USING_RATEL

int main()
{    
    DB* db = DB::OpenDB("test.db", RATEL_DB_OPEN_READWRITE);
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
    Statement* stat = db->createStatement(cursql);
    int32 rescode = stat->stepExec();
    delete stat;

    Table files("File");
    files.addColumn(IntCol("fileid").setPrimaryKey(true).setUnique(true).setNotNull(true));
    files.addColumn(StrCol("name", 30));
    files.addColumn(StrCol("diskfileuid", 100));
    cursql = files.makeCreateSql();
    cout << cursql.cstr() << endl;
    stat = db->createStatement(cursql);
    rescode = stat->stepExec();
    delete stat;

    RString testsql = "insert into Company(id, name, manager, age, factor) values(1, \"chenjj\", \"zhucg\", 31, 0.36);";
    stat = db->createStatement(testsql);
    rescode = stat->stepExec();
    delete stat;

    testsql = "insert into Company(id, name, manager, age, factor) values(2, \"zhhb\", \"zhucg\", 31, 1.26);" \
        "insert into Company(id, name, manager, age, factor) values(3, \"libing\", \"zhucg\", 32, 0.37);";
    stat = db->createStatement(testsql);
    rescode = stat->exec(testsql, nullptr, nullptr);
    delete stat;

    testsql = "select * from Company;";
    stat = db->createStatement(testsql);
    while((rescode = stat->stepExec()) == RESCODE_ROW) {
        int32 id = stat->fetchIntColumn(0);
        RString name = stat->fetchTextColumn(1);
        RString manager = stat->fetchTextColumn(2);
        int32 age = stat->fetchIntColumn(3);
        double factor = stat->fetchDoubleColumn(4);
        cout << "id:" << id << " name:" << name.cstr() << " manager:" << manager.cstr() << " age:" << age << " factor:" << factor << endl;
    }
    delete stat;

    delete db;

    return 0;
}
