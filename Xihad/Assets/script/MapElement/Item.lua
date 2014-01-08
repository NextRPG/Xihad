Item = {
	type = ""
}

function Item:new( o )
	o = o or {}

	setmetatable(o, self)
	self.__index = self
	return o
end

function Item:permitCasting( charater, skill )
	return true
end

function Item:canPass( charater )
	return true
end

function Item:canStay( charater )
	return true
end

function Item:getActionPointIncr( charater )
	return true
end

function Item:getActionPointCost(  )
	return 0
end
