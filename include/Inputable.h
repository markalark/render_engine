/*
 * File:   Inputable.h
 * Author: Markalark
 *
 * Created on July 12, 2014, 10:22 PM
 */

#ifndef INPUTABLE_H
#define	INPUTABLE_H
#include <functional>
#include "Vec2.h"
//
typedef std::function<void(Vec2f) > MouseMoveCallback;
typedef std::function<void(Vec2f) > InputCallback;
class Inputable {
public:
    void on_mouse_move(MouseMoveCallback callback) {
        mouse_move_callbacks.push_back(callback);
    }
    void on_input(int key, int action, InputCallback callback) {
        while (key >= input_callbacks.size()) {
            input_callbacks.push_back(std::vector<std::vector<InputCallback> >());
        }
        while (action >= input_callbacks[key].size()) {
            input_callbacks[key].push_back(std::vector<InputCallback>());
        }
        input_callbacks[key][action].push_back(callback);
    }
    void on_next_input(int key, int action, InputCallback callback) {
        while (key >= next_input_callbacks.size()) {
            next_input_callbacks.push_back(std::vector<std::queue<InputCallback> >());
        }
        while (action >= next_input_callbacks[key].size()) {
            next_input_callbacks[key].push_back(std::queue<InputCallback>());
        }
        next_input_callbacks[key][action].push(callback);
    }
    void handle_input(int key, int action, int mods, Vec2f mouse_position) {
        for (auto callback : input_callbacks[key][action]) {
            callback(mouse_position);
        }
        if (key < next_input_callbacks.size() && action < next_input_callbacks[key].size()) {
            while (!next_input_callbacks[key][action].empty()) {
                InputCallback callback = next_input_callbacks[key][action].front();
                next_input_callbacks[key][action].pop();
                callback(mouse_position);
            }
        }
    }
private:
    std::vector<MouseMoveCallback> mouse_move_callbacks;
    // indexed by the key and action of the input: callbacks[key][action]
    std::vector<std::vector<std::vector<InputCallback> > > input_callbacks;
    std::vector<std::vector<std::queue<InputCallback> > > next_input_callbacks;
};

#endif	/* INPUTABLE_H */

