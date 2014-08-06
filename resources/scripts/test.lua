package.path = ";./resources/scripts/?.lua;" .. package.path
local Interface = require "Interface"


-- create the interface object
ui = Interface {
    background = "interface_bg.png",
    position = {0, 480-40},
    size = {640, 40}
}

--  Elements
rotate_cube_button = ui:Element {
    name = "the button",
    position = {320-32, 480-20-16},
    size = {64, 32},
    image = "turn_button_idle.png",
    image_pressed = "turn_button_pressed.png"
}

-- Callbacks
rotate_cube_button:on ("click", function()
    Engine.Scene.toggle_cube_rotation_direction(-1)
end)

-- return the ui
return ui
