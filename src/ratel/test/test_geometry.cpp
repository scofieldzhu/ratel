/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: test_geometry.cpp 
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

#include "test_geometry.h"
#include "ratel/geometry/geometry.h"
#include "ratel/basic/dbg_tracker.h"
#include <iostream>
#include <iterator>

using namespace std;
using namespace ratel;

using Arry2f = ArrayX<float, 2>;
using Arry2i = ArrayX<int, 2>;
using Arry3f = ArrayX<float, 3>;
using Arry3i = ArrayX<int, 3>;
using Vec2f = VecProxy<Arry2f>;
using Vec2i = VecProxy<Arry2i>;
using Vec3f = VecProxy<Arry3f>;
using Vec3i = VecProxy<Arry3i>;
using VecString = VecProxy<StringProxy>;
using VecByte = VecProxy<char>;

void TestCase_ElementProxy()
{
    {
        ElementProxy<int> ep(234);
        auto ibv = ep.serializeToBytes();
        ElementProxy<int> ep_cpy;
        ep_cpy.loadBytes(ibv.data(), ibv.size());
        cout << "ep_cpy:" << ep_cpy.element() << endl;
    }
    {
        ElementProxy<StringProxy> ep("sfdfsdf");
        auto ibv = ep.serializeToBytes();
        ElementProxy<StringProxy> ep_cpy;
        ep_cpy.loadBytes(ibv.data(), ibv.size());
        cout << "string ep_cpy:" << ep_cpy.element().stdStr().c_str() << endl;

    }
}

template <typename T, size_t N>
void PrintArrayX(const ArrayX<T, N>& a)
{
    auto std_array = a.toArray();
    std::copy(std_array.begin(), std_array.end(), std::ostream_iterator<T>(cout, ", "));
}

void TestCase_ArrayX()
{
    cout << endl <<"Enter TestCase_ArrayX" << endl;
    Arry2f ay2f{2.3f, 3.4f};
    auto ay2f_cpy = ay2f;
    cout << endl << "ay2f_cpy elements:";
    PrintArrayX(ay2f_cpy);
    Arry3i ay3i{202, -1, 90};
    cout << endl << "ay3i elements:";
    PrintArrayX(ay3i);
    ByteVec x = ay3i.serializeToBytes();
    Arry3i ay3i_cpy;
    ay3i_cpy.loadBytes(x.data(), x.size());
    cout << endl << "ay3i_cpy elements:";
    PrintArrayX(ay3i_cpy);
    cout << endl <<"Leave TestCase_ArrayX" << endl;
}

void TestCase_VecProxy()
{
    auto arry2_str_f = [](const Arry2f& a2){
        return std::format("[{}, {}]", a2[0], a2[1]);
    };
    Vec2f v2f;
    v2f.mutableData().push_back({101.3f, 90.3f});
    v2f.mutableData().push_back({102.3f, 91.3f});
    v2f.mutableData().push_back({1.3f, 1.3f});
    cout << endl << "v2f elements:";
    for(const auto& l : v2f.data()){
        cout << arry2_str_f(l) << " ";
    }
    auto bv = v2f.serializeToBytes();
    Vec2f v2f_cpy;
    v2f_cpy.loadBytes(bv.data(), bv.size());
    cout << endl << "v2f_cpy elements:";
    for(const auto& l : v2f_cpy.data()){
        cout << arry2_str_f(l) << " ";
    }

    VecProxy<VecByte> vvbp;
    VecByte vb1;
    vb1.mutableData().resize(10);
    memcpy(vb1.mutableData().data(), "hello vb1vb1vb1", 10);
    vvbp.mutableData().push_back(std::move(vb1));
    VecByte vb2;
    vb2.mutableData().resize(11);
    memcpy(vb2.mutableData().data(), "hello vb2vb2vb2", 11);
    vvbp.mutableData().push_back(std::move(vb2));
    VecByte vb3;
    vb3.mutableData().resize(12);
    memcpy(vb3.mutableData().data(), "hello vb3vb3vb3", 12);
    vvbp.mutableData().push_back(std::move(vb3));
    auto sbytes = vvbp.serializeToBytes();
    VecProxy<VecByte> vvbp_cpy;
    vvbp_cpy.loadBytes(sbytes.data(), sbytes.size());
    cout << endl << "vvbp_cpy elements:";
    for(const auto& l : vvbp_cpy.data()){
        std::string p(l.data().data(), l.data().size());
        cout << p.c_str() << endl;
    }

}

void TestCase_DictProxy()
{
    {
        DictProxy<std::string, float> vs;
        vs.mutableData().insert({"AA", 2.3f});
        vs.mutableData().insert({"BB", 2.4f});
        vs.mutableData().insert({"CC", 2.5f});
        cout << endl << "vs elements:";
        for(const auto& l : vs.data()){
            cout << std::format("['{}' : {}]", l.first, l.second).c_str() << " ";
        }

        DictProxy<std::string, float> vs_cpy;
        auto d = vs.serializeToBytes();
        vs_cpy.loadBytes(d.data(), d.size());
        cout << endl << "vs_cpy elements:";
        for(const auto& l : vs_cpy.data()){
            cout << std::format("['{}' : {}]", l.first, l.second).c_str() << " ";
        }
    }
    {
        DictProxy<int, std::string> vs1;
        vs1.mutableData().insert({2, "AA"});
        vs1.mutableData().insert({3, "BBB"});
        vs1.mutableData().insert({4, "CCC"});
        cout << endl << "vs1 elements:";
        for(const auto& l : vs1.data()){
            cout << std::format("[{} : '{}']", l.first, l.second).c_str() << " ";
        }

        DictProxy<int, std::string> vs1_cpy;
        auto d = vs1.serializeToBytes();
        vs1_cpy.loadBytes(d.data(), d.size());
        cout << endl << "vs1_cpy elements:";
        for(const auto& l : vs1_cpy.data()){
            cout << std::format("[{} : '{}']", l.first, l.second).c_str() << " ";
        }
    }
}

void TestCase_ProxyCombine()
{
    Vec2f v2f;
    v2f.mutableData().push_back({101.3f, 90.3f});
    v2f.mutableData().push_back({102.3f, 91.3f});
    v2f.mutableData().push_back({1.3f, 1.3f});

    Vec2i v2i;
    v2i.mutableData().push_back({101, 90});
    v2i.mutableData().push_back({102, 91});
    v2i.mutableData().push_back({1, 1});

    ProxyCombine<Vec2f, Vec2i> pc(std::move(v2f), std::move(v2i));
    auto bv = pc.serializeToBytes();

    ProxyCombine<Vec2f, Vec2i> pc_cpy;
    pc_cpy.loadBytes(bv.data(), bv.size());
    Vec2f& v2f_1 = pc_cpy.proxyA();
    Vec2i& v2i_1 = pc_cpy.proxyB();

    auto arry2_str_f = [](const Arry2f& a2){
        return std::format("[{}, {}]", a2[0], a2[1]);
    };

    cout << endl << "v2f_1 elements:";
    for(const auto& l : v2f_1.data()){
        cout << arry2_str_f(l) << " ";
    }

    auto arry2_str_i = [](const Arry2i& a2){
        return std::format("[{}, {}]", a2[0], a2[1]);
    };

    cout << endl << "v2i_1 elements:";
    for(const auto& l : v2i_1.data()){
        cout << arry2_str_i(l) << " ";
    }
}

void TestCase_Geometry()
{
    _AUTO_FUNC_TRACK_
    TestCase_ElementProxy();
    TestCase_ArrayX();
    TestCase_VecProxy();
    TestCase_DictProxy();
    TestCase_ProxyCombine();
}
