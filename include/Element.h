#ifndef ELEMENT_H
#define	ELEMENT_H

#include <functional>
#include <vector>
#include <lua.hpp>
#include "Vec2.h"
#include "Attribute.h"
#include "Rectangle.h"
#include "Drawable.h"
class Texture;

typedef std::function<void(float) > UpdateCallback;
typedef std::function<void() > EventCallback;

//
class Element : public gl::Drawable {
public:
    Element(std::string name, Vec2f position, Vec2f size) :
    name(name), body(position, size) { }

    // events that can be done on an element
    enum Event {
        PRESS = 0,
        RELEASE,
        CLICK
    };

    // do every callback that has been registered under that event
    void do_event(Event event) {
        for (auto callback : callbacks[event]) {
            callback();
        }
    }

    // add a callback for the given event
    void add_callback(Event event, EventCallback callback) {
        // if the event is out of bounds of the array, add entries
        while (event >= callbacks.size()) {
            callbacks.push_back(std::vector<EventCallback>());
        }

        // add the callback to the array indexed by the event type
        callbacks[event].push_back(callback);
    }

    // check if the element has a callback for a given event
    bool has_callback(Event event) {
        return callbacks.size() > event && !callbacks[event].empty();
    }

    // set element properties
    void set_size(const Vec2f &size) {
        body.set_size(size);
    }
    void set_position(const Vec2f &position) {
        body.set_position(position);
    }
    void move(const Vec2f &distance) {
        body.set_position(body.get_position() + distance);
    }
    void set_texture(Texture *texture) {
        this->texture = texture;
    }
    std::string get_name() {
        return name;
    }
    Texture *get_texture() {
        return texture;
    }
    const Rectangle &get_bounds() const {
        return body;
    }
    Vec2f get_size() const {
        return body.get_size();
    }
    Vec2f get_position() const {
        return body.get_position();
    }

private:
    std::string name;
    Rectangle body;
    Texture *texture;
    // callbacks indexed by event type
    std::vector<std::vector<EventCallback> > callbacks;
    static gl::DataCopier create_element_copier(Element *element);

public:
    // ========== Lua Interface ================= //
    static std::vector<luaL_Reg> lua_class_functions;
    static std::vector<luaL_Reg> lua_member_functions;
    static int get_position(lua_State *L);
    static int get_size(lua_State *L);
    static int get_name(lua_State *L);
    static int set_position(lua_State *L);
    static int move(lua_State *L);
    static int set_size(lua_State *L);
    static int set_texture(lua_State *L);
};

#endif