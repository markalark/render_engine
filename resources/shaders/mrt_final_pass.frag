#version 330
in vec2 frag_uv;
uniform sampler2D diffuse_texture;
uniform sampler2D emissive_texture;
uniform sampler2D specular_texture;

out vec4 color;

// 
void main() {
  vec3 diffuse = texture(diffuse_texture, 1 - frag_uv).xyz;
  vec3 emissive = texture(emissive_texture, 1 - frag_uv).xyz;
  vec3 specular = texture(specular_texture, 1 - frag_uv).xyz;
  color.xyz = diffuse * 0.02; // ambient
  color.xyz += diffuse * emissive; // lambert
  color.xyz += specular * 0.1; // specular
  color.a = 1;
}
