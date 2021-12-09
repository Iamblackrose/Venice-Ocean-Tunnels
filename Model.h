#pragma once
#include <GL/glew.h>

namespace GE {
	struct Vertex {
		float x, y, z;
		float u, v;

		Vertex(float _x, float _y, float _z, float _u, float _v) {
			x = _x;
			y = _y;
			z = _z;

			u = _u;
			v = _v;
		}

		Vertex() {
			x = y = z = 0.0f;
			u = v = 0.0f;
		}
	};

	class Model {
	public:
		Model(GLuint vboModel, int numModelVertices) {
			vertices = nullptr;
			numVertices = numModelVertices;
			pos = vboModel;
		}

		~Model() {
			delete[] vertices;
		}

		void* getVertices() {
			return (void*)vertices;
		}

		int getNumVertices() {
			return numVertices;
		}

		GLuint getPos() {
			return pos;
		}

	private:
		Vertex* vertices;
		int numVertices;
		GLuint pos;
	};

	Model* loadFromFile(const char* filename);

}