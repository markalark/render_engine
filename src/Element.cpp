#include "Element.h"
#include "Texture.h"
#include "LuaUtils.h"

// ========== Lua Interface ================= //
std::vector<luaL_Reg> Element::lua_member_functions = {
    {"get_position", Element::get_position},
    {"get_name", Element::get_name},
    {"set_position", Element::set_position},
    {"move", Element::move},
    {"get_size", Element::get_size},
    {"set_size", Element::set_size},
    {"set_texture", Element::set_texture},
    {NULL, NULL}
};
std::vector<luaL_Reg> Element::lua_class_functions = {
    {NULL, NULL}
};

// return the position to lua
int Element::get_position(lua_State * L) {
    Element *element = (Element *) Lua::get_data(L, 1);
    Vec2f position = element->get_position();
    lua_pushnumber(L, position.x);
    lua_pushnumber(L, position.y);
    return 2;
}

// return the name to lua
int Element::get_name(lua_State * L) {
    Element *element = (Element *) Lua::get_data(L, 1);
    lua_pushstring(L, element->get_name().c_str());
    return 1;
}

// set position to lua arguments
int Element::set_position(lua_State * L) {
    Element *element = (Element *) Lua::get_data(L, 1);
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    element->set_position(Vec2f(x, y));
    return 0;
}
int Element::move(lua_State * L) {
    Element *element = (Element *) Lua::get_data(L, 1);
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    element->move(Vec2f(x, y));
    return 0;
}
int Element::get_size(lua_State * L) {
    Element *element = (Element *) Lua::get_data(L, 1);
    Vec2f size = element->get_size();
    lua_pushnumber(L, size.x);
    lua_pushnumber(L, size.y);
    return 2;
}
int Element::set_size(lua_State * L) {
    Element *element = (Element *) Lua::get_data(L, 1);
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    element->set_size(Vec2f(x, y));
    return 0;
}
int Element::set_texture(lua_State * L) {
    Element *element = (Element *) Lua::get_data(L, 1);
    Texture *texture = *((Texture **) lua_touserdata(L, 2));
    element->set_texture(texture);
}
