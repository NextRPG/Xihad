local SurveyedBarrier = require 'Barrier.SurveyedBarrier'
local SimpleBarrierFactory = {}

function SimpleBarrierFactory.treasure(item, count)
	return SimpleBarrierFactory.once('打开', 
		function (barrier, warrior, surveyedTimes)
			print('warrior got item: ', item:getName(), count)
			return true
		end)
end

function SimpleBarrierFactory.once(surveyType, trigger)
	local barrier = SurveyedBarrier.new(surveyType)
	
	function barrier:_canSurvey(warrior, surveyedTimes)
		return surveyedTimes == 0
	end
	
	barrier._onSurveyed = trigger
	return barrier
end

function SimpleBarrierFactory.always(surveyType, trigger)
	local barrier = SurveyedBarrier.new(surveyType)
	barrier._onSurveyed = trigger
	return barrier
end

return SimpleBarrierFactory
