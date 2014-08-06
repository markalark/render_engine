local Element = require "Element"

local function constructor (options)
    local interface = Engine.Interface.new(options.name or "Untitled")
    interface.Element = Element
    interface:Element {
        position = options.position,
        size = options.size,
        image = options.background
    }
    return interface
end

return constructor