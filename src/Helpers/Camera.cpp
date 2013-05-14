//#include "stdafx.h"
#include "Helpers/Camera.h"
#include <boost/assert.hpp>
#include <glm/gtc/matrix_transform.hpp>

Bolts::Camera::Camera():m_viewDirty( true),m_projectionDirty( true), m_position(1.f), m_target(0.f),m_up( 0.f, 1.f, 0.f),
	m_fov( 50.f), m_aspectRatio( 1.f), m_zNear( 2.f), m_zFar( 15.f)
{

}

Bolts::mat4 Bolts::Camera::GetViewMatrix() const
{
	if( m_viewDirty)
		UpdateViewMatrix ();

	return m_viewMatrix;
}

void Bolts::Camera::UpdateViewMatrix() const
{
	vec3 vx,vy,vz;
	vz = glm::normalize(m_position - m_target);
	vx = glm::normalize( glm::cross( m_up, vz));
	vy = glm::cross( vz, vx);
	mat4 tempView;
	tempView[0] = vec4( vx, 0.f);
	tempView[1] = vec4( vy, 0.f);
	tempView[2] = vec4( vz, 0.f);
	tempView[3] = vec4( m_position, 1.f);

	m_viewMatrix = glm::inverse (tempView);
	m_viewDirty = false;
}

Bolts::mat4 Bolts::Camera::GetProjectionMatrix() const
{
	if( m_projectionDirty)
		UpdateProjMatrix ();

	return m_projMatrix;
}

void Bolts::Camera::UpdateProjMatrix() const
{
	m_projMatrix = glm::perspective( m_fov, m_aspectRatio, m_zNear, m_zFar);
	m_projectionDirty = false;
}
