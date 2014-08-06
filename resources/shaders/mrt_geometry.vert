#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 vert_uv;

out vec3 frag_normal;
out vec2 frag_uv;
out vec4 frag_world_position;
out vec4 color;

uniform mat4 mvp;
uniform mat4 world_space;

// 
void main() {
  frag_uv.x = vert_uv.x;
  frag_uv.y = vert_uv.y;
  frag_normal = (world_space * vec4(vert_normal, 1)).xyz;
  frag_world_position = world_space * vec4(position, 1);
  gl_Position = mvp * vec4(position, 1); 
}
