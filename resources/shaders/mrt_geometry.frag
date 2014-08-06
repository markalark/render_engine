#version 330

uniform sampler2D texture;
in vec3 frag_normal;
in vec2 frag_uv;
in vec4 frag_world_position;
in vec4 color;

layout(location = 0) out vec4 diffuse;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 world_position;

void main(void) {
    diffuse = texture2D(texture, 1 - frag_uv);
	normal = vec4((frag_normal * 0.5) + 0.5, 1.0);
	world_position = frag_world_position;
}