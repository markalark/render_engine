#version 330

layout(location = 0) out vec4 specular;
layout(location = 1) out vec4 emissive;

/** compute every fragment within the light's xy screen region. 
 * this doesn't mean the fragment is guaranteed to be within the light's volume */
void main(void) {
	specular = vec4(1, 0, 0, 1);
	emissive = vec4(0, 0, 1, 1);
}