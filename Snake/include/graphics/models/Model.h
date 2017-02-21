#pragma once

#ifndef _MODEL_H
#define _MODEL_H

#include "graphics\structures\Mesh.h"

namespace gfx 
{
	class Model 
	{
	protected:
		glm::vec4 position;
		glm::mat4x4 worldMatrix;
		std::vector<Mesh*> meshes;

	public:
		Model();
		virtual ~Model();
		
		void RotateX(float alpha);
		void RotateY(float alpha);
		void RotateZ(float alpha);
		void Translate(const glm::vec3 & vec);
		void Translate(float x, float y, float z);

		const glm::vec4& Position() const { return position; }
		glm::mat4x4 WorldMatrix() const { return worldMatrix; }
		std::vector<Mesh*>::const_iterator MeshesBegin() const { return meshes.cbegin(); }
		std::vector<Mesh*>::const_iterator MeshesEnd() const { return meshes.cend(); }

		friend class SolidFactory;
	};
}

#endif // !_MODEL_H
