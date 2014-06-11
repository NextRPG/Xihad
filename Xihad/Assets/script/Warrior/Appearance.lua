local Apperance = {
	icon = nil,
	model= nil,
}
Apperance.__index = Apperance

function Apperance.new(data, object)
	return setmetatable({
			icon = data.icon,
			model= data.model,
		}, Apperance)
end

return Apperance