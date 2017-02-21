#define _USE_MATH_DEFINES

#include <cmath>
#include <ctime>
#include <sstream>
#include <fstream>
#include <glm\gtc\random.hpp>
#include "graphics\models\SolidFactory.h"

namespace gfx 
{
	SolidFactory& SolidFactory::Instance()
	{
		static SolidFactory factory;
		return factory;
	}

	Model* SolidFactory::CreateSurface(float edge, int width, int height, const MaterialInfo& mInfo)
	{
		Model* model = new Model();
		meshBuilder.BuildMesh(mInfo);

		glm::vec4 normal = glm::vec4(0, 1, 0, 0);
		for (int i = 0; i < width; ++i) 
		{
			for (int j = 0; j < height; ++j)
			{
				Vertex vertices[4]
				{
					Vertex(glm::vec4(edge * i, 0, edge * j, 1), normal),
					Vertex(glm::vec4(edge * (i + 1), 0, edge * j, 1), normal),
					Vertex(glm::vec4(edge * i, 0, edge * (j + 1), 1), normal), 
					Vertex(glm::vec4(edge * (i + 1), 0, edge * (j + 1), 1), normal)
				};
				meshBuilder.BuildTriangle(vertices[2], vertices[1], vertices[0]);
				meshBuilder.BuildTriangle(vertices[2], vertices[3], vertices[1]);
				meshBuilder.BuildTriangle(vertices[2], vertices[0], vertices[1]);
				meshBuilder.BuildTriangle(vertices[2], vertices[1], vertices[3]);

			}
		}

		model->meshes.push_back(meshBuilder.GetMesh());
		return model;
	}


	Model* SolidFactory::CreateCuboid(float w, float h, float d, const MaterialInfo& mInfo)
	{
		glm::vec4 normal;
		Vertex* vertices;

		Model* model = new Model();
		meshBuilder.BuildMesh(mInfo);

		// back wall
		
		normal = glm::vec4(0, 0, -1, 0);
		vertices = new Vertex[4]{
			Vertex(glm::vec4(0, 0, 0, 1), normal), Vertex(glm::vec4(0, h, 0, 1), normal),
			Vertex(glm::vec4(w, h, 0, 1), normal), Vertex(glm::vec4(w, 0, 0, 1), normal)
		};
		meshBuilder.BuildTriangle(vertices[0], vertices[1], vertices[2]);
		meshBuilder.BuildTriangle(vertices[0], vertices[2], vertices[3]);
		delete[] vertices;

		// front wall

		normal = glm::vec4(0, 0, 1, 0);
		vertices = new Vertex[4]{
			Vertex(glm::vec4(0, 0, d, 1), normal), Vertex(glm::vec4(w, 0, d, 1), normal),
			Vertex(glm::vec4(w, h, d, 1), normal), Vertex(glm::vec4(0, h, d, 1), normal)
		};
		meshBuilder.BuildTriangle(vertices[0], vertices[1], vertices[2]);
		meshBuilder.BuildTriangle(vertices[0], vertices[2], vertices[3]);
		delete[] vertices;

		// left wall

		normal = glm::vec4(-1, 0, 0, 0);
		vertices = new Vertex[4]{
			Vertex(glm::vec4(0, 0, 0, 1), normal), Vertex(glm::vec4(0, 0, d, 1), normal),
			Vertex(glm::vec4(0, h, d, 1), normal), Vertex(glm::vec4(0, h, 0, 1), normal)
		};
		meshBuilder.BuildTriangle(vertices[0], vertices[1], vertices[2]);
		meshBuilder.BuildTriangle(vertices[0], vertices[2], vertices[3]);
		delete[] vertices;

		// right wall

		normal = glm::vec4(1, 0, 0, 0);
		vertices = new Vertex[4]{
			Vertex(glm::vec4(w, 0, 0, 1), normal), Vertex(glm::vec4(w, h, 0, 1), normal),
			Vertex(glm::vec4(w, h, d, 1), normal), Vertex(glm::vec4(w, 0, d, 1), normal)
		};
		meshBuilder.BuildTriangle(vertices[0], vertices[1], vertices[2]);
		meshBuilder.BuildTriangle(vertices[0], vertices[2], vertices[3]);
		delete[] vertices;

		// bottom wall

		normal = glm::vec4(0, -1, 0, 0);
		vertices = new Vertex[4]{
			Vertex(glm::vec4(0, 0, 0, 1), normal), Vertex(glm::vec4(w, 0, 0, 1), normal),
			Vertex(glm::vec4(w, 0, d, 1), normal), Vertex(glm::vec4(0, 0, d, 1), normal)
		};
		meshBuilder.BuildTriangle(vertices[0], vertices[1], vertices[2]);
		meshBuilder.BuildTriangle(vertices[0], vertices[2], vertices[3]);
		delete[] vertices;

		// top wall

		normal = glm::vec4(0, 1, 0, 0);
		vertices = new Vertex[4]{
			Vertex(glm::vec4(0, h, 0, 1), normal), Vertex(glm::vec4(0, h, d, 1), normal),
			Vertex(glm::vec4(w, h, d, 1), normal), Vertex(glm::vec4(w, h, 0, 1), normal)
		};
		meshBuilder.BuildTriangle(vertices[0], vertices[1], vertices[2]);
		meshBuilder.BuildTriangle(vertices[0], vertices[2], vertices[3]);
		delete[] vertices;

		model->meshes.push_back(meshBuilder.GetMesh());
		return model;
	}

	Model* SolidFactory::CreateCylinder(float r, float h, int n, const MaterialInfo& mInfo)
	{
		Vertex** vertices = new Vertex*[4*n + 2];
		Model* model = new Model();
		meshBuilder.BuildMesh(mInfo);

		// top base

		glm::vec4 normal(0, 1, 0, 0);
		vertices[0] = new Vertex(glm::vec4(0, h, 0, 1), normal);
		for (int i = 1; i <= n; ++i) {
			vertices[i] = new Vertex(glm::vec4(r*std::cos(2 * M_PI * (i - 1) / n), h, r*std::sin(2 * M_PI * (i - 1) / n), 1), normal);
		}
		for (int i = 0; i < n - 1; ++i) {
			meshBuilder.BuildTriangle(*vertices[0], *vertices[i + 2], *vertices[i + 1]);
		}
		meshBuilder.BuildTriangle(*vertices[0], *vertices[1], *vertices[n]);

		// bottom base

		normal = glm::vec4(0, -1, 0, 0);
		vertices[4*n + 1] = new Vertex(glm::vec4(0, 0, 0, 1), normal);
		for (int i = 3*n + 1; i < 4*n + 1; ++i) {
			vertices[i] = new Vertex(glm::vec4(r*std::cos(2 * M_PI * (i - 1) / n), 0, r*std::sin(2 * M_PI * (i - 1) / n), 1), normal);
		}
		for (int i = 3*n; i < 4*n - 1; ++i) {
			meshBuilder.BuildTriangle(*vertices[4*n + 1], *vertices[i + 1], *vertices[i + 2]);
		}
		meshBuilder.BuildTriangle(*vertices[4 * n + 1], *vertices[4*n], *vertices[3 * n + 1]);

		// vertical faces

		for (int i = n + 1; i < 2 * n + 1; ++i) {
			vertices[i] = new Vertex(vertices[i - n]->Point(), glm::vec4(vertices[i - n]->Point()[0] / r, 0, vertices[i - n]->Point()[2] / r, 0));
			vertices[i + n] = new Vertex(vertices[i + 2*n]->Point(), glm::vec4(vertices[i + 2*n]->Point()[0] / r, 0, vertices[i + 2*n]->Point()[2] / r, 0));
		}

		for (int i = n; i < 2 * n - 1; ++i) {
			meshBuilder.BuildTriangle(*vertices[i + 1], *vertices[i + 2], *vertices[i + 1 + n]);
			meshBuilder.BuildTriangle(*vertices[i + n + 1], *vertices[i + 2], *vertices[i + n + 2]);
		}
		meshBuilder.BuildTriangle(*vertices[2 * n], *vertices[n + 1], *vertices[3 * n]);
		meshBuilder.BuildTriangle(*vertices[3 * n], *vertices[n + 1], *vertices[2 * n + 1]);

		for (int i = 0; i < 4*n + 2; ++i) {
			delete vertices[i];
			vertices[i] = nullptr;
		}
		delete[] vertices;

		model->meshes.push_back(meshBuilder.GetMesh());
		return model;
	}

	Model* SolidFactory::CreateSphere(float r, int m, int n, const MaterialInfo& mInfo)
	{
		Vertex** vertices = new Vertex*[m * n + 2];
		Model* model = new Model();
		meshBuilder.BuildMesh(mInfo);

		vertices[0] = new Vertex(glm::vec4(0, r, 0, 1), glm::vec4(0, 1, 0, 0));
		for (int i = 1; i < m + 1; ++i) {
			for (int j = 1; j < n + 1; ++j) {
				glm::vec4 point(
					r * std::cos(2 * M_PI*(j - 1) / n) * std::sin(M_PI * i / (m + 1)),
					r *	std::cos(M_PI * i / (m + 1)),
					r * std::sin(2 * M_PI*(j - 1) / n) * std::sin(M_PI * i / (m + 1)),
					1);
				glm::vec4 normal(point[0] / r, point[1] / r, point[2] / r, 0);
				vertices[(i - 1)*n + j] = new Vertex(point, normal);
			}
		}
		vertices[m*n+1] = new Vertex(glm::vec4(0, -r, 0, 1), glm::vec4(0, -1, 0, 0));

		// top caps

		for (int i = 0; i < n - 1; ++i) {
			meshBuilder.BuildTriangle(*vertices[0], *vertices[i + 2], *vertices[i + 1]);
			meshBuilder.BuildTriangle(*vertices[m*n + 1], *vertices[(m - 1)*n + i + 1], *vertices[(m - 1)*n + i + 2]);
		}
		meshBuilder.BuildTriangle(*vertices[0], *vertices[1], *vertices[n]);
		meshBuilder.BuildTriangle(*vertices[m*n + 1], *vertices[m*n], *vertices[(m - 1)*n + 1]);

		// parallels

		for (int i = 0; i < m - 1; ++i) {
			for (int j = 1; j < n; ++j) {
				meshBuilder.BuildTriangle(*vertices[i*n + j], *vertices[i*n + j + 1], *vertices[(i + 1)*n + j + 1]);
				meshBuilder.BuildTriangle(*vertices[i*n + j], *vertices[(i + 1)*n + j + 1], *vertices[(i + 1)*n + j]);
			}
			meshBuilder.BuildTriangle(*vertices[(i + 1)*n], *vertices[i*n + 1], *vertices[(i + 1)*n + 1]);
			meshBuilder.BuildTriangle(*vertices[(i + 1)*n], *vertices[(i + 1)*n + 1], *vertices[(i + 2)*n]);
		}

		for (int i = 0; i < 4 * n + 2; ++i) {
			delete vertices[i];
			vertices[i] = nullptr;
		}
		delete[] vertices;

		model->meshes.push_back(meshBuilder.GetMesh());
		return model;
	}
}