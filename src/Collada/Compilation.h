#pragma once
#include <rapidxml/rapidxml.hpp>

namespace Bolts {
	namespace Collada {

		void loadGPUBuffer( rapidxml::xml_node<> &mesh_node );

		void compileDocument( rapidxml::xml_document<> &doc );

	};
};