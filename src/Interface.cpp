#include "gl_headers.h"
#include "Interface.h"
#include "Rectangle.h"

//
Interface *Interface::load(std::string lua_file) {
    lua_State *L = Engine::get_instance()->get_lua_state();
    int error = luaL_dofile(L, ("resources/scripts/" + lua_file).c_str());
    if (error) {
        printf("%s\n", lua_tostring(L, -1));
    }
    Interface *interface = (Interface *) Lua::get_data(L, 1);
    lua_pop(L, 1);
    return interface;
}

//
void Interface::on(Element *target, Element::Event event, EventCallback callback) {
    // if this is the element's first time registering a callback for this event...
    if (!target->has_callback(event)) {
        // set the interface to listen for these events on this element
        listen_for(target, event);
    }
    target->add_callback(event, callback);
}

//
void Interface::listen_for(Element *target, Element::Event event) {
    switch (event) {
        case Element::PRESS:
            /** an element is 'pressed' if the left mouse button is pressed on top of it */
            on_input(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, [this, target](Vec2f mouse_position) {
                // check if it collided with the element
                if (target->get_bounds().contains(mouse_position)) {
                    target->do_event(Element::PRESS);
                }
            });
            break;

        case Element::RELEASE:
            /** after being pressed, an element can be 'released' by letting go of
             * the mouse button from anywhere on the screen. */
            on(target, Element::PRESS, [this, target]() {
                // listen for the mouse button to be released
                on_next_input(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, [target](Vec2f mouse_position) {
                    target->do_event(Element::RELEASE);
                });
            });
            break;

        case Element::CLICK:
            /** register a 'click' if an element is released on top of itself. */
            on(target, Element::PRESS, [this, target]() {
                on_next_input(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, [target](Vec2f mouse_position) {
                    // check if it collided with the element
                    if (target->get_bounds().contains(mouse_position)) {
                        target->do_event(Element::CLICK);
                    }
                });
            });
            break;
    }
}

// ------------ Lua Interface ------------------//
std::vector<luaL_Reg> Interface::lua_class_functions = {
    {"new", Interface::new_interface},
    {NULL, NULL}
};

std::vector<luaL_Reg> Interface::lua_member_functions = {
    {"create_element", Interface::new_element},
    {"add_element_callback", Interface::add_element_callback},
    {NULL, NULL}
};

// making and interface
int Interface::new_interface(lua_State *L) {
    // number of arguments (size of stack passed in)
    int n = lua_gettop(L);
    // verify size
    if (n != 1) {
        return luaL_error(L, "Got %d arguments expected 1 (name)", n);
    }

    // arg 1: name
    std::string name = luaL_checkstring(L, 1);

    // create lua object
    Interface *interface = new Interface(name);
    Lua::create_object(L, "Engine.Interface", (void *) interface);

    // return the lua object (top of stack)
    return 1;
}

// making an element
int Interface::new_element(lua_State *L) {
    // number of arguments (size of stack)
    int n = lua_gettop(L);
    // verify size
    if (n != 4) {
        return luaL_error(L, "Got %d arguments expected 4 (self, name, position, size)", n);
    }

    // arg 1: 'self' (the interface instance)
    Interface *interface = (Interface *) Lua::get_data(L, 1);
    // arg 2: name
    std::string name = luaL_checkstring(L, 2);
    // arg 3: position
    Vec2f position = Lua::get_vec2f(L, 3);
    // arg 4: size
    Vec2f size = Lua::get_vec2f(L, 4);


    // create lua object
    Element *element = interface->create_element(name, position, size);
    Lua::create_object(L, "Engine.Interface.Element", (void *) element);

    // return the lua object
    return 1;
}
int Interface::add_element_callback(lua_State *L) {
    // arg 1: 'self' (the interface instance)
    Interface *interface = (Interface *) Lua::get_data(L, 1);
    // arg 2: the element
    Element *element = (Element *) Lua::get_data(L, 2);
    // arg 3: the event
    std::string event_name = luaL_checkstring(L, 3);
    Element::Event event;
    if (event_name == std::string("press")) event = Element::PRESS;
    if (event_name == std::string("release")) event = Element::RELEASE;
    if (event_name == std::string("click")) event = Element::CLICK;
    // arg 4: the callback
    Lua::Callback callback = Lua::Callback(L, 4);

    // do it
    interface->on(element, event, [callback]() {
        callback.call();
    });
}