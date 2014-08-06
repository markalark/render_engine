#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 vert_uv;
out vec2 frag_uv;

// 
void main() {
  frag_uv.x = vert_uv.x;
  frag_uv.y = vert_uv.y;
  gl_Position = vec4(position.x, position.y, 0, 1);
}
