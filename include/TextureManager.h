/*
 * File:   TextureManager.h
 * Author: Markalark
 *
 */
#ifndef TEXTUREMANAGER_H
#define	TEXTUREMANAGER_H

#include <map>
#include <string>
#include <lua.hpp>
#include <vector>
#include "Texture.h"
class TextureManager {
public:
    static TextureManager *get_instance() {
        static TextureManager *manager = new TextureManager();
        return manager;
    }
    void register_texture(std::string filename) {
        Texture *texture = new Texture("resources/images/" + filename);
        if (textures.find(filename) == std::end(textures)) {
            textures[filename] = texture;
        }
        std::cout << "registered texture: " << filename << std::endl;
    }
    Texture *get_texture(std::string filename) {
        return textures[filename];
    }

    // ---------- Lua Interface --------- //
    static std::vector<luaL_Reg> lua_class_functions;
    static std::vector<luaL_Reg> lua_member_functions;
    static int get_texture(lua_State *L);
private:
    TextureManager() { }
    std::map<std::string, Texture *> textures;
};

#endif	/* TEXTUREMANAGER_H */
