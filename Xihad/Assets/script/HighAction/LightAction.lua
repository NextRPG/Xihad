local functional = require 'std.functional'
local SpanAction = require 'Action.SpanVariableAction'
local LightAction = {}

function LightAction._colorAction(funcName, light, spanColor, duration, lerp)
	return SpanAction.new(spanColor, functional.bindself(light, funcName), duration, lerp)
end

function LightAction.ambient(light, spanColor, duration, lerp)
	spanColor.origin = spanColor.origin or light:getAmbientColor()
	return LightAction._colorAction('setAmbientColor', light, spanColor, duration, lerp)
end

function LightAction.diffuse(light, spanColor, duration, lerp)
	spanColor.origin = spanColor.origin or light:getDiffuseColor()
	return LightAction._colorAction('setDiffuseColor', light, spanColor, duration, lerp)
end

function LightAction.specular(light, spanColor, duration, lerp)
	spanColor.origin = spanColor.origin or light:getSpecularColor()
	return LightAction._colorAction('setSpecularColor', light, spanColor, duration, lerp)
end

return LightAction