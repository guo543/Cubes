#version 330

smooth in vec3 fragNorm;	// Interpolated model-space normal
smooth in vec2 fragUV;

out vec3 outCol;	// Final pixel color

uniform sampler2D cubeTex;

void main() {
	// Visualize normals as colors
	//outCol = normalize(fragNorm) * 0.5f + vec3(0.5f);

	//vec3 light = normalize(vec3(-0.4, 0.6, 0.7));
	//vec3 norm = normalize(fragNorm);

	//float ambient = 0.5;

	//float diffuse = max(dot(light, norm), 0.0);
	//outCol = (ambient + diffuse) * vec3(0.0, 1.0, 1.0);
	outCol = texture(cubeTex, fragUV).xyz;
}
