#version 330

uniform vec3 camera_position;
uniform vec2 screen_size;
uniform sampler2D position_texture;
uniform sampler2D normal_texture;
uniform vec3 light_position;
uniform vec3 light_color;
uniform float light_intensity;

layout(location = 0) out vec4 specular;
layout(location = 1) out vec4 emissive;

/** compute every fragment within the light's xy screen region. 
 * this doesn't mean the fragment is guaranteed to be within the light's volume */
void main(void) {
    // get the screen position from the geometry pass
	vec3 screen_position = vec3(gl_FragCoord.x / screen_size.x,
                         gl_FragCoord.y / screen_size.y,
						 0.0);
	
	// get normal and convert from screen space [0, 1] to clip space [-1, 1]
	vec3 color_normal = texture(normal_texture, screen_position.xy).xyz;
	vec3 normal = (color_normal * 2.0) - 1.0;
	
	// get world position
	vec3 world_position = texture(position_texture, screen_position.xy).xyz;
	
	// light distance and attenuation
	float light_distance = length(light_position - world_position);
	float attenuation = 1.0 - clamp(light_distance / light_intensity, 0.0, 1.0);

	// discard fragments outside the bounds of the light
	if (attenuation == 0.0) {
        discard;
	}
	
	// calculate lighting properties
	vec3 incident = normalize(light_position - world_position);
	vec3 view_direction = normalize(camera_position - world_position);
	vec3 half_angle = normalize(incident + view_direction);
	float lambert = clamp(dot(incident, normal), 0.0, 1.0);
	float r_factor = clamp(dot(half_angle, normal), 0.0, 1.0);
	float s_factor = pow(r_factor, 3.0);
	
	// output textures
	emissive = vec4(light_color.xyz * lambert * attenuation, 1.0);
	
	//specular = vec4(camera_position, 1);
	//specular = vec4(color_normal, 1);
	specular = vec4(light_color.xyz * s_factor * attenuation, 1);
}