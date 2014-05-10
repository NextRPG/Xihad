local Trigonometry = require 'math.Trigonometry'
local SpanAction   = require 'Action.SpanVariableAction'

local RenderAction = {}

local function interpolateColor(span, d)
	local color1, color2 = span.origin, span.finish
	
	local alpha = color1:getAlpha() * (1-d) + color2:getAlpha() * d
	local red 	= color1:getRed() 	* (1-d) + color2:getRed() 	* d
	local green = color1:getGreen() * (1-d) + color2:getGreen() * d
	local blue 	= color1:getBlue() 	* (1-d) + color2:getBlue() 	* d
	
	return Color.new(alpha, red, green, blue)
end

local function getColorSetter(render, funcName)
	return function(color) 
		for _, mat in render:materials() do
			mat[funcName](mat, color)
		end
	end
end

function RenderAction._colorAction(funcName, render, spanColor, duration, lerp)
	spanColor.interpolate = interpolateColor
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
