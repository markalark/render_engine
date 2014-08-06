/*
 * File:   Interface.h
 * Author: Markalark
 *
 */

#ifndef INTERFACE_H
#define	INTERFACE_H
#include <queue>
#include <lua.hpp>
#include "LuaUtils.h"
#include "Engine.h"
#include "Inputable.h"
#include "Element.h"

//
class Interface : public Inputable {
public:
    Interface(std::string name) : name(name) { }
    static Interface *load(std::string lua_file);
    const std::vector<Element *> &get_elements() {
        return elements;
    }
    Element *create_element(std::string name, Vec2f position, Vec2f size) {
        Element *element = new Element(name, position, size);
        elements.push_back(element);
        unprepared_elements.push(element);
        return element;
    }
    bool has_unprepared_elements() {
        return !unprepared_elements.empty();
    }
    Element *next_unprepared_element() {
        Element *element = nullptr;
        if (!unprepared_elements.empty()) {
            element = unprepared_elements.front();
            unprepared_elements.pop();
        }
        return element;
    }
private:
    void on(Element *target, Element::Event event, EventCallback callback);
    void listen_for(Element *target, Element::Event event);
    std::vector<Element *> elements;
    std::string name;

    // callbacks
    std::vector<EventCallback> event_callbacks[4];
    std::queue<Element *> unprepared_elements;

public:
    // ================== Lua Interface ======================//
    static std::vector<luaL_Reg> lua_class_functions;
    static std::vector<luaL_Reg> lua_member_functions;
    static int new_interface(lua_State *L);
    static int new_element(lua_State *L);
    static int add_element_callback(lua_State *L);
};

#endif	/* INTERFACE_H */

