local base = require 'Barrier'
local SurveyedBarrier = setmetatable({
		surveyType = nil,
		surveyedTimes = 0,
	}, base)
SurveyedBarrier.__index= SurveyedBarrier

function SurveyedBarrier.new(surveyType)
	assert(type(surveyType) == 'string')
	local o = setmetatable(base.new(), SurveyedBarrier)
	o.surveyType = surveyType
	return o
end

function SurveyedBarrier:_canSurvey(warrior, surveyedTimes)
	return true
end

function SurveyedBarrier:getSurveyType(warrior)
	if self:_canSurvey(warrior, self.surveyedTimes) then
		return self.surveyType
	end
end

function SurveyedBarrier:_onSurveyed(warrior, times)
	error('no implementation by default')
end

function SurveyedBarrier:onSurveyed(warrior)
	local success, msg = self:_onSurveyed(warrior, self.surveyedTimes)
	assert(type(success) == 'boolean')
	if success then
		self.surveyedTimes = self.surveyedTimes + 1
	end
	
	return success, msg
end

return SurveyedBarrier