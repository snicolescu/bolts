#ifndef Camera_h__
#define Camera_h__

#include "Core/math_types.h"

namespace Bolts {


	/*
	 *	Basic, generic Perspective Cam
	 */
	class Camera {
		public:
			Camera();

			void	SetPosition( const vec3 &newPos ) {
				m_position = newPos;
				m_viewDirty = true;
			}
			void	SetTarget( const vec3 &newTgt ) {
				m_target = newTgt;
				m_viewDirty = true;
			}
			void	SetUpVector( const vec3 &newUp ) {
				m_up = newUp;
				m_viewDirty = true;
			}

			void	SetFov( const float &fovDegrees ) {
				m_fov = fovDegrees;
				m_projectionDirty = true;
			}
			void	SetZNear( const float &znear ) {
				m_zNear = znear;
				m_projectionDirty = true;
			}
			void	SetZFar( const float &zfar ) {
				m_zFar = zfar;
				m_projectionDirty = true;
			}
			void	SetAspectRatio( const float &aspRatio ) {
				m_aspectRatio = aspRatio;
				m_projectionDirty = true;
			}

			mat4	GetViewMatrix() const;
			mat4	GetProjectionMatrix() const;
		private:
			void	UpdateViewMatrix() const;
			void	UpdateProjMatrix() const;

			mutable mat4	m_viewMatrix;
			mutable mat4	m_projMatrix;
			//View-related
			vec3			m_position;
			vec3			m_target;
			vec3			m_up;
			//Projection- related
			float			m_zNear;
			float			m_zFar;
			float			m_fov;
			float			m_aspectRatio;

			mutable bool	m_viewDirty;
			mutable bool	m_projectionDirty;
	};
};
#endif // Camera_h__