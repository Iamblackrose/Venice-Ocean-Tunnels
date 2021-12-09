#pragma once

#include <vector>
#include <gl/glew.h>

namespace GE {
	struct Vertexx {
		Vertexx(float xPos, float yPos, float zPos, float uCoord, float vCoord) {
			x = xPos;
			y = yPos;
			z = zPos;
			u = uCoord;
			v = vCoord;
		}
		float x, y, z, u, v;
	};

	unsigned int createVBO(const std::vector<Vertexx>& vertices);
	unsigned int createIBO(const std::vector<unsigned int>& indices);
}