/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.basic 
Module: directed_graph.h 
=========================================================================*/
#ifndef __directed_graph_h__
#define __directed_graph_h__

#include <map>
#include <vector>
#include <string>
#include <cstdint>
#include <type_traits>
#include "ratel_nsp.h"

RATEL_NAMESPACE_BEGIN

template <class VDT, class ADT, class VKT = std::string>
class DirectedGraph
{
public:
	using vertex_key_type = VKT;
	using vertex_key_reference = vertex_key_type&;
	using const_vertex_key_reference = const vertex_key_type&;
	using vertex_data_type = std::enable_if<std::is_default_constructible_v<VDT> &&
											std::is_copy_assignable_v<VDT>, VDT>::type;
	using arc_data_type = std::enable_if<std::is_default_constructible_v<ADT> &&
										 std::is_copy_assignable_v<ADT>, ADT>::type;
	struct ArcInfo
	{
		arc_data_type data;
		vertex_key_type source_vertex;
		vertex_key_type target_vertex;
	};
	using ArcInfoList = std::vector<ArcInfo>;

	void addVertex(const_vertex_key_reference key, const vertex_data_type& data);
	void removeVertex(const_vertex_key_reference key);
	bool existVertex(const_vertex_key_reference key)const;
	void addArc(const_vertex_key_reference source_vertex, const_vertex_key_reference target_vertex, const arc_data_type& data);
	void removeArc(const_vertex_key_reference source_vertex, const_vertex_key_reference target_vertex);
	bool existArc(const_vertex_key_reference source_vertex, const_vertex_key_reference target_vertex)const;
	void getVertexArcs(const_vertex_key_reference vertex_key, ArcInfoList& out_arcs, ArcInfoList& in_arcs)const;
	void clear();
	DirectedGraph();
	~DirectedGraph();

private:
	struct ArcNode;
	struct VertexNode;
	auto locateArcNode(const_vertex_key_reference source_vertex, const_vertex_key_reference target_vertex)const;
	void removeOutArcNode(VertexNode& node, const_vertex_key_reference target_vertex);
	void removeAllOutArcNodes(VertexNode& host);
	void removeInArcNode(VertexNode& node, const_vertex_key_reference source_vertex);

	struct ArcNode
	{
		arc_data_type data;
		vertex_key_type source_vertex;
		vertex_key_type target_vertex;
		ArcNode* next_out_arc = nullptr;
		ArcNode* next_in_arc = nullptr;
	};
	struct VertexNode
	{
		vertex_data_type data;
		ArcNode* head_in_arc = nullptr;
		ArcNode* head_out_arc = nullptr;
	};
	std::map<vertex_key_type, VertexNode> vertex_node_map_;
};

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::addVertex(const_vertex_key_reference key, const vertex_data_type& data)
{
	if(!existVertex(key))
		return;
	VertexNode vnode;
	vnode.data = data;
	vertex_node_map_[key] = std::move(vnode);
}

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::removeVertex(const_vertex_key_reference key)
{
	if(!existVertex(key))
		return;
	auto& host_vertex = vertex_node_map_[key];
	removeAllOutArcNodes(host_vertex);
	vertex_node_map_.erase(key);
}

template<class VDT, class ADT, class VKT>
inline bool DirectedGraph<VDT, ADT, VKT>::existVertex(const_vertex_key_reference key) const
{
	return vertex_node_map_.find(key) != vertex_node_map_.end();
}

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::addArc(const_vertex_key_reference source_vertex, const_vertex_key_reference target_vertex, const arc_data_type& data)
{
	if(existArc(source_vertex, target_vertex))
		return;
	ArcNode* anode = new ArcNode;
	anode->data = data;
	anode->source_vertex = source_vertex;
	anode->target_vertex = target_vertex;
	anode->next_out_arc = vertex_node_map_[source_vertex].head_out_arc;
	vertex_node_map_[source_vertex].head_out_arc = anode;
	anode->next_in_arc = vertex_node_map_[target_vertex].head_in_arc;
	vertex_node_map_[target_vertex].head_in_arc = anode;
}

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::removeArc(const_vertex_key_reference source_vertex, const_vertex_key_reference target_vertex)
{
	if(!existArc(source_vertex, target_vertex))
		return;
	ArcNode* target_arc_node = locateArcNode(source_vertex, target_vertex);
	removeOutArcNode(vertex_node_map_[source_vertex], target_vertex);
	removeInArcNode(vertex_node_map_[target_vertex], source_vertex);
	delete target_arc_node;
}

template<class VDT, class ADT, class VKT>
inline bool DirectedGraph<VDT, ADT, VKT>::existArc(const_vertex_key_reference source_vertex, const_vertex_key_reference target_vertex) const
{
	if(source_vertex.empty() ||
		target_vertex.empty() ||
		!existVertex(source_vertex) ||
		!existVertex(target_vertex)){
		return false;
	}
	return locateArcNode(source_vertex, target_vertex) != nullptr;
}

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::getVertexArcs(const_vertex_key_reference vertex_key, ArcInfoList& out_arcs, ArcInfoList& in_arcs) const
{
	if(!existVertex(vertex_key))
		return;
	const auto& vertex_node = vertex_node_map_.at(vertex_key);
	auto cur_arc = vertex_node.head_out_arc;
	while(cur_arc){
		ArcInfo info;
		info.data = cur_arc->data;
		info.source_vertex = cur_arc->source_vertex;
		info.target_vertex = cur_arc->target_vertex;
		out_arcs.push_back(std::move(info));
		cur_arc = cur_arc->next_out_arc;
	}
	cur_arc = vertex_node.head_in_arc;
	while(cur_arc){
		ArcInfo info;
		info.data = cur_arc->data;
		info.source_vertex = cur_arc->source_vertex;
		info.target_vertex = cur_arc->target_vertex;
		in_arcs.push_back(std::move(info));
		cur_arc = cur_arc->next_in_arc;
	}
}

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::clear()
{

}

template<class VDT, class ADT, class VKT>
inline DirectedGraph<VDT, ADT, VKT>::DirectedGraph()
{}

template<class VDT, class ADT, class VKT>
inline DirectedGraph<VDT, ADT, VKT>::~DirectedGraph()
{}

template<class VDT, class ADT, class VKT>
inline auto DirectedGraph<VDT, ADT, VKT>::locateArcNode(const_vertex_key_reference source_vertex, const_vertex_key_reference target_vertex) const
{
	auto cur_arc = vertex_node_map_.at(source_vertex).head_out_arc;
	while(cur_arc){
		if(cur_arc->target_vertex == target_vertex)
			return cur_arc;
		cur_arc = cur_arc->next_out_arc;
	}
	return (ArcNode*)nullptr;
}

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::removeOutArcNode(VertexNode& node, const_vertex_key_reference target_vertex)
{
	ArcNode* last_arc = node.head_out_arc;
	auto cur_arc = node.head_out_arc;
	while(cur_arc){
		if(cur_arc->target_vertex == target_vertex){
			if(cur_arc == node.head_out_arc)
				node.head_out_arc = cur_arc->next_out_arc;
			else
				last_arc->next_out_arc = cur_arc->next_out_arc;
			cur_arc->next_out_arc = nullptr;
			break;
		}
		last_arc = cur_arc;
		cur_arc = cur_arc->next_out_arc;
	}
}

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::removeAllOutArcNodes(VertexNode& host)
{
	while(host.head_out_arc)
		removeArc(host.head_out_arc->source_vertex, host.head_out_arc->target_vertex);
}

template<class VDT, class ADT, class VKT>
inline void DirectedGraph<VDT, ADT, VKT>::removeInArcNode(VertexNode& node, const_vertex_key_reference source_vertex)
{
	ArcNode* last_arc = node.head_in_arc;
	auto cur_arc = node.head_in_arc;
	while(cur_arc){
		if(cur_arc->source_vertex == source_vertex){
			if(cur_arc == node.head_in_arc)
				node.head_in_arc = cur_arc->next_in_arc;
			else
				last_arc->next_in_arc = cur_arc->next_in_arc;
			cur_arc->next_in_arc = nullptr;
			break;
		}
		last_arc = cur_arc;
		cur_arc = cur_arc->next_in_arc;
	}
}

RATEL_NAMESPACE_END

#endif