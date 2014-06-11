local BarrierUtil = {}

function BarrierUtil.synchronizeTranslate(barrier)
	local object = barrier:getHostObject()
	local translate = barrier:getTile():getCenterVector()
	object:resetTranslate(translate)
end

return BarrierUtil
