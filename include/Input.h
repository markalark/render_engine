#ifndef INPUT_H
#define	INPUT_H
#include "Vec2.h"

//
class Input {
public:
    Input(int key, int action, Vec2f mouse_position = {0, 0}) :
    key(key),
    action(action),
    mouse_position(mouse_position) { }

    //
    int key;
    int action;
    Vec2f mouse_position;

private:
};

#endif	/* INPUT_H */

