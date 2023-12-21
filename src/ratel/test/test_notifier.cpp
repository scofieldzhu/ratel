/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: test_notifier.cpp 
 *  Create Time: Tue Dec 19 2023  
 *  Copyright (c) 2023-2023 scofieldzhu
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

#include "test_notifier.h"
#include "ratel/basic/notifier.hpp"
#include "spdlog/spdlog.h"
using namespace ratel;

void TestF(int int_arg, float float_arg, std::string str_arg)
{
	spdlog::info("TestF trigger... int_arg:{} float_arg:{} str_arg:{}", int_arg, float_arg, str_arg);
}

void TestCase_Notifier()
{
	Notifier<int, float, std::string> nf;
	nf.bind(TestF);
	nf.invoke(10, 23.56f, "nima");
	int ix = 11;
	float fx = 12.45f;
	std::string sx = "ppx";
	nf.invokeR(ix, fx, sx);
}