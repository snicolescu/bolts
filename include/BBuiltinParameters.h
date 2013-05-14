#pragma once

#include "BMath.h"
#include "BTypes.h"
#include "BShader.h"

namespace Bolts {
	namespace Rendering {

		struct BuiltinHolder {
			mat4 mMVPMatrix;
			mat4 mMVMatrix;
			mat4 mPMatrix;
			float mTime;
			float mDTime;
		};

	};
};