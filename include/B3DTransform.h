#include "BGlobals.h"

namespace Bolts {
	struct B3DTransform {
		B3DTransform( vec3 p, quat r, vec3 s = vec3() ): mPosition( p ), mRotation( r ), mScale( s ) {};
		B3DTransform(): mPosition(), mRotation(), mScale() {};

		vec3 mPosition;
		vec3 mScale;
		quat mRotation;
	};
};