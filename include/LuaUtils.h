/*
 * File:   LuaUtils.h
 * Author: Markalark
 *
 * Created on July 9, 2014, 10:24 PM
 */

#ifndef LUAUTILS_H
#define	LUAUTILS_H
#include <lua.hpp>
#include "Vec2.h"
#include "Vec3.h"

//
class Lua {
public:
    struct Callback {
        Callback(lua_State *L, int stack_index) : L(L) {
            lua_pushvalue(L, stack_index);
            index = luaL_ref(L, LUA_REGISTRYINDEX);
            lua_pop(L, 1);
        }
        void call() const {
            lua_rawgeti(L, LUA_REGISTRYINDEX, index);
            if (lua_pcall(L, 0, 0, 0)) {
                std::cout << "Lua callback had errors" << std::endl;
                printf("%s\n", lua_tostring(L, -1));
            }
        }
        ~Callback() {
            //            luaL_unref(L, LUA_REGISTRYINDEX, index);
        }
    private:
        lua_State *L;
        int index;
    };
    static int push_class_binding(lua_State *L,
                                  std::string name,
                                  std::vector<luaL_Reg> class_functions,
                                  std::vector<luaL_Reg> member_functions) {
        // set member functions
        luaL_newmetatable(L, name.c_str());
        luaL_setfuncs(L, member_functions.data(), 0);

        // metatable.__index = metatable
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");
        lua_pop(L, 1);

        // return table with class functions
        lua_newtable(L);
        luaL_setfuncs(L, class_functions.data(), 0);

        return 1;
    }
    static void create_object(lua_State *L, std::string metatable_name, void *data) {
        // Create new lua object
        lua_newtable(L);

        // push the named metatable found in registry into front of stack
        luaL_getmetatable(L, metatable_name.c_str());

        // object.metatable = class metatable
        lua_setmetatable(L, -2);

        // store the element pointer into the lua object
        Lua::set_data(L, -1, (void *) data);
    }
    static void *get_data(lua_State *L, int index) {
        luaL_checktype(L, index, LUA_TTABLE);
        lua_getfield(L, index, "data");
        void *data = *((void **) lua_touserdata(L, -1));
        return data;
    }
    static void *set_data(lua_State *L, int index, void *data) {
        lua_pushvalue(L, index);
        void **data_handle = (void **) lua_newuserdata(L, sizeof (void *));
        *data_handle = data;
        lua_setfield(L, -2, "data");
        lua_pop(L, 1);
    }
    static Vec2f get_vec2f(lua_State *L, int index) {
        lua_rawgeti(L, index, 1); // x
        lua_rawgeti(L, index, 2); // y
        Vec2f result = Vec2f((float) luaL_checknumber(L, -2),
                             (float) luaL_checknumber(L, -1));
        lua_pop(L, 2);
        return result;
    }
    static Vec3f get_vec3f(lua_State *L, int index) {
        lua_rawgeti(L, index, 1); // x
        lua_rawgeti(L, index, 2); // y
        lua_rawgeti(L, index, 3); // z
        Vec3f result = Vec3f((float) luaL_checknumber(L, -3),
                             (float) luaL_checknumber(L, -2),
                             (float) luaL_checknumber(L, -1));
        lua_pop(L, 3);
    }
    static Vec2i get_vec2i(lua_State *L, int index) {
        lua_rawgeti(L, index, 1); // x
        lua_rawgeti(L, index, 2); // y
        Vec2i result = Vec2i((int) luaL_checkint(L, -2),
                             (int) luaL_checkint(L, -1));
        lua_pop(L, 2);
        return result;
    }
    static Vec3i get_vec3i(lua_State *L, int index) {
        lua_rawgeti(L, index, 1); // x
        lua_rawgeti(L, index, 2); // y
        lua_rawgeti(L, index, 3); // z
        Vec3i result = Vec3i((int) luaL_checkint(L, -3),
                             (int) luaL_checkint(L, -2),
                             (int) luaL_checkint(L, -1));
        lua_pop(L, 3);
    }
private:
};

#endif	/* LUAUTILS_H */

