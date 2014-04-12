local LightManager = {

}

function LightManager:init(  )
	local sun = g_scene:createObject(c"sun")
	lightComp = sun:appendComponent(c"Light")
	lightComp:castShadow(false)
	sun:concatTranslate(math3d.vector(0, 50, -5))
end

return LightManager