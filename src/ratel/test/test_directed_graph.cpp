/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: test_directed_graph.cpp 
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

#include "test_directed_graph.h"
#include <format>
#include "ratel/basic/directed_graph.hpp"
#include "spdlog/spdlog.h"

using namespace ratel;

using DG = DirectedGraph<int, int>;

void PrintVertexArcs(const DG& dg, const std::string& vertex_name);

void TestCase_DirectedGraph()
{
	DG dg;
	dg.addVertex("V1", 0);
	dg.addVertex("V2", 0);
	dg.addVertex("V3", 0);
	dg.addVertex("V4", 0);
	dg.addArc("V1", "V2", 0);
	dg.addArc("V1", "V3", 0);
	dg.addArc("V3", "V4", 0);
	dg.addArc("V3", "V1", 0);
	dg.addArc("V4", "V2", 0);
	dg.addArc("V4", "V1", 0);
	dg.addArc("V4", "V3", 0);	
	auto display_dg_arcs_func = [&dg](){
		PrintVertexArcs(dg, "V1");
		PrintVertexArcs(dg, "V2");
		PrintVertexArcs(dg, "V3");
		PrintVertexArcs(dg, "V4");
	};
	spdlog::info("===========================");
	spdlog::info("DG is created, exists cycle:{}", dg.checkCycle());
	display_dg_arcs_func();
	
	spdlog::info("===========================");
	dg.removeArc("V1", "V3");
	spdlog::info("After remove arc:V1->V3, graphics arcs are following: exists cycle:{}", dg.checkCycle());
	display_dg_arcs_func();

	spdlog::info("===========================");
	dg.removeVertex("V1");
	spdlog::info("After remove vertex:V1, graphics arcs are following:exists cycle:{}", dg.checkCycle());
	display_dg_arcs_func();

	spdlog::info("===========================");
	dg.removeArc("V3", "V4");
	spdlog::info("After remove arc:V3->V4, graphics arcs are following:exists cycle:{}", dg.checkCycle());
	display_dg_arcs_func();

	spdlog::info("===========================");
	spdlog::info("After clear. graphics arcs are following:");
	dg.clear();
	display_dg_arcs_func();
}

std::string ArcInfoToString(int idx, const DG::ArcInfo& info)
{
	return std::format("The {}th arc info: data:{} src_vert:{} dst_vert:{}", idx, info.data, info.source_vertex, info.target_vertex);
}

void PrintVertexArcs(const DG& dg, const std::string& vertex_name)
{
	DG::ArcInfoList out_arc_infos, in_arc_infos;
	dg.getVertexArcs(vertex_name, out_arc_infos, in_arc_infos);
	spdlog::info("------------------");
	spdlog::info("Vertex:{} out arcs:", vertex_name);
	int id = 0;
	for(const auto& arc_inf : out_arc_infos){
		std::string arc_info_str = ArcInfoToString(id++, arc_inf);
		spdlog::info(arc_info_str);
	}
	spdlog::info("Vertex:{} in arcs:", vertex_name);
	id = 0;
	for(const auto& arc_inf : in_arc_infos){
		std::string arc_info_str = ArcInfoToString(id++, arc_inf);
		spdlog::info(arc_info_str);
	}	
}
