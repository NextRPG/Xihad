local Trigonometry = require 'math.Trigonometry'
local SpanAction   = require 'Action.SpanVariableAction'

local RenderAction = {}

local function getColorSetter(render, funcName)
	return function(color) 
		for _, mat in render:materials() do
			mat[funcName](mat, color)
		end
	end
end

function RenderAction._colorAction(funcName, render, spanColor, duration, lerp)
	return SpanAction.new(spanColor, getColorSetter(render, funcName), duration, lerp)
end

function RenderAction.ambient(render, spanColor, duration, lerp)
	spanColor.origin = spanColor.origin or render:getMaterial(0):getAmbientColor()
	return RenderAction._colorAction('setAmbientColor', render, spanColor, duration, lerp)
end

function RenderAction.diffuse(render, spanColor, duration, lerp)
	spanColor.origin = spanColor.origin or render:getMaterial(0):getDiffuseColor()
	return RenderAction._colorAction('setDiffuseColor', render, spanColor, duration, lerp)
end

function RenderAction.specular(render, spanColor, duration, lerp)
	spanColor.origin = spanColor.origin or render:getMaterial(0):getSpecularColor()
	return RenderAction._colorAction('setSpecularColor', render, spanColor, duration, lerp)
end

function RenderAction.emssive(render, spanColor, duration, lerp)
	spanColor.origin = spanColor.origin or render:getMaterial(0):getEmissiveColor()
	return RenderAction._colorAction('setEmissiveColor', render, spanColor, duration, lerp)
end

return RenderAction
