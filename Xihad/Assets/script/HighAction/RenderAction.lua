local Trigonometry = require 'math.Trigonometry'
local SpanAction   = require 'Action.SpanVariableAction'

local RenderActoin = {}

-- TODO
function RenderActoin.color(render, spanColor, duration, lerp)
	-- spanColor.origin = spanColor.origin or object:getTranslate()
	local sync = function(color) 
		for _, mat in render:materials() do
			mat:setDiffuseColor(color)
		end
	end
	
	return SpanAction.new(spanColor, sync, duration, lerp)
end

return RenderActoin
