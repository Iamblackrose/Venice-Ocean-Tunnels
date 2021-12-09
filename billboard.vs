#version 140

// Inputs to vertex shader
in vec3 vertexPos3D;
in vec2 vUV;

// Output
out vec2 uv;

// MVP transformations
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

// Shader main code
void main() {
	// Convert input vertex to 4D 
	vec4 v = vec4(vertexPos3D.xyz, 1);

	// Transform
	v = projection * view * transform * v;

	// Pass transformed vertex to next stage
	gl_Position = v;

	// As well as uv.  Don't transform uv
	uv = vUV;
}