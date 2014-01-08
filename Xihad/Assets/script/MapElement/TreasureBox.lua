TreasureBox = {
	type = ""
}

function TreasureBox:new( o )
	o = o or {}

	setmetatable(o, self)
	self.__index = self
	return o
end

function TreasureBox:permitCasting( charater, skill )
	return true
end

function TreasureBox:canPass( charater )
	return true
end

function TreasureBox:canStay( charater )
	return true
end

function TreasureBox:getActionPointIncr( charater )
	return true
end

function TreasureBox:getActionPointCost(  )
	return 0
end
