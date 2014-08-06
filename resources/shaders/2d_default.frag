#version 330
in vec2 frag_uv;
out vec4 color;
uniform sampler2D texture;

// 
void main() {
  //color = vec4(1, 0, 0, 1);
  color = texture2D(texture, frag_uv.st).rgba;
  //color = vec4(frag_uv.x, frag_uv.y, 0, 1);
}
