/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: line.h  
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

#ifndef __line_hpp__
#define __line_hpp__

#include "ratel/geometry/point.h"
#include "ratel/geometry/proxy_combine.hpp"

RATEL_NAMESPACE_BEGIN

template <class PointType>
class Line 
{
public:
    ByteVec serializeToBytes()const
    {
        ProxyCombine<PointType, PointType> pc;
        pc.proxyA().mutableElement() = p1_;
        pc.proxyB().mutableElement() = p2_;
        return pc.serializeToBytes();
    }

    size_t loadBytes(ConsBytePtr buffer, size_t size)
    {
        ProxyCombine<PointType, PointType> pc;
        return pc.loadBytes(buffer, size);
    }

    void setP1(const PointType& pt){ p1_ = pt; }
    const auto& p1()const{ return p1_; }
    void setP2(const PointType& pt){ p2_ = p2; }
    const auto& p2()const{ return p2_; }
    
    Line& operator=(const Line& l)
    {
        if(this != &l){
            p1_ = l.p1_;
            p2_ = l.p2_;
        }
        return *this;
    }

    Line& operator=(Line&& l)
    {
        p1_ = std::move(l.p1_);
        p2_ = std::move(l.p2_);
        return *this;
    }

    Line(const PointType& p1, const PointType& p2)
        :p1_(p1),
        p2_(p2)
    {}
    Line(const Line& l)
        :p1_(l.p1_),
        p2_(l.p2_)
    {}
    Line(Line&& l)
        :p1_(std::move(l.p1_)),
        p2_(std::move(l.p2_))
    {}
    ~Line() = default;

private:
    PointType p1_;
    PointType p2_;
};

using Line2 = Line<Point2>;
using Line2f = Line<Point2f>;
using Line3 = Line<Point3>;
using Line3f = Line<Point3f>;

RATEL_NAMESPACE_END

#endif
