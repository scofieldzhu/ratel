#include "vec2.hpp"
#include "vec3.hpp"
#include "vec_proxy.hpp"

using namespace ratel;

void Test()
{
	Vec2<float> fv2;
	Vec3<float> fv3;
	using Pt2f = Vec2<float>;
	VecProxy<Pt2f> pt2f_vp;
	auto byte_vec = pt2f_vp.serializeToBytes();

	using Pt2i = Vec2<int>;
	VecProxy<Pt2i> pt2i_vp;

	VecProxy<int> vpi;
	vpi.loadBytes(byte_vec.data(), byte_vec.size());

}