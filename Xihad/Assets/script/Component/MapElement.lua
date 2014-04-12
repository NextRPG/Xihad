MapElement = {
	type = ""
}
package.loaded["MapElement"] = MapElement

function MapElement:new( o )
	o = o or {}

	setmetatable(o, self)
	self.__index = self
	return o
end

function MapElement:permitCasting( charater, skill )
	return true
end

function MapElement:canPass( charater )
	return true
end

function MapElement:canStay( charater )
	return true
end

function MapElement:getActionPointCost(  )
	return 0
end