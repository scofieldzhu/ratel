/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: tcp_session.cpp 
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

#include "tcp_session.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include "spdlog/spdlog.h"

namespace baip = boost::asio::ip;
using IoContext = boost::asio::io_context;

RATEL_NAMESPACE_BEGIN

constexpr std::size_t kMaxBufferSize = 1024;

struct TcpSession::Impl 
{
    baip::tcp::socket socket;
    Byte read_buf[kMaxBufferSize] = {0};
    std::size_t read_bytes_size = 0;
    Byte write_buf[kMaxBufferSize] = {0};
    std::size_t write_bytes_size = 0;
    TcpSession* owner;
    
    Impl(TcpSession* ts, SCK_CTX ctx)
        :owner(ts),
        socket(*reinterpret_cast<IoContext*>(ctx))
    {
    }

    IoContext* getIoContext()
    {
        return reinterpret_cast<IoContext*>(&socket.get_executor().context());
    }

    void postNextRead()
    {
        auto self(owner->shared_from_this());
        socket.async_read_some(
            boost::asio::buffer(read_buf, kMaxBufferSize),
            boost::bind(&TcpSession::Impl::onReadFinished, this, self, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }

    void onReadFinished(TcpSessionPtr self, const boost::system::error_code& ec, std::size_t len)
    {
        read_bytes_size = len;
        if(!ec){ //no error
            self->rcv_signal.invoke(self.get(), read_buf, len);
            if(!getIoContext()->stopped()) //trigger next read if not stopped            
                postNextRead();            
        }else if(ec == boost::asio::error::eof) {
            // ther other party close connection!
            self->close_signal.invoke(self.get(), ec.value());
        }else{
            //other error occur
            auto msg = ec.message();
            spdlog::error("Error occur:{}", msg);
            self->err_signal.invoke(self.get(), msg);
        }
    }

    void postNextWrite()
    {
        auto self(owner->shared_from_this());
        boost::asio::async_write(socket, 
                                 boost::asio::buffer(write_buf, write_bytes_size),
                                 boost::bind(&TcpSession::Impl::onWriteFinished, this, self, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
        );
    }

    void onWriteFinished(TcpSessionPtr self, const boost::system::error_code& ec, std::size_t len)
    {
        if(!ec){
            self->sent_signal.invoke(self.get(), len);
        }else if(ec == boost::asio::error::eof){
            // ther other party close connection!
            self->close_signal.invoke(self.get(), ec.value());
        }else{
            //other error occur
            auto msg = ec.message();
            spdlog::error("Error occur:{}", msg);
            self->err_signal.invoke(self.get(), msg);
        }
    }
};

TcpSession::TcpSession(SCK_CTX ctx)
    : impl_(new Impl(this, ctx))
{
}

TcpSession::~TcpSession()
{
}

TcpSessionPtr TcpSession::Create(SCK_CTX ctx)
{
    if(ctx == nullptr)
        return nullptr;
    return TcpSessionPtr(new TcpSession(ctx));
}

void TcpSession::start()
{
    impl_->postNextRead();
}

int TcpSession::send(const Byte* data, std::size_t size)
{
    if(data == nullptr || size > kMaxBufferSize){
        spdlog::error("Empty data or too large size:{}", size);
        return 1;
    }
    memcpy(impl_->write_buf, data, size);
    impl_->write_bytes_size = size;
    impl_->postNextWrite();
    return 0;
}

size_t TcpSession::GetMaxSendRcvSize()
{
    return kMaxBufferSize;
}

SCK_HANDLE TcpSession::socket()
{
    return &impl_->socket;
}

std::tuple<const Byte*, std::size_t> TcpSession::getRcvBuffer()
{
    return std::make_tuple(impl_->read_buf, impl_->read_bytes_size);
}

// vtkMatrix* CalcTransform(glm::vec3 source_pt, 
//                          glm::vec source_rotation,
//                          glm::vec3 target_pt, 
//                          glm::vec target_rotation)
// {
//     auto s_pt1 = source_pt;
//     auto s_n_rotation = glm::normalize(source_rotation);
//     auto s_pt2 = glm::vec3(source_pt[0] + n_rotation[0], source_pt[1], source_pt[2]);
//     auto s_pt3 = glm::vec3(source_pt[0], source_pt[1] + n_rotation[1], source_pt[2]);
//     auto s_pt4 = glm::vec3(source_pt[0], source_pt[1], source_pt[2] + n_rotation[2]);
//     auto source_pts = vtkPoints::New();
//     source_pts->insertPoint(s_pt1);
//     source_pts->insertPoint(s_pt2);
//     source_pts->insertPoint(s_pt3);
//     source_pts->insertPoint(s_pt4);

//     auto t_pt1 = target_pt;
//     auto t_n_rotation = glm::normalize(target_rotation);
//     auto t_pt2 = glm::vec3(target_pt[0] + t_n_rotation[0], target_pt[1], target_pt[2]);
//     auto t_pt3 = glm::vec3(target_pt[0], target_pt[1] + t_n_rotation[1], target_pt[2]);
//     auto t_pt4 = glm::vec3(target_pt[0], target_pt[1], target_pt[2] + t_n_rotation[2]);
//     auto target_pts = vtkPoints::New();
//     target_pts->insertPoint(t_pt1);
//     target_pts->insertPoint(t_pt2);
//     target_pts->insertPoint(t_pt3);
//     target_pts->insertPoint(t_pt4);

//     auto lm_trans = vtkLandmarkTransform::New();
//     lm_trans->setTranformToRigidBody();
//     lm_trans->SetSourceLandmarks(source_pts);
//     lm_trans->SetTargetLandmarks(target_pts);
//     lm_trans->update();
//     return lm_trans->GetMatrix();
// }

RATEL_NAMESPACE_END
