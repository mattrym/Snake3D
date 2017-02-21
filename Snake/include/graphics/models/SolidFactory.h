#pragma once

#ifndef _FIGUREFACTORY_H
#define _FIGUREFACTORY_H

#include <string>
#include "MeshBuilder.h"
#include "Model.h"

namespace gfx 
{
	class SolidFactory 
	{
	protected:
		MeshBuilder meshBuilder;
		SolidFactory() { }
	
	public:
		static SolidFactory& Instance();
		virtual ~SolidFactory() { }

		Model* CreateSurface(float edge, int width, int height, const MaterialInfo& mInfo);
		Model* CreateCuboid(float width, float height, float breadth, const MaterialInfo& mInfo);
		Model* CreateCylinder(float radius, float height, int n, const MaterialInfo& mInfo);
		Model* CreateSphere(float radius, int m, int n, const MaterialInfo& mInfo);
	};
}

#endif // !_FIGUREFACTORY_H
