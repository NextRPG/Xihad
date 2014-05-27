local Grader = {}
Grader.__index = Grader

function Grader.whatever()
	return function ()
		return 1, true
	end
end

return Grader
