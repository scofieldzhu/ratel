/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: tcp_session.h  
 *  Copyright (c) 2023-2024 scofieldzhu
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

#ifndef __tcp_session_h__
#define __tcp_session_h__

#include <tuple>
#include "ratel/asio/asio_base_type.h"
#include "ratel/basic/notifier.hpp"

RATEL_NAMESPACE_BEGIN

class RATEL_ASIO_API TcpSession final : public std::enable_shared_from_this<TcpSession>
{
public:
    using ErrSignal = Notifier<TcpSession*, std::string>;
    ErrSignal err_signal;
    using SentSignal = Notifier<TcpSession*, std::size_t>;
    SentSignal sent_signal;
    using RcvSignal = Notifier<TcpSession*, ConsBytePtr, std::size_t>;
    RcvSignal rcv_signal;
    using CloseSignal = Notifier<TcpSession*, int>;
    CloseSignal close_signal;
    static TcpSessionPtr Create(SCK_CTX, bool asyn_mode);    
    int send(const Byte* data, std::size_t size);
    std::size_t syncSend(const Byte* data, std::size_t size, std::string* detail_err = nullptr);
    std::size_t syncRead(std::string* detail_err = nullptr);
    static std::size_t GetMaxSendRcvSize();
    std::tuple<const Byte*, std::size_t> getRcvBuffer();
    bool asynMode()const;
    ~TcpSession();

private:
    friend class TcpServer;
    friend class TcpClient;
    void start();
    SCK_HANDLE socket();
    TcpSession(SCK_CTX ctx, bool asyn_mode);
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

RATEL_NAMESPACE_END

#endif
