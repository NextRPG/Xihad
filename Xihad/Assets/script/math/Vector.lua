local Vector = {}

function Vector.getX(vec)
	return (vec:xyz())
end

function Vector.getY(vec)
	return (select(2, vec:xyz()))
end

function Vector.getZ(vec)
	return select(3, vec:xyz())
end

return Vector