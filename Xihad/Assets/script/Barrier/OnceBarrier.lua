local base = require 'Barrier.SurveyedBarrier'
local OnceBarrier = setmetatable({}, base)
OnceBarrier.__index = OnceBarrier

function OnceBarrier.new(surveyType)
	return setmetatable(base.new(surveyType), OnceBarrier)
end

function OnceBarrier:_canSurvey(warrior, surveyedTimes)
	return surveyedTimes == 0
end

return OnceBarrier