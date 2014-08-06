/*
 * File:   Engine.cpp
 * Author: Markalark
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "gl_headers.h"
#include "Engine.h"
#include "LuaUtils.h"
#include "Vec3.h"
#include "Vec2.h"
#include "Renderer.h"
#include "Interface.h"
#include "Scene.h"
#include "View.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Element.h"
Engine::Engine() { }


// texture registering
void Engine::register_textures() {
    TextureManager::get_instance()->register_texture("crate.png");
    TextureManager::get_instance()->register_texture("anime_chick.png");
    TextureManager::get_instance()->register_texture("anime_chick_jumping.png");
    TextureManager::get_instance()->register_texture("turn_button_idle.png");
    TextureManager::get_instance()->register_texture("turn_button_pressed.png");
    TextureManager::get_instance()->register_texture("interface_bg.png");
}

// shader registering
void Engine::register_shaders() {
    // for models
    Shader *shader = ShaderManager::get_instance()->register_shader("test.vert", "test.frag");
    shader->compile();
    shader->register_uniform("mvp", Shader::MVP);
    shader->register_uniform("texture", Shader::TEXTURE);
    // for sprites
    shader = ShaderManager::get_instance()->register_shader("2d_default.vert", "2d_default.frag");
    shader->compile();
    shader->register_uniform("mvp", Shader::MVP);
    shader->register_uniform("texture", Shader::TEXTURE);
    // for testing 2d
    shader = ShaderManager::get_instance()->register_shader("simple_2d.vert", "simple_2d.frag");
    shader->compile();
    shader->register_uniform("texture", Shader::TEXTURE);
    // for deferred models
    shader = ShaderManager::get_instance()->register_shader("mrt_geometry.vert", "mrt_geometry.frag");
    shader->compile();
    shader->register_uniform("mvp", Shader::MVP);
    shader->register_uniform("texture", Shader::TEXTURE);
    shader->register_uniform("world_space", Shader::WORLD_SPACE);
    // for deferred lights
    shader = ShaderManager::get_instance()->register_shader("mrt_lighting.vert", "mrt_lighting.frag");
    shader->compile();
    shader->register_uniform("mvp", Shader::MVP);
    shader->register_uniform("camera_position", Shader::CAMERA_POSITION);
    shader->register_uniform("screen_size", Shader::SCREEN_SIZE);
    shader->register_uniform("position_texture", Shader::POSITION_TEXTURE);
    shader->register_uniform("normal_texture", Shader::NORMAL_TEXTURE);
    shader->register_uniform("light_radius", Shader::LIGHT_RADIUS);
    shader->register_uniform("light_position", Shader::LIGHT_POSITION);
    shader->register_uniform("light_color", Shader::LIGHT_COLOR);
    shader->register_uniform("light_intensity", Shader::LIGHT_INTENSITY);
    // for deferred final pass
    shader = ShaderManager::get_instance()->register_shader("mrt_final_pass.vert", "mrt_final_pass.frag");
    shader->compile();
    shader->register_uniform("diffuse_texture", Shader::TEXTURE);
    shader->register_uniform("emissive_texture", Shader::EMISSIVE_TEXTURE);
    shader->register_uniform("specular_texture", Shader::SPECULAR_TEXTURE);
    // for viewing world position texture
    shader = ShaderManager::get_instance()->register_shader("world_positions.vert", "world_positions.frag");
    shader->compile();
    shader->register_uniform("texture", Shader::TEXTURE);
    // for viewing simple 3d objects
    shader = ShaderManager::get_instance()->register_shader("simple_3d.vert", "simple_3d.frag");
    shader->compile();
    shader->register_uniform("mvp", Shader::MVP);
    // for viewing normals
    shader = ShaderManager::get_instance()->register_shader("normals.vert", "normals.frag");
    shader->compile();
    shader->register_uniform("texture", Shader::TEXTURE);
}

// register lua class bindings
lua_State *Engine::register_lua_bindings() {
    // init lua
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    //Engine
    Lua::push_class_binding(L, "Engine", Engine::lua_class_functions, Engine::lua_member_functions);

    // Engine.TextureManger
    Lua::push_class_binding(L, "Engine.TextureManager", TextureManager::lua_class_functions, TextureManager::lua_member_functions);
    lua_setfield(L, -2, "TextureManager");

    // Engine.Interface
    Lua::push_class_binding(L, "Engine.Interface", Interface::lua_class_functions, Interface::lua_member_functions);

    // Engine.Interface.Element
    Lua::push_class_binding(L, "Engine.Interface.Element", Element::lua_class_functions, Element::lua_member_functions);
    lua_setfield(L, -2, "Element");
    lua_setfield(L, -2, "Interface");

    // Engine.Scene
    Lua::push_class_binding(L, "Engine.Scene", Scene::lua_class_functions, Scene::lua_member_functions);
    lua_setfield(L, -2, "Scene");

    lua_setglobal(L, "Engine");
    return L;
}

//
int Engine::start() {
    //
    Vec2f window_size(640, 480);

    // init glfw
    if (!glfwInit()) {
        return -1;
    }

    // create window
    window = glfwCreateWindow(window_size.x, window_size.y, "Hey.", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // set this window as the opengl context
    glfwMakeContextCurrent(window);

    // input callbacks
    glfwSetKeyCallback(window, Engine::key_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // glew init
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* glew failed to init*/
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // register textures used by program
    register_textures();

    // register shaders used by program
    register_shaders();

    // register lua bindings
    lua_state = register_lua_bindings();

    // used to bind data to the draw buffers
    Renderer *renderer = new Renderer(window_size);

    // setup the view (used to draw the scene)
    View view(window_size, renderer);

    // create the scene and it's interface
    interface = Interface::load("test.lua");
    scene = new Scene();

    // the loop
    while (!glfwWindowShouldClose(window)) {
        // update
        scene->update();

        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw
        view.render(scene);
        view.render(interface);

        // display
        glfwSwapBuffers(window);

        // update
        glfwPollEvents();
    }
    return 0;
}

//
void Engine::handle_input(int key, int action, int mods, Vec2f mouse_position) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    interface->handle_input(key, action, mods, mouse_position);
}
void Engine::mouse_button_callback(GLFWwindow* window, int key, int action, int mods) {
    Vec2d pos;
    glfwGetCursorPos(window, &pos.x, &pos.y);
    Engine::get_instance()->handle_input(key, action, mods, pos);
}
//
void Engine::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Vec2d pos;
    glfwGetCursorPos(window, &pos.x, &pos.y);
    Engine::get_instance()->handle_input(key, action, mods, pos);
}
void Engine::mouse_move_callback(GLFWwindow* window, double x, double y) { }


// ------------ Lua Interface ---------------- //
std::vector<luaL_Reg> Engine::lua_class_functions = {
    {NULL, NULL}
};
std::vector<luaL_Reg> Engine::lua_member_functions = {
    {NULL, NULL}
};

