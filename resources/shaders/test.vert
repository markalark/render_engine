#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vert_uv;
out vec2 frag_uv;
uniform mat4 mvp;

// 
void main() {
  vec4 v = vec4(position, 1);
  frag_uv.x = 1 - vert_uv.x;
  frag_uv.y = 1 - vert_uv.y;
  gl_Position = mvp * v;
}
