/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.geometry
Module: geometry.h
Create time: 2023/12/08 20:05:45
=========================================================================*/
#ifndef __geometry_h__
#define __geometry_h__

#include "ratel/geometry/vec2.hpp"
#include "ratel/geometry/vec3.hpp"
#include "ratel/geometry/vec_proxy.hpp"

RATEL_NAMESPACE_BEGIN

using Pt2i = Vec2<int32_t>;
using Pt2u = Vec2<uint32_t>;
using Pt2f = Vec2<float>;
using VPPt2i = VecProxy<Pt2i>;
using VPPt2u = VecProxy<Pt2u>;
using VPPt2f = VecProxy<Pt2f>;

using Pt3i = Vec3<int32_t>;
using Pt3u = Vec3<uint32_t>;
using Pt3f = Vec3<float>;
using VPPt3i = VecProxy<Pt3i>;
using VPPt3u = VecProxy<Pt3u>;
using VPPt3f = VecProxy<Pt3f>;

RATEL_NAMESPACE_END

#endif