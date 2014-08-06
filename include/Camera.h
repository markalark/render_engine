#ifndef CAMERA_H
#define	CAMERA_H
class Camera {
public:
    Camera(Vec3f position, Vec3f target, Vec3f orientation = Vec3f(0, 1, 0)) :
    position(position), target(target), orientation(orientation) { }
    void look_at(Vec3f position) {
        this->target = position;
    }
    void set_position(Vec3f position) {
        this->position = position;
    }
    void move(Vec3f distance) {
        this->position += distance;
    }
    Vec3f get_position() const {
        return position;
    }
    void rotate(float radians, Vec3f axis) {
        glm::mat4 rotation_matrix = glm::rotate(radians, glm::vec3(axis.x, axis.y, axis.z));
        glm::vec4 tmp_orientation = rotation_matrix * glm::vec4(orientation.x, orientation.y, orientation.z, 1);
        orientation = Vec3f(tmp_orientation.x, tmp_orientation.y, tmp_orientation.z);
    }
    glm::mat4 get_view() const {
        return glm::lookAt(glm::vec3(position.x, position.y, position.z),
                           glm::vec3(target.x, target.y, target.z),
                           glm::vec3(orientation.x, orientation.y, orientation.z));
    }
private:
    Vec3f target;
    Vec3f position;
    Vec3f orientation;
};

#endif	/* CAMERA_H */

