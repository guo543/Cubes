#version 330

layout(location = 0) in vec3 pos;		// Model-space position
layout(location = 1) in vec3 norm;		// Model-space normal
layout(location = 2) in vec2 uv;

smooth out vec3 fragNorm;	// Model-space interpolated normal
smooth out vec2 fragUV;

uniform mat4 xform;			// Model-to-clip space transform

void main() {

	// Interpolate normals
	fragNorm = norm;
	// Interpolate texture coordinates
	fragUV = uv;

	// Transform vertex position
	gl_Position = xform * vec4(pos, 1.0);
}