/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: testpackage 
Module: testdirtree.cpp 
CreateTime: 2018-9-26 21:09
=========================================================================*/
#include "testdirtree.h"
#include <iostream>
#include "dirtree.h"
#include "dirnode.h"
#include "path.h"
using namespace std;
USING_RATEL

void TestCase_PrintTree(const DirNode& rootnode, int32& layer)
{
    cout << "******Layer" << layer << "******" << endl;
    const DirNode* nextlayer_child = nullptr;
    for (const DirNode* childnode = &rootnode; childnode != nullptr; childnode = childnode->nextsibling) {
        cout << "Dir:" << childnode->name.cstr() << " ";
        for (const FileNode& fn : childnode->allfiles)
            cout << "File:" << fn.filename.cstr() << " ";
        cout << endl;
        if (nextlayer_child == nullptr && childnode->nextchild)
            nextlayer_child = childnode->nextchild;
    }
    if (nextlayer_child)
        TestCase_PrintTree(*nextlayer_child, ++layer);
}

//*/ratel/[dir1 dir2 dir3/[dir31] dir4]
void TestCase_CreateDir(DirTree& tree)
{
    tree.createDir("ratel", ROOTDIR_NAME);
    tree.createDir("dir1", "*/ratel");
    tree.createDir("dir2", "*/ratel");
    tree.createDir("dir3", "*/ratel");
    tree.createDir("dir31", "*/ratel/dir3");
    tree.createDir("dir4", "*/ratel");

    cout << "________________________after TestCase_CreateDir:" << endl;
    int32 layer = 0;
    TestCase_PrintTree(*tree.rootNode(), layer);
}

//*/ratel/{dir1/[file10.ttf file11.txt] dir2 dir3/(dir31 file32.html) dir4(file41.py) file5.cpp}
void TestCase_CreateFile(DirTree& tree)
{
    tree.createFile("*/ratel/file5.cpp");
    tree.createFile("*/ratel/dir1/file10.ttf");
    tree.createFile("*/ratel/dir1/file11.txt");
    tree.createFile("*/ratel/dir3/file32.html");
    tree.createFile("*/ratel/dir4/file41.py");

    cout << "________________________after TestCase_CreateFile:" << endl;
    int32 layer = 0;
    TestCase_PrintTree(*tree.rootNode(), layer);
}

//*/ratel/{dir1/[file10_n.ttf file11.txt] dir2 dir3/(dir31 file32_x.html) dir4(file41.py) file5.cpp}
void TestCase_RenameFile(DirTree& tree)
{
    tree.renameFile("*/ratel/dir1/file10.ttf", "file10_n.ttf");
    tree.renameFile("*/ratel/dir3/file32.html", "file32_x.html");

    cout << "________________________after TestCase_RenameFile:" << endl;
    int32 layer = 0;
    TestCase_PrintTree(*tree.rootNode(), layer);
}

//*/ratel/{newdir1/[file10_n.ttf file11.txt] dir2 newdir3/(newdir31 file32_x.html) dir4(file41.py) file5.cpp}
void TestCase_RenameDir(DirTree& tree)
{
    tree.renameDir("*/ratel/dir1", "newdir1");
    tree.renameDir("*/ratel/dir1", "newdir1");
    tree.renameDir("*/ratel/dir3", "newdir3");
    tree.renameDir("*/ratel/dir3", "newdir3");
    tree.renameDir("*/ratel/newdir3/dir31", "dir31");
    tree.renameDir("*/ratel/newdir3/dir31", "newdir31");
    cout << "________________________after TestCase_RenameDir:" << endl;
    int32 layer = 0;
    TestCase_PrintTree(*tree.rootNode(), layer);
}

//*/ratel/{newdir1/[file10_n.ttf file11.txt] dir2 dir4(file41.py) file5.cpp}
void TestCase_DeleteDir(DirTree& tree)
{
    tree.deleteDir("*/ratel/newdir3");
    cout << "________________________after TestCase_DeleteDir:" << endl;
    int32 layer = 0;
    TestCase_PrintTree(*tree.rootNode(), layer);
}

void TestCase_TestTreeOp()
{
    DirTree tree;
    TestCase_CreateDir(tree);
    TestCase_CreateFile(tree);
    TestCase_RenameFile(tree);
    TestCase_RenameDir(tree);
    TestCase_DeleteDir(tree);
}