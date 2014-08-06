#include <iostream>
#include <fstream>
#include "gl_headers.h"
#include "Shader.h"

// read files
std::string read_file(std::string filepath) {
    std::string contents;
    std::ifstream file_stream(filepath.c_str(), std::ios::in);
    if (file_stream.is_open()) {
        std::string line = "";
        while (std::getline(file_stream, line)) {
            contents += "\n" + line;
        }
        file_stream.close();
    } else {
        std::cout << "couldn't find " << filepath << std::endl;
    }
    return contents;
}

//
Shader::Shader(std::string vertex_filepath, std::string fragment_filepath) :
    vertex_filepath(vertex_filepath),
    fragment_filepath(fragment_filepath),
    compiled(false), id(0) { }

//
void Shader::compile() {
    // read the vertex shader code from file
    std::string vert_code = read_file(this->vertex_filepath);

    // read the fragment shader code from file
    std::string frag_code = read_file(this->fragment_filepath);

    // compile vertex shader
    std::cout << "Compiling shader: " << vertex_filepath << std::endl;
    GLuint vert_shader_id = compile_shader(GL_VERTEX_SHADER, vert_code);

    // print compiling errors
    std::cout << get_shader_log(vert_shader_id) << std::endl;

    // compile fragment shader
    std::cout << "Compiling shader: " << fragment_filepath << std::endl;
    GLuint frag_shader_id = compile_shader(GL_FRAGMENT_SHADER, frag_code);

    // print compiling errors
    std::cout << get_shader_log(frag_shader_id) << std::endl;

    // link the shader program
    std::cout << "Linking program" << std::endl;
    GLuint program_id = link_shaders(vert_shader_id, frag_shader_id);

    // print linking errors
    std::cout << get_program_log(program_id) << std::endl;

    // remove the shaders now that we have the program
    glDeleteShader(vert_shader_id);
    glDeleteShader(frag_shader_id);

    // set the id
    this->id = program_id;
    this->compiled = true;
}

//
GLuint Shader::compile_shader(GLenum shader_type, std::string code) {
    // create id
    GLuint shader_id = glCreateShader(shader_type);

    // collection of code pieces (just one for my case)
    std::vector<const char *> codes = {code.c_str()};

    // construct the source from an array of strings (just one string for my case)
    glShaderSource(
                   shader_id,
                   1, // number of strings used to contruct the code (it will add strings together)
                   &codes.at(0), // the array of c strings
                   NULL); // array of lengths for the strings (optional for null terminated strings)

    // compile the source
    glCompileShader(shader_id);

    return shader_id;
}

//
GLuint Shader::link_shaders(GLuint vert_shader_id, GLuint frag_shader_id) {
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vert_shader_id);
    glAttachShader(program_id, frag_shader_id);
    glLinkProgram(program_id);

    return program_id;
}

//
void Shader::register_uniform(std::string uniform_name, int uniform_id) {
    GLint location = glGetUniformLocation(this->id, uniform_name.c_str());
    while (uniform_locations.size() <= uniform_id) {
        uniform_locations.push_back(-1);
    }
    uniform_locations[uniform_id] = location;
}

//
std::string Shader::get_shader_log(GLuint &shader_id) {
    // check compilation result
    GLint result = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);

    // fetch and set log length
    int log_length = 0;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> error_message(log_length);

    //fetch and return the log
    glGetShaderInfoLog(shader_id, log_length, NULL, &error_message[0]);
    std::string log = (result == GL_TRUE) ? "Success" : std::string(&error_message[0]);
    return "Compilation: " + log + "\n";
}

//
std::string Shader::get_program_log(GLuint &program_id) {
    // check linking result
    GLint result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);

    // fetch and set log length
    int log_length;
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
    std::vector<char> error_message(std::max(log_length, int(1)));

    //fetch and return the log
    glGetProgramInfoLog(program_id, log_length, NULL, &error_message[0]);
    std::string log = (result == GL_TRUE) ? "Success" : std::string(&error_message[0]);
    return "Linking: " + log + "\n";
}
