local GradeSelector = {}

function GradeSelector.select(iter, grader)
	local highScore, bestValue = nil, nil
	for value in iter() do
		local newScore, enough = grader(value)
		if highScore and highScore < newScore or newScore >= 0 then
			highScore = newScore
			bestValue = value
		end
		
		if enough then break end
	end
	
	return bestValue
end

return GradeSelector
