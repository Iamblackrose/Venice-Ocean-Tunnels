#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Model.h"
#include <iostream>

namespace GE {
	Model* loadFromFile(const char* filename) {
		std::vector<Vertex> loadedVertices;

		Assimp::Importer imp;

		const aiScene* pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);

		if (!pScene) {
			std::cout << "Model file not found" << std::endl;
				
			return nullptr;
		}

		for (int MeshIdx = 0; MeshIdx < pScene->mNumMeshes; MeshIdx++) {
			const aiMesh* mesh = pScene->mMeshes[MeshIdx];

			for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
				const aiFace& face = mesh->mFaces[faceIdx];

				for (int vertIdx = 0; vertIdx < face.mNumIndices; vertIdx++) {
					const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertIdx]];

					const aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[vertIdx]];

					loadedVertices.push_back(Vertex(pos->x, pos->y, pos->z, uv.x, uv.y));
				}

				
			}
		}

		int numModelVertices = loadedVertices.size();

		GLuint vboModel;

		glGenBuffers(1, &vboModel);
		glBindBuffer(GL_ARRAY_BUFFER, vboModel);

		glBufferData(GL_ARRAY_BUFFER, numModelVertices * sizeof(Vertex), &loadedVertices[0], GL_STATIC_DRAW);

		return new Model(vboModel, numModelVertices);
	}

	
}