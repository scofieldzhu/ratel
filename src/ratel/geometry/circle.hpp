/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: circle.h  
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
#ifndef __circle_hpp__
#define __circle_hpp__

#include "ratel/geometry/array_x.hpp"
#include "ratel/geometry/element_proxy.hpp"
#include "ratel/geometry/proxy_combine.hpp"

RATEL_NAMESPACE_BEGIN

template <typename T>
class Circle final
{
public:
    using value_type = std::enable_if_t<std::is_same_v<T, int> || std::is_same_v<T, float>, T>;
    using point_type = ArrayX<value_type, 2>;
    using float_proxy = ElementProxy<float>;

    ByteVec serializeToBytes()const
    {
        ProxyCombine<point_type, float_proxy> pc;
        pc.proxyA().mutableElement() = center_;
        pc.proxyB().mutableElement() = radius_;
        return pc.serializeToBytes();
    }

    size_t loadBytes(ConsBytePtr buffer, size_t size)
    {
        ProxyCombine<point_type, float_proxy> pc;
        return pc.loadBytes(buffer, size);
    }

    void setCenter(value_type x, value_type y){ center_ = point_type{x, y}; }
    void setCenter(const point_type& pt){ center_ = pt; }
    const point_type& center()const{ return center_; }
    void setRadius(float r){ radius_ = r; }
    auto radius()const{ return radius_; }

    Circle& operator=(const Circle& other)
    {
        if(this != &other){
            center_ = other.center_;
            radius_ = other.radius_;
        }
        return *this;
    }

    Circle& operator=(const Circle&& other)
    {
        center_ = std::move(other.center_);
        radius_ = std::move(other.radius_);
        return *this;
    }

    Circle(const point_type& pt, float r)
        :center_(pt),
        radius_(r){}

    Circle(const Circle& other)
        :center_(other.center_),
        radius_(other.radius_){}

    Circle(Circle&& other)
        :center_(std::move(other.center_)),
        radius_(std::move(radius_)){}
        
    ~Circle() = default;

private:
    point_type center_{value_type(0), value_type(0)};
    float radius_ = 0.0f;
};

RATEL_NAMESPACE_END

#endif
