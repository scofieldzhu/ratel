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
	using point_type = Vec2<value_type>;
	using size_type = Vec2<value_type>;

	void setLT(const point_type& pt){ lt_ = pt; }
	const point_type& lt()const{ return lt_; }
	void setSize(const size_type& s){ size_ = s; }
	const auto& size()const{ return size_; }
	auto angle()const{return angle_;}

	Rect()
	{}

	~Rect() = default;

private:
	point_type lt_{value_type(0), value_type(0)};
	size_type size_{value_type(0), value_type(0)};
	float angle_ = 0.0f;
};

RATEL_NAMESPACE_END

#endif