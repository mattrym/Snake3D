#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include "graphics\structures\Vertex.h"

namespace gfx 
{
	class Camera 
	{
	protected:
		static glm::mat4x4 CreateLookAt(const glm::vec3& _position, const glm::vec3& _target, const glm::vec3& _up);

	public:
		Camera() { }
		virtual ~Camera() { }

		virtual glm::vec4 Position() const = 0;
		virtual glm::vec4 Target() const = 0;
		glm::mat4x4 ViewMatrix() const;
		glm::vec4 ViewVector(const Vertex& v) const;
	};

	class FixedCamera : public Camera
	{
	private:
		const glm::vec4 position;
		const glm::vec4 target;
	public:
		FixedCamera(const glm::vec4& _position, const glm::vec4& _target);
		virtual ~FixedCamera() { }

		virtual glm::vec4 Position() const;
		virtual glm::vec4 Target() const;
	};

	class InteractiveCamera : public Camera
	{
	private:
		const glm::vec4 position;
		const glm::vec4& trackedObject;

	public:
		InteractiveCamera(const glm::vec4& _position, const glm::vec4& _trackedObject);
		virtual ~InteractiveCamera() { }
		
		virtual glm::vec4 Position() const;
		virtual glm::vec4 Target() const;
	};

	class TrackingCamera : public Camera
	{
	private:
		const glm::vec4& trackedObject;
		const glm::vec4& distance;
		
	public:
		TrackingCamera(const glm::vec4& _trackedObject, const glm::vec4& _distance);
		virtual ~TrackingCamera() { }
		
		virtual glm::vec4 Position() const;
		virtual glm::vec4 Target() const;
	};
}

#endif // !_CAMERA_H
