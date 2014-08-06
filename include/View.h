#ifndef VIEW_H
#define	VIEW_H
#include <vector>
#include "gl_headers.h"
#include "Vec2.h"
#include "Light.h"
#include "GLUniform.h"
#include "DrawableRectangle.h"
class Rectangle;
class Renderer;
class Model;
class Scene;
class Camera;
class Interface;
class Element;

//
class View {
public:
    View(Vec2f window_size, Renderer *renderer);
    // convenience functions for rendering commonly used object types
    void render(Scene *scene);
    void render(Interface *interface);
    void render(Model *model, const Camera *camera);
    void render(Element *element);
    void render(DrawableBox &box, const Camera *camera);
    void render(DrawableRectangle &rectangle, Shader *shader, std::vector<gl::Uniform> uniforms);
    void render(Light *light, const Camera *camera, std::vector<gl::Uniform> additional_uniforms);
    void prepare(Scene *scene);
    void prepare(Interface *interface);
    void prepare(Model *model);
    void prepare(Light *light);

private:
    gl::DataBlock get_rectangle_vertex_data(const Rectangle &rectangle);
    gl::DataBlock get_rectangle_uv_data(const Rectangle &rectangle);
    gl::DataBlock get_box_vertex_data(const Box &box);
    gl::DataBlock get_box_uv_data(const Box &box);
    Renderer *renderer;
    DrawableRectangle drawable_screen_region;
    Vec2f window_size;
    glm::mat4 projection;

    // create a data block that points to the contents of a vector
    template<typename T>
    gl::DataBlock get_vector_data(const T& vec) {
        // initial check
        if (vec.empty()) {
            return gl::DataBlock(nullptr, 0);
        }

        // create a data copier that appends data from the vector onto the specified address
        auto data_copier = [&vec](void *copy_address, size_t data_size) {
            size_t offset = 0;
            for (int i = 0; i < vec.size() && offset < data_size; i++) {
                std::memcpy(copy_address + offset, &vec[i], sizeof (vec[0]));
                offset += sizeof (vec[0]);
            }
        };
        size_t data_size = sizeof (vec[0]) * vec.size();

        return gl::DataBlock(data_copier, data_size);
    }
};

//

#endif	/* VIEW_H */