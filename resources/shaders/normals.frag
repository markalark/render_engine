#version 330
uniform sampler2D texture;
in vec2 frag_uv;
out vec4 color;

// 
void main() {
    vec4 normal = texture2D(texture, 1 - frag_uv);
	normal = (normal * 2.0) - 1.0;
	float negative_shading = -.3;
	if (normal.x < 0) normal.x *= negative_shading;
	if (normal.y < 0) normal.y *= negative_shading;
	if (normal.z < 0) normal.z *= negative_shading;
	color = normal;
}
