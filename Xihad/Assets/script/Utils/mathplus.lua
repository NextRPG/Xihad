
function math.round( i )
	return i - math.floor(i) > 0.5 and math.ceil(i) or math.floor(i)
end

function math.p_add( p1, p2 )
	
	return {x = p1.x + p2.x, y = p1.y + p2.y}
end