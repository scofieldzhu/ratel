/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: main.cpp 
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

#include "test_logger.h"
#include "test_string_proxy.h"
#include "test_directed_graph.h"
#include "test_notifier.h"
#include "test_geometry.h"
#include "test_asio_timer.h"
#include "test_asio_tcp_client.h"
#include "test_asio_tcp_server.h"
#include "test_asio_udp.h"
#include "ratel/basic/dbg_tracker.h"

int main()
{
	TestCase_SPDLogger();
	_AUTO_FUNC_TRACK_
	//TestCase_StringProxy();
	//TestCase_DirectedGraph();
	//TestCase_Notifier();
	//TestCase_Geometry();
	//TestCase_Asio_Timer();
	//TestCase_Asio_Tcp_Client();
	//TestCase_Asio_Tcp_Server();
	TestCase_Asio_Udp();
	return 0;
}