local Table = {}

function Table.keys(t)
	local f, s, v = pairs(self.skills)
	local wrapped = function ()
		return (f(s, v))
	end
	
	return wrapped, s, v
end

return Table