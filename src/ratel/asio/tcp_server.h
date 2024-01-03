/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: tcp_server.h  
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

#ifndef __tcp_server_h__
#define __tcp_server_h__

#include "ratel/asio/asio_base_type.h"
#include "ratel/basic/notifier.hpp"

RATEL_NAMESPACE_BEGIN

class RATEL_ASIO_API TcpServer final
{
public:
    using ConnectSignal = Notifier<TcpSessionPtr>;
    ConnectSignal conn_signal;
    ASIO_CTX context();
    // void run();
    // void exit();
    bool asynMode()const;
    TcpServer(ASIO_CTX ctx, short port, bool async_mode);
    ~TcpServer();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

RATEL_NAMESPACE_END

#endif
