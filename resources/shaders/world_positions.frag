#version 330
uniform sampler2D texture;
in vec2 frag_uv;
out vec4 color;

// 
void main() {
    vec4 position = texture2D(texture, 1 - frag_uv);
	if (position.x != 0 || position.y != 0 || position.z != 0) {
		color = (position / 2) + .5;
	} else {
		color = vec4(0, 0, 0, 1);
	}
}
