#include "test_directed_graph.h"
#include <format>
#include "basic/directed_graph.hpp"
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
	display_dg_arcs_func();
	spdlog::info("===========================");
	spdlog::info("After remove arc:V1->V3, graphics arcs are following:");
	dg.removeArc("V1", "V3");
	display_dg_arcs_func();
	spdlog::info("===========================");
	spdlog::info("After remove vertex:V1, graphics arcs are following:");
	dg.removeVertex("V1");
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
