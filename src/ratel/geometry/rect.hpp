/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.

Project: ratel.geometry
Module: rect.hpp
Create time: 2023/12/08 20:22:45
=========================================================================*/
#ifndef __rect_hpp__
#define __rect_hpp__

#include "vec2.hpp"

RATEL_NAMESPACE_BEGIN

template <typename T>
class Rect
{
public:
	using value_type = std::enable_if_t<std::is_same_v<T, int> || std::is_same_v<T, float>, T>;

	Rect()
	{}

	Rect()
	{}

};

RATEL_NAMESPACE_END

#endif