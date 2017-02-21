#include <glm\gtc\matrix_transform.hpp>
#include "graphics\Camera.h"

namespace gfx 
{
	glm::mat4x4 Camera::CreateLookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
	{
		glm::vec3 z3 = glm::normalize(position - target);
		glm::vec3 x3 = glm::normalize(glm::cross(up, z3));
		glm::vec3 y3 = glm::normalize(glm::cross(z3, x3));

		glm::vec4 x = glm::vec4(x3, 0.f);
		glm::vec4 y = glm::vec4(y3, 0.f);
		glm::vec4 z = glm::vec4(z3, 0.f);
		glm::vec4 w = glm::vec4(position, 1.f);

		return glm::inverse(glm::mat4x4(x, y, z, w));
	}

	glm::mat4x4 Camera::ViewMatrix() const
	{
		return Camera::CreateLookAt(glm::vec3(Position()), glm::vec3(Target()), glm::vec3(0, 1, 0));
	}

	glm::vec4 Camera::ViewVector(const Vertex& v) const
	{
		return glm::normalize(Position() - v.Point());
	}
	
	FixedCamera::FixedCamera(const glm::vec4& _position, const glm::vec4& _target)
		: position(_position), target(_target)
	{

	}

	glm::vec4 FixedCamera::Position() const
	{
		return position;
	}

	glm::vec4 FixedCamera::Target() const
	{
		return target;
	}

	InteractiveCamera::InteractiveCamera(const glm::vec4& _position, const glm::vec4& _trackedObject)
		: position(_position), trackedObject(_trackedObject)
	{

	}

	glm::vec4 InteractiveCamera::Position() const
	{
		return position;
	}

	glm::vec4 InteractiveCamera::Target() const
	{
		return trackedObject;
	}

	TrackingCamera::TrackingCamera(const glm::vec4& _trackedObject, const glm::vec4& _distance)
		: trackedObject(_trackedObject), distance(_distance)
	{

	}

	glm::vec4 TrackingCamera::Position() const
	{
		return trackedObject + distance;
	}

	glm::vec4 TrackingCamera::Target() const
	{
		return trackedObject;
	}
}