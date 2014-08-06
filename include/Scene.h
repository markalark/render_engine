#ifndef SCENE_H
#define	SCENE_H

#include <queue>
#include <lua.hpp>
#include "Model.h"
class Camera;
class Light;

//
class Scene {
public:
    Scene();
    void update();
    const std::vector<Model *> &get_models() {
        return models;
    }
    const std::vector<Light *> &get_lights() {
        return lights;
    }
    const Camera *get_camera() {
        return camera;
    }
    bool has_unprepared_models() {
        return !unprepared_models.empty();
    }
    Model *next_unprepared_model() {
        Model *model = unprepared_models.front();
        unprepared_models.pop();
        return model;
    }
    bool has_unprepared_lights() {
        return !unprepared_lights.empty();
    }
    Light *next_unprepared_light() {
        Light *light = unprepared_lights.front();
        unprepared_lights.pop();
        return light;
    }

    // ---------- Lua Interface --------- //
    static std::vector<luaL_Reg> lua_class_functions;
    static std::vector<luaL_Reg> lua_member_functions;
    static int toggle_rotation_direction(lua_State *L);
private:
    static int rotation_direction;
    Model *init_model(unsigned int model_id);
    Camera *camera;
    std::vector<Model *> models;
    std::vector<Light *> lights;
    std::queue<Model *> unprepared_models;
    std::queue<Light *> unprepared_lights;
    // TODO lights, etc
};

#endif	/* SCENE_H */

