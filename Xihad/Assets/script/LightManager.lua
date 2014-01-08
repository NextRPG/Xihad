local LightManager = {

}

function LightManager:init(  )
	local sun = scene:createObject(c"sun")
	lightComp = sun:appendComponent(c"Light")
	lightComp:castShadow(true)
	sun:concatTranslate(0, 50, -5)
end

return LightManager