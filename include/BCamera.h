#pragma once

#include "BGlobals.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Bolts {
	class BCamera: public ref_counted_type {
		public :
			virtual mat4 getViewMatrix() = 0;

			virtual void moveUp( float_ units ) = 0;
			virtual void moveForward( float_ units ) = 0;
			virtual void moveRight( float_ units ) = 0;
	};

	class BOrbitCamera: public BCamera {
		public :
			BOrbitCamera(): mInverted( 1 ), mHAngle( 0 ), mVAngle( 0 ), mViewDistance( 3.0f ) {};

			mat4 getViewMatrix() {
				float_ vcos = glm::cos( mVAngle );
				return glm::lookAt(
				           vec3(	glm::sin( mHAngle ) * mViewDistance * vcos,
				                   glm::sin( mVAngle ) * mViewDistance,
				                   glm::cos( mHAngle ) * mViewDistance * vcos ),
				           mLookAt,
				           vec3( 0, 1, 0 ) );
			};

			void moveUp( float_ units ) {
				mVAngle += mInverted * units * M_PI_2;
				if ( mVAngle > M_PI_2 ) {
					mVAngle = M_PI_2;
				}
				if ( mVAngle < -M_PI_2 ) {
					mVAngle = -M_PI_2;
				}
			};

			void moveForward( float_ units ) {
				mViewDistance -= units;
			};

			void moveRight( float_ units ) {
				mHAngle += mInverted * units * M_PI;
				if ( mHAngle > M_PI * 2.0f ) {
					mHAngle -= M_PI * 2.0f;
				}
				if ( mHAngle < 0 ) {
					mHAngle -= M_PI * 2.0f;
				}
			};

			void invertAxes() {
				mInverted *= -1;
			}

			float_ mHAngle;
			float_ mVAngle;
			float_ mViewDistance;
			//The point at which the camera looks at
			vec3 mLookAt;
		private:
			byte_t mInverted;
	};
};