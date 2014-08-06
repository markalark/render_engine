#ifndef SHADER_H
#define	SHADER_H

#include <string>
#include <vector>
#include "gl_headers.h"

//
class Shader {
public:
    Shader(std::string vertex_filepath, std::string fragment_filepath);
    void compile();
    bool is_compiled() const {
        return compiled;
    }
    GLuint get_id() const {
        return id;
    }
    // get the location of the uniform in the shader based on the uniform's global id
    GLint get_uniform_location(int uniform_id) const {
        return uniform_locations[uniform_id];
    }
    void register_uniform(std::string uniform_name, int uniform_id);

    // important uniform ids
    enum UniformId {
        MVP = 0,
        TEXTURE,
        NORMAL_TEXTURE,
        DEPTH_TEXTURE,
        POSITION_TEXTURE,
        SPECULAR_TEXTURE,
        EMISSIVE_TEXTURE,
        INVERSE_PROJECTION_VIEW,
        CAMERA_POSITION,
        WORLD_SPACE,
        SCREEN_SIZE,
        LIGHT_RADIUS,
        LIGHT_POSITION,
        LIGHT_COLOR,
        LIGHT_INTENSITY
    };
private:
    GLuint compile_shader(GLuint id, std::string code);
    GLuint link_shaders(GLuint vert_shader_id, GLuint frag_shader_id);
    std::string get_shader_log(GLuint &shader_id);
    std::string get_program_log(GLuint &program_id);
    std::string vertex_filepath;
    std::string fragment_filepath;
    std::vector<GLint> uniform_locations;
    bool compiled;
    GLuint id;
};

#endif	/* SHADER_H */

