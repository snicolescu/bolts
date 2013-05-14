#include "Collada/Compilation.h"
#include "Core/types.h"
#include "Core/helpers.h"

/*
#include <rapidxml/rapidxml_iterators.hpp>

using namespace rapidxml;

void Bolts::Collada::compileDocument( rapidxml::xml_document<> &doc )
{

	//For now it only makes sure that the only vertex indices are in the <mesh><triangles>node
	xml_node<> *node1 = NULL, *node2 = NULL;
	node1 = doc.first_node( "library_geometries", 18, true);
	if ( node1 ){
		node2 = node1->first_node( "geometry", 8, true);
		if( node2 ){
			node1 = node2->first_node( "mesh", 4, true);
			if( node1 ){
				node2 = node1->first_node( "triangles", 9, true);
			}
		}
	}
}

bool nodeNameEquals(rapidxml::xml_node<>& node, const char* isName, size_t isNameSize){
	if ( node.name_size() != isNameSize)
		return false;

	for ( uint8_t it = 0; it < isNameSize; ++it)
	{
		if ( node.name()[it] != isName[it] )
			return false;
	}
	return true;
}

void Bolts::Collada::loadGPUBuffer( rapidxml::xml_node<>& mesh_node )
{
	BOLTS_ASSERT( nodeNameEquals(mesh_node, "mesh", 4));

	xml_node<> *sourceNode = mesh_node.first_node("source", 6, true);
	BOLTS_ASSERT(sourceNode);

	xml_node<> *accessorNode = sourceNode->first_node("technique_common", 16, true)->first_node("accessor", 8, true);
	BOLTS_ASSERT(accessorNode);
	accessorNode->first_attribute("count",5,true)->value();


	sourceNode = sourceNode->first_node("float_array", 11, true);
	BOLTS_ASSERT(sourceNode);


}
*/