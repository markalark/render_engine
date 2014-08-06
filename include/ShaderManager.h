/*
 * File:   ShaderManager.h
 * Author: Markalark
 *
 */

#ifndef SHADERMANAGER_H
#define	SHADERMANAGER_H

#include <map>
#include <string>
#include <iostream>
#include <lua.hpp>
#include <vector>
#include "Shader.h"
class ShaderManager {
public:
    static ShaderManager *get_instance() {
        static ShaderManager *manager = new ShaderManager();
        return manager;
    }
    Shader *register_shader(std::string vertex_filename, std::string frag_filename) {
        Shader *shader = new Shader("resources/shaders/" + vertex_filename, "resources/shaders/" + frag_filename);
        std::string shader_name = vertex_filename + ", " + frag_filename;
        if (shaders.find(shader_name) == std::end(shaders)) {
            shaders[shader_name] = shader;
        }
        std::cout << "registered shader: " << shader_name << std::endl;
        return shader;
    }
    Shader *get_shader(std::string vertex_filename, std::string frag_filename) {
        return shaders[vertex_filename + ", " + frag_filename];
    }

private:
    ShaderManager() { }
    std::map<std::string, Shader *> shaders;
};



#endif	/* SHADERMANAGER_H */

