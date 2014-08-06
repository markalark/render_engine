/*
 * File:   Engine.h
 * Author: Markalark
 *
 */

#ifndef ENGINE_H
#define	ENGINE_H

#include <lua.hpp>
#include <vector>
#include "Input.h"
class GLFWwindow;
class Interface;
class Scene;

//
class Engine {
public:
    static Engine *get_instance() {
        static Engine *engine = new Engine();
        return engine;
    }
    int start();
    void handle_input(int key, int action, int mods, Vec2f mouse_position);
    lua_State *get_lua_state() {
        return lua_state;
    }
private:
    Engine();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int key, int action, int mods);
    static void mouse_move_callback(GLFWwindow* window, double x, double y);
    void register_textures();
    void register_shaders();
    GLFWwindow *window;
    Interface *interface;
    Scene *scene;
    lua_State *register_lua_bindings();
    lua_State *lua_state;

    // ------- Lua Interface --------- //
    static std::vector<luaL_Reg> lua_class_functions;
    static std::vector<luaL_Reg> lua_member_functions;
};

#endif	/* ENGINE_H */

