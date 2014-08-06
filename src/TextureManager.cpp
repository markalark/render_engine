#include "TextureManager.h"

// ---------- Lua Interface --------- //
std::vector<luaL_Reg> TextureManager::lua_class_functions = {
    {"get", TextureManager::get_texture},
    {NULL, NULL}
};
std::vector<luaL_Reg> TextureManager::lua_member_functions = {
    {NULL, NULL}
};

//
int TextureManager::get_texture(lua_State * L) {
    std::string name = luaL_checkstring(L, 1);
    void **data_handle = (void **) lua_newuserdata(L, sizeof (void *));
    *data_handle = get_instance()->get_texture(name);
    return 1;
}

