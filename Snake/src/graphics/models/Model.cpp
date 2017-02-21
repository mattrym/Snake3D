#include <glm\gtc\matrix_transform.hpp>
#include "graphics\models\Model.h"

namespace gfx 
{
	Model::Model() 
	{
		worldMatrix = glm::mat4(1.0f);
		position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Model::~Model() 
	{
		std::vector<Mesh*>::iterator mesh_it;
		for (mesh_it = meshes.begin(); mesh_it != meshes.end(); ++mesh_it) 
		{
			delete *mesh_it;
		}
	}

	void Model::RotateX(float alpha)
	{
		glm::mat4 transformMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(1, 0, 0));
		worldMatrix = transformMatrix * worldMatrix;
	}

	void Model::RotateY(float alpha) 
	{
		glm::mat4 transformMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(0, 1, 0));
		worldMatrix = transformMatrix * worldMatrix;
	}

	void Model::RotateZ(float alpha) 
	{
		glm::mat4 transformMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(alpha), glm::vec3(0, 0, 1));
		worldMatrix = transformMatrix * worldMatrix;
	}

	void Model::Translate(const glm::vec3& vec) 
	{
		glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), vec);
		position = transformMatrix * position;
		worldMatrix = transformMatrix * worldMatrix;
	}

	void Model::Translate(float x, float y, float z) 
	{
		glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		position = transformMatrix * position;
		worldMatrix = transformMatrix * worldMatrix;
	}
}
