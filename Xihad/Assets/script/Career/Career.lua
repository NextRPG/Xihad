local Career = {
	name = nil,
	grader = nil,
}
Career.__index = Career

function Career.new(name, grader)
	return setmetatable({
			name = name,
			grader = grader,
		}, Career)
end

function Career:getGrader()
	return self.grader
end

function Career:getName()
	return self.name
end

function Career:__tostring()
	return self:getName()
end

return Career