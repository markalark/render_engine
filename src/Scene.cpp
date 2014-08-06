#include <cassert>
#include "Scene.h"
#include "Engine.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Light.h"
#include "Model.h"
#include "Shader.h"
#include "GLUniform.h"
#include "Camera.h"
#define PI 3.14159265359

//
Scene::Scene() {
    camera = new Camera(Vec3f(5.0, 3.0, 0.0), Vec3f(0, 0, 0));
    Model *model = init_model(0);
    Light *light = new Light(Vec3f(2.0, 2.0, 0.0), Vec3f(5, 5, 5), 5);
    models.push_back(model);
    lights.push_back(light);
    unprepared_lights.push(light);
    unprepared_models.push(model);
}

//
void Scene::update() {
    for (Model *model : models) {
        model->rotate(rotation_direction * (PI / 1000), Vec3f(0, 1, 0));
    }
}

//
Model *Scene::init_model(unsigned int model_id) {
    // init shader
    Shader *shader = ShaderManager::get_instance()->get_shader("mrt_geometry.vert", "mrt_geometry.frag");

    // tmp model verts
    std::vector<Vec3f> vertices = {
        // front
        Vec3f(-1.0, -1.0, 1.0),
        Vec3f(1.0, -1.0, 1.0),
        Vec3f(1.0, 1.0, 1.0),
        Vec3f(-1.0, 1.0, 1.0),
        // top
        Vec3f(-1.0, 1.0, 1.0),
        Vec3f(1.0, 1.0, 1.0),
        Vec3f(1.0, 1.0, -1.0),
        Vec3f(-1.0, 1.0, -1.0),
        // back
        Vec3f(1.0, -1.0, -1.0),
        Vec3f(-1.0, -1.0, -1.0),
        Vec3f(-1.0, 1.0, -1.0),
        Vec3f(1.0, 1.0, -1.0),
        // bottom
        Vec3f(-1.0, -1.0, -1.0),
        Vec3f(1.0, -1.0, -1.0),
        Vec3f(1.0, -1.0, 1.0),
        Vec3f(-1.0, -1.0, 1.0),
        // left
        Vec3f(-1.0, -1.0, -1.0),
        Vec3f(-1.0, -1.0, 1.0),
        Vec3f(-1.0, 1.0, 1.0),
        Vec3f(-1.0, 1.0, -1.0),
        // right
        Vec3f(1.0, -1.0, 1.0),
        Vec3f(1.0, -1.0, -1.0),
        Vec3f(1.0, 1.0, -1.0),
        Vec3f(1.0, 1.0, 1.0),
    };
    std::vector<Vec3f> normals = {
        // front
        Vec3f(0.0, 0.0, 1.0),
        Vec3f(0.0, 0.0, 1.0),
        Vec3f(0.0, 0.0, 1.0),
        Vec3f(0.0, 0.0, 1.0),
        // top
        Vec3f(0.0, 1.0, 0.0),
        Vec3f(0.0, 1.0, 0.0),
        Vec3f(0.0, 1.0, 0.0),
        Vec3f(0.0, 1.0, 0.0),
        // back
        Vec3f(0.0, 0.0, -1.0),
        Vec3f(0.0, 0.0, -1.0),
        Vec3f(0.0, 0.0, -1.0),
        Vec3f(0.0, 0.0, -1.0),
        // bottom
        Vec3f(0.0, -1.0, 0.0),
        Vec3f(0.0, -1.0, 0.0),
        Vec3f(0.0, -1.0, 0.0),
        Vec3f(0.0, -1.0, 0.0),
        // left
        Vec3f(-1.0, 0.0, 0.0),
        Vec3f(-1.0, 0.0, 0.0),
        Vec3f(-1.0, 0.0, 0.0),
        Vec3f(-1.0, 0.0, 0.0),
        // right
        Vec3f(1.0, 0.0, 0.0),
        Vec3f(1.0, 0.0, 0.0),
        Vec3f(1.0, 0.0, 0.0),
        Vec3f(1.0, 0.0, 0.0),
    };
    std::vector<Vec2f> uvs = {
        // front
        Vec2f(0.0, 0.0),
        Vec2f(1.0, 0.0),
        Vec2f(1.0, 1.0),
        Vec2f(0.0, 1.0),
        // top
        Vec2f(0.0, 0.0),
        Vec2f(1.0, 0.0),
        Vec2f(1.0, 1.0),
        Vec2f(0.0, 1.0),
        // back
        Vec2f(0.0, 0.0),
        Vec2f(1.0, 0.0),
        Vec2f(1.0, 1.0),
        Vec2f(0.0, 1.0),
        // bottom
        Vec2f(0.0, 0.0),
        Vec2f(1.0, 0.0),
        Vec2f(1.0, 1.0),
        Vec2f(0.0, 1.0),
        // left
        Vec2f(0.0, 0.0),
        Vec2f(1.0, 0.0),
        Vec2f(1.0, 1.0),
        Vec2f(0.0, 1.0),
        // right
        Vec2f(0.0, 0.0),
        Vec2f(1.0, 0.0),
        Vec2f(1.0, 1.0),
        Vec2f(0.0, 1.0),

    };
    Texture *texture = TextureManager::get_instance()->get_texture("crate.png");

    // the model
    Model *model = new Model(model_id, vertices, normals, uvs);

    /** a vertex group is a piece of the mesh idenitified by the indices of the used
     * vertices.They are drawn as opengl 'elements' with 'glDrawElements' */
    std::vector<unsigned int> indices = {
        // front
        0, 1, 2, 2, 3, 0,
        // top
        4, 5, 6, 6, 7, 4,
        // back
        8, 9, 10, 10, 11, 8,
        // bottom
        12, 13, 14, 14, 15, 12,
        // left
        16, 17, 18, 18, 19, 16,
        // right
        20, 21, 22, 22, 23, 20,
    };
    model->add_vertex_group(indices, texture, shader);

    // return the tmp model
    return model;
}

// ---------- Lua Interface --------- //
std::vector<luaL_Reg> Scene::lua_class_functions = {
    {"toggle_cube_rotation_direction", Scene::toggle_rotation_direction},
    {NULL, NULL}
};
std::vector<luaL_Reg> Scene::lua_member_functions = {
    {NULL, NULL}
};

//
int Scene::rotation_direction = 1;
int Scene::toggle_rotation_direction(lua_State * L) {
    int direction = luaL_checkint(L, 1);
    Scene::rotation_direction *= direction;
    return 1;
}