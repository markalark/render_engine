
function constructor (interface, options)
    local element = interface:create_element(options.name or "Untitled", options.position, options.size)
    element.interface = interface
    element.textures = {}
    function element.on (self, event, callback)
        self.interface:add_element_callback(self, event, callback);
    end
    if (options.image) then
        element.textures.idle = Engine.TextureManager.get(options.image)
    end
    if (options.image_pressed) then
        element.textures.pressed = Engine.TextureManager.get(options.image_pressed)
        element:on ("press", function()
            element:set_texture(element.textures.pressed)
        end)
        element:on ("release", function()
            element:set_texture(element.textures.idle)
        end)
    end
    element:set_texture(element.textures.idle)
    return element
end

return constructor