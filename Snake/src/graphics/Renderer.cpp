#define _USE_MATH_DEFINES
#include <cmath>
#include "graphics\Renderer.h"

namespace gfx 
{
	glm::mat4x4 Renderer::CreatePerspectiveFieldOfView(float theta, float a, float n, float f)
	{
		float e = 1 / std::tanf(theta * (float)M_PI / 360);
		return glm::mat4x4
		(
			e,						0,						0,						0,
			0,						e / a,					0,						0,
			0,						0,						-(f + n) / (f - n),		-1,
			0,						0,						-2 * f * n / (f - n),	0
		);
	}

	Renderer::Renderer(SDL_Renderer* _sdlrenderer, int screenWidth, int screenHeight) 
		: sdlrenderer(_sdlrenderer), SCREEN_WIDTH(screenWidth), SCREEN_HEIGHT(screenHeight)
	{
		reflector = nullptr;
		shader = nullptr;
		camera = nullptr;
		
		float fieldOfView = 45.0f;
		float aspectRadio = screenHeight / (float)screenWidth;
		float near = 1, far = 1000;
		projectionMatrix = CreatePerspectiveFieldOfView(fieldOfView, aspectRadio, near, far);

		zBuffer = new int*[screenWidth];
		for (int i = 0; i < screenWidth; ++i) 
		{
			zBuffer[i] = new int[screenHeight];
		}

	}

	Renderer::~Renderer() 
	{
		if (camera) 
		{
			delete camera;
			camera = nullptr;
		}

		if (reflector) 
		{
			delete reflector;
			reflector = nullptr;
		}

		if (shader) 
		{
			delete shader;
			shader = nullptr;
		}

		for (int i = 0; i < this->SCREEN_WIDTH; ++i) 
		{
			delete[] zBuffer[i];
		}
		delete[] zBuffer;
	}

	void Renderer::AddModel(Model* model) 
	{
		models.push_back(model);
	}

	void Renderer::AddLight(Light* light)
	{
		lights.push_back(light);
	}

	void Renderer::SetCamera(CAMERA _camera)
	{
		if (camera)
		{
			delete camera;
			camera = nullptr;
		}
		switch (_camera)
		{
		case gfx::FIXED_CAMERA:
			camera = new FixedCamera(cameraStaticPosition, cameraStaticTarget);
			break;
		case gfx::INTERACTIVE_CAMERA:
			camera = new InteractiveCamera(cameraStaticPosition, cameraTrackedObject->Position());
			break;
		case gfx::TRACKING_CAMERA:
			camera = new TrackingCamera(cameraTrackedObject->Position(), cameraTrackingDistance);
			break;
		default:
			break;
		}
		currentCamera = _camera;
	}

	void Renderer::SetShader(SHADER _shader)
	{
		if (shader) 
		{
			delete shader;
			shader = nullptr;
		}
		switch (_shader)
		{
		case gfx::PLAIN_SHADER:
			shader = new PlainShader(lights, camera);
			break;
		case gfx::GOURAUD_SHADER:
			shader = new GouraudShader(lights, camera);
			break;
		case gfx::PHONG_SHADER:
			shader = new PhongShader(lights, camera);
			break;
		default:
			break;
		}
		currentShader = _shader;
	}

	void Renderer::SetReflector(REFLECTOR _reflector)
	{
		if (reflector) 
		{
			delete reflector;
			reflector = nullptr;
		}
		switch (_reflector)
		{
		case PHONG_REFLECTOR:
			reflector = new PhongReflector(lights, camera);
			break;
		case BLINN_REFLECTOR:
			reflector = new BlinnReflector(lights, camera);
			break;
		default:
			break;
		}
		currentReflector = _reflector;
	}

	void Renderer::SetCameraPosition(glm::vec4& _cameraStaticPosition)
	{
		cameraStaticPosition = _cameraStaticPosition;
	}

	void Renderer::SetCameraStaticTarget(glm::vec4& _cameraStaticTarget)
	{
		cameraStaticTarget = _cameraStaticTarget;
	}

	void Renderer::SetCameraTrackingDistance(glm::vec4& _cameraTrackingDistance)
	{
		cameraTrackingDistance = _cameraTrackingDistance;
	}

	void Renderer::SetCameraTrackedObject(Model* _cameraTrackedObject)
	{
		cameraTrackedObject = _cameraTrackedObject;
	}

	void Renderer::RenderScene()
	{
		if (!camera || !reflector || !shader) 
		{
			return;
		}

		Pixel* points[3];
		Vertex* wVertices[3];
		glm::vec4 pVertices[3];

		std::vector<Model*>::const_iterator model_it;
		std::vector<Mesh*>::const_iterator mesh_it;
		std::vector<Triangle>::const_iterator triangle_it;

		glm::mat4x4& pM = projectionMatrix;
		glm::mat4x4& vM = camera->ViewMatrix();

		for (int i = 0; i < SCREEN_WIDTH; ++i) 
		{
			for (int j = 0; j < SCREEN_HEIGHT; ++j) 
			{
				zBuffer[i][j] = INT_MAX;
			}
		}

		for (model_it = models.cbegin(); model_it != models.cend(); ++model_it) 
		{
			Model& model = **model_it;
			glm::mat4x4& wM = model.WorldMatrix();

			for (mesh_it = model.MeshesBegin(); mesh_it != model.MeshesEnd(); ++mesh_it) 
			{
				Mesh& mesh = **mesh_it;
				MaterialInfo& mInfo = mesh.Material();

				for (triangle_it = mesh.TrianglesBegin(); triangle_it != mesh.TrianglesEnd(); ++triangle_it) 
				{
					const Triangle& t = *triangle_it;
					
					for (int i = 0; i < 3; ++i) 
					{
						wVertices[i] = new Vertex(wM * t[i]);
						pVertices[i] = pM * vM * wVertices[i]->Point();
						pVertices[i] /= pVertices[i].w;
						points[i] = ProjectVertex(*wVertices[i], pVertices[i], mInfo);
					}
					if (CheckVisibility(pVertices)) 
					{
						FillTriangle(points, mInfo);
					}

					for (int i = 0; i < 3; ++i) 
					{
						delete wVertices[i];
						delete points[i];
					}
				}
			}
		}
	}

	Pixel* Renderer::ProjectVertex(const Vertex& wVertex, glm::vec4 pVertex, const MaterialInfo& mInfo)
	{
		int x = int(pVertex.x * SCREEN_WIDTH / 2 + SCREEN_WIDTH / 2);
		int y = int(SCREEN_HEIGHT / 2 - pVertex.y * SCREEN_HEIGHT / 2);
		int z = int(pVertex.z * INT_MAX);
		glm::vec3 color = reflector->Reflect(wVertex, mInfo);

		return new Pixel(x, y, z, color, wVertex);
	}

	bool Renderer::CheckVisibility(glm::vec4 pVertices[3])
	{
		for (int i = 0; i < 3; ++i) 
		{
			float z = pVertices[i].z;
			if (z < -1 || z > 1) 
			{
				return false;
			}
		}
		
		float ax = pVertices[1].x - pVertices[0].x;
		float ay = pVertices[1].y - pVertices[0].y;
		float bx = pVertices[1].x - pVertices[2].x;
		float by = pVertices[1].y - pVertices[2].y;
		
		return ax * by - bx * ay <= 0;
	}

	void Renderer::FillTriangle(Pixel* pnt[3], const MaterialInfo& mInfo)
	{
		for (int i = 0; i < 2; ++i) 
		{
			for (int j = 0; j < 2 - i; ++j) 
			{
				if (pnt[j]->Y() > pnt[j + 1]->Y()) 
				{
					Pixel* aux = pnt[j];
					pnt[j] = pnt[j + 1];
					pnt[j + 1] = aux;
				}
			}
		}

		float d01 = (pnt[1]->Y() > pnt[0]->Y()) ? (pnt[1]->X() - pnt[0]->X()) / (float)(pnt[1]->Y() - pnt[0]->Y()) : 0;
		float d02 = (pnt[2]->Y() > pnt[0]->Y()) ? (pnt[2]->X() - pnt[0]->X()) / (float)(pnt[2]->Y() - pnt[0]->Y()) : 0;
		float d12 = (pnt[2]->Y() > pnt[1]->Y()) ? (pnt[2]->X() - pnt[1]->X()) / (float)(pnt[2]->Y() - pnt[1]->Y()) : 0;

		int y = pnt[0]->Y();
		if (pnt[2]->Y() >= 0 || pnt[0]->Y() < SCREEN_HEIGHT)
		{
			float pos = float((pnt[1]->X() - pnt[0]->X()) * (pnt[2]->Y() - pnt[0]->Y()) 
				- (pnt[2]->X() - pnt[0]->X()) * (pnt[1]->Y() - pnt[0]->Y()));
			float xs = (float)pnt[0]->X(), xe = (float)pnt[0]->X();
			if (pos > 0)
			{
				while (y < pnt[1]->Y())
				{
					DrawLine(xs, xe, y, pnt, mInfo);
					xs += d02;
					xe += d01;
					++y;
				}
				xe = (float)pnt[1]->X();
				while (y < pnt[2]->Y())
				{
					DrawLine(xs, xe, y, pnt, mInfo);
					xs += d02;
					xe += d12;
					++y;
				}
			}
			else
			{
				while (y < pnt[1]->Y())
				{
					DrawLine(xs, xe, y, pnt, mInfo);
					xs += d01;
					xe += d02;
					++y;
				}
				xs = (float)pnt[1]->X();
				while (y < pnt[2]->Y())
				{
					DrawLine(xs, xe, y, pnt, mInfo);
					xs += d12;
					xe += d02;
					++y;
				}
			}
		}
	}

	void Renderer::DrawLine(float xs, float xe, int y, Pixel* pnt[3], const MaterialInfo& m)
	{
		if (y >= 0 && y < SCREEN_HEIGHT)
		{
			int s = (int)glm::max(0.f, glm::ceil(xs));
			int e = (int)glm::min(SCREEN_WIDTH - 1.f, glm::floor(xe));

			for (int x = s; x <= e; ++x)
			{
				Pixel pixel = shader->ShadePixel(x, y, pnt, m);
				if (pixel.Z() < zBuffer[x][y])
				{
					glm::vec3 colorRGB = pixel.Color();
					SDL_SetRenderDrawColor(sdlrenderer, pixel.R(), pixel.G(), pixel.B(), pixel.A());
					SDL_RenderDrawPoint(sdlrenderer, x, y);
					zBuffer[x][y] = pixel.Z();
				}
			}
		}
	}
}
