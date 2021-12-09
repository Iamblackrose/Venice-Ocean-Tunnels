#include "ModelRenderer.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace GE {

	// Class default constructor
	ModelRenderer::ModelRenderer(Camera* c) : cam(c) {
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = scale_y = scale_z = 1.0f;

		
	}

	// Class destructor
	ModelRenderer::~ModelRenderer() {

	}

	// Helper function that displays shader compiler error messages
	void displayShaderCompilerError(GLuint shaderId) {
		// Get the length of the error message string
		GLint MsgLen = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &MsgLen);

		// if the message is longer than one character, then display it
		if (MsgLen > 1) {
			// Create a character buffer to store the error message chars
			GLchar* Msg = new GLchar[MsgLen + 1];
			// Retrieve error message from OpenGL internal log
			glGetShaderInfoLog(shaderId, MsgLen, NULL, Msg);
			// Display the error message
			std::cerr << "Error compiling shader: " << Msg << std::endl;
			// release memory allocated to the message string
			delete[] Msg;
		}
	}

	// Create and compiles the shaders, create the project and links it, 
	// create the VBO
	void ModelRenderer::init()
	{
		// Create the vertex shader - must be compiled before attaching to the program
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Vertex Shader code
		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos3D;\n"
			"in vec2 vUV;\n"
			"out vec2 uv;\n"
			"uniform mat4 transform;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main() {\n"
			"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
			"v = projection * view * transform * v;\n"
			"gl_Position = v;\n"
			"uv = vUV;\n"
			"}\n" };
		// Copy the shader source to OpenGL for compilation
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);
		// Compile this shader
		glCompileShader(vertexShader);
		// Check for compiler errors, default assumption is fail
		GLint isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);
		// Failed to compile?
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader!" << std::endl;
			displayShaderCompilerError(vertexShader);
			return;
		}

		// Same process for the fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Fragment shader code
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec2 uv;\n"
			"uniform sampler2D sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main() {\n"
			"fragmentColour = texture(sampler, uv).rgba;\n"
			"}\n"
		};
		// Transfer and compile the shader
		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);
		glCompileShader(fragmentShader);
		// Check for compiler errors, default assumption is fail
		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);
		// Failed to compile?
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader!" << std::endl;
			displayShaderCompilerError(fragmentShader);
			return;
		}

		// Create the program object
		programId = glCreateProgram();

		// Attach shaders to the program object
		glAttachShader(programId, vertexShader);
		glAttachShader(programId, fragmentShader);

		// Link the program to create an executable program and render the object
		// Program exists in graphics memory
		glLinkProgram(programId);
		// Check for link errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;
		}

		// Get a link to the vColour attribute to add to the shader
		vertexUVLocation = glGetAttribLocation(programId, "vUV");
		// Any errors?
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vUV" << std::endl;
		}

		// Now get a link to the vertexPos3D attribute so we can link to our vertices for rendering
		vertexPos3DLocation = glGetAttribLocation(programId, "vertexPos3D");
		// Any errors?
		if (vertexPos3DLocation == -1) {
			std::cerr << "Problem getting the vertex3DPos" << std::endl;
		}

		// Link the uniforms to the member fields
		transformUniformId = glGetUniformLocation(programId, "transform");
		viewUniformId = glGetUniformLocation(programId, "view");
		projectionUniformId = glGetUniformLocation(programId, "projection");
		samplerId = glGetUniformLocation(programId, "sampler");
	}

	// Update is where any animation would occur for example
	void ModelRenderer::update() {
		
	}

	// Draws the triangles from the buffer object
	void ModelRenderer::draw(Model* model, Texture* material, float pos_x, float pos_y, float pos_z) {

		glEnable(GL_CULL_FACE);

		GLuint(vboModel);

		//glGenBuffers(1, &vboModel);
		//glBindBuffer(GL_ARRAY_BUFFER, vboModel);

		//glBufferData(GL_ARRAY_BUFFER, model->getNumVertices() * sizeof(Vertex), model->getVertices(), GL_STATIC_DRAW);
		//try moving this to model.cpp ^^

		glm::mat4 transformationMat = glm::mat4(1.0f);
		transformationMat = glm::translate(transformationMat, glm::vec3(pos_x, pos_y, pos_z));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMat = glm::scale(transformationMat, glm::vec3(scale_x, scale_y, scale_z));

		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		glUseProgram(programId);

		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		glBindBuffer(GL_ARRAY_BUFFER, model->getPos());

		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, x));

		glEnableVertexAttribArray(vertexPos3DLocation);

		glVertexAttribPointer(vertexUVLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

		glEnableVertexAttribArray(vertexUVLocation);

		

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, material->getTextureName());

		glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices());

		glDisableVertexAttribArray(vertexPos3DLocation);

		glDisableVertexAttribArray(vertexUVLocation);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindTexture(GL_TEXTURE_2D, NULL);

		glUseProgram(0);

		glDisable(GL_CULL_FACE);
	}

	void ModelRenderer::drawTerrain(Terrain* terrain, float pos_x, float pos_y, float pos_z)
	{
		glUseProgram(programId);

		pi = glm::pi<float>();

		glEnable(GL_CULL_FACE);

		glm::mat4 transformationMat = glm::mat4(1.0f);
		glm::vec3 rotation(0, 0, 0), translation(0, 0, 0), scale(1, 1, 1);

		// Create and manipulate the Model Matrix
		glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix
		modelMatrix = glm::rotate(modelMatrix, rotation.x * 180.0f / pi, glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotation.y * 180.0f / pi, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, rotation.z * 180.0f / pi, glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMat = glm::translate(transformationMat, glm::vec3(pos_x, pos_y, pos_z));
		modelMatrix = glm::scale(modelMatrix, scale);

		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat));
		glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat));

		glEnableVertexAttribArray(vertexPos3DLocation);
		glBindBuffer(GL_ARRAY_BUFFER, terrain->getVBO());
		glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GE::Vertex), (void*)offsetof(Vertex, x));
		//glVertexAttribPointer(vertexPos3DLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		//glBindBuffer(GL_ARRAY_BUFFER, vboModel);

		glEnableVertexAttribArray(vertexUVLocation);
		glVertexAttribPointer(vertexUVLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GE::Vertex), (void*)offsetof(GE::Vertex, u));

		glEnableVertexAttribArray(vertexPos3DLocation);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);

		glBindTexture(GL_TEXTURE_2D, terrain->getTexture()->getTextureName());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain->getIBO());

		glDrawElements(GL_TRIANGLES, terrain->getIndexCount(), GL_UNSIGNED_INT, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);


		glDisableVertexAttribArray(vertexPos3DLocation);

		glDisableVertexAttribArray(vertexUVLocation);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindTexture(GL_TEXTURE_2D, NULL);

		glUseProgram(0);

		glDisable(GL_CULL_FACE);

	}

	// Release objects allocated memory, program and VBO
	void ModelRenderer::destroy() {
		glDeleteProgram(programId);
		glDeleteBuffers(1, &vboModel);
	}
}