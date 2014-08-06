#version 330
in vec2 frag_uv;
out vec4 color;
uniform sampler2D texture;

// 
void main() {
  //color = vec4(1, 0, 0, 1);
  color = texture2D(texture, 1 - frag_uv.st).rgba;
}
