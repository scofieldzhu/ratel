/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: geometry.h  
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

#ifndef __geometry_h__
#define __geometry_h__

#include "ratel/geometry/element_proxy.hpp"
#include "ratel/geometry/array_x.hpp"
#include "ratel/geometry/vec_proxy.hpp"
#include "ratel/geometry/dict_proxy.hpp"
#include "ratel/geometry/proxy_combine.hpp"
#include "ratel/geometry/rect.hpp"
#include "ratel/geometry/geometry_export.h"

RATEL_NAMESPACE_BEGIN



// using Pt2i = Vec2<int32_t>;
// using Pt2u = Vec2<uint32_t>;
// using Pt2f = Vec2<float>;
// using VPPt2i = VecProxy<Pt2i>;
// using VPPt2u = VecProxy<Pt2u>;
// using VPPt2f = VecProxy<Pt2f>;

//  using Pt3i = Vec3<int32_t>;
// using Pt3u = Vec3<uint32_t>;
// using Pt3f = Vec3<float>;
// using VPPt3i = VecProxy<Pt3i>;
// using VPPt3u = VecProxy<Pt3u>;
// using VPPt3f = VecProxy<Pt3f>;

RATEL_NAMESPACE_END

RATEL_GEOMETRY_API void Test();

#endif