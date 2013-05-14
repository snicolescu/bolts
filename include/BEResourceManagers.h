#pragma once

#include "BGlobals.h"
#include "BEManager.h"
#include "BEMesh.h"
#include "BParticleSystem.h"
#include <string>

namespace Bolts {
	enum BEResourceError { AOK = 0, RES_NOT_FOUND, OUT_OF_MEMORY, UNKNOWN_ERROR};

	class BEMeshManager: public BEManager<BEMesh> {
	};

	class BPSManager: public BEManager<BParticleSystem> {
	};
};