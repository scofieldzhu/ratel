#include "test_dll_loader.h"
/*
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *
 *  File: test_dll_loader.cpp
 *  Copyright (c) 2024-2024 scofieldzhu
 *
 *  MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */
#include "test_dll_loader.h"
#include "ratel/system/dynamic_library_loader.h"
#include "ratel/basic/dbg_tracker.h"
#include "spdlog/spdlog.h"

using namespace ratel;

void TestCase_Dll_Loader()
{
    _AUTO_FUNC_TRACK_
    DynamicLibraryLoader loader;
    if(!loader.load("Dll1.dll")){
        spdlog::error("Load Dll1.dll failed!");
        return;
    }
    using Func1Ptr = int(*)(int,int);
    Func1Ptr p = loader.getFunction<Func1Ptr>("Add");
    if(p == nullptr){
        spdlog::error("getFunction Add failed!");
        return;
    }
    int c = p(10, 23);
    spdlog::trace("c:{}", c);
    using Func2Ptr = float(*)(float,float);
    Func2Ptr p2 = loader.getFunction<Func2Ptr>("Subtract");
    if(p2 == nullptr){
        spdlog::error("getFunction Subtract failed!");
        return;
    }
    float x = p2(2.3, 3.5);
    spdlog::trace("x:{}", x);
}
