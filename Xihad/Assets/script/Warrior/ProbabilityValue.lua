local ProbabilityValue = {
	values = nil,
	probabilities = nil,
}
ProbabilityValue.__index = ProbabilityValue

function ProbabilityValue.new()
	return setmetatable({
			values = {},
			probabilities = {},
		}, ProbabilityValue)
end

function ProbabilityValue:empty()
	return #self.values > 0
end

function ProbabilityValue:addValue(probability, value)
	if probability > 0 and value ~= 0 then
		table.insert(self.values, value)
		table.insert(self.probabilities, probability)
	end
end

function ProbabilityValue:_accum_if(f)
	local values, probabilities = self.values, self.probabilities
	local total = 0
	for i = 1, #values do
		local value, probability = values[i], probabilities[i]
		if f(value, probability) then
			total = total + value
		end
	end
	
	return total
end

function ProbabilityValue:getMin()
	return self:_accum_if(function(value, probability)
			return value > 0 and probability >= 1 
				or value < 0
		end)
end

function ProbabilityValue:getMax()
	return self:_accum_if(function(value, probability)
			return value < 0 and probability >= 1 
				or value > 0
		end)
end

function ProbabilityValue:random()
	return self:_accum_if(function(value, probability)
			return probability >= 1 
				or math.random() < probability
		end)
end

return ProbabilityValue