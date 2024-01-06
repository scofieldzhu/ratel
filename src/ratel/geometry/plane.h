/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: plane.h  
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

#ifndef __plane_h__
#define __plane_h__

#include "ratel/geometry/array_x.hpp"
#include "ratel/geometry/geometry_export.h"
#include "ratel/geometry/proxy_combine.hpp"

RATEL_NAMESPACE_BEGIN

class RATEL_GEOMETRY_API Plane final
{
public:
    using normal_type = ArrayX<float, 3>;
    using normal_const_reference = const normal_type&;
    using point_type = ArrayX<float, 3>;
    using point_const_reference = const point_type&;
    ByteVec serializeToBytes()const;
    size_t loadBytes(ConsBytePtr buffer, size_t size);
    void setNormal(normal_const_reference n){ normal_ = n; }
    normal_const_reference normal()const{ return normal_; }
    void setOrigin(point_const_reference o){ origin_ = o; }
    point_const_reference origin()const{ return origin_; }
    Plane(point_const_reference o, normal_const_reference n);

private:
    point_type origin_;
    normal_type normal_;
};

RATEL_NAMESPACE_END

#endif