
function math.round( i )
	return i - math.floor(i) > 0.5 and math.ceil(i) or math.floor(i)
end

function math.p_add( p1, p2 )
	return {x = p1.x + p2.x, y = p1.y + p2.y}
end

function math.p_sub( p1, p2 )
	return {x = p1.x - p2.x, y = p1.y - p2.y}
end

function math.p_distance( p1, p2 )
	return math.abs(p1.x - p2.x) + math.abs(p1.y - p2.y)
end

function math.p_same( p1, p2 )
	return p1.x == p2.x and p1.y == p2.y
end

function point( _x, _y )
	return {x = _x, y = _y}
end