local factory = {
}
factory.__index = factory

function factory.new(team)
	return setmetatable({}, factory)
end

function factory:create(team, name, data)
	local object = g_scene:createObject(c(name))
	
	-- Ability
	object:appendComponent(c'Ability', data)
	
	-- Barrier
	local barrier = object:appendComponent(c'WarriorBarrier', team)
	
	-- Animated mesh
	local param = { 
		mesh  = "Assets/model/".. data.model .. "/" .. data.model ..  ".b3d", 
		clips = "Assets/model/".. data.model .. "/" .. data.model ..  ".clip" 
	}
	local anim = object:appendComponent(c"AnimatedMesh", param)
	anim:createSelector(c"stupid") 
	anim:playAnimation(c"idle")
	
	object:concatTranslate(math3d.vector(0, 3, 0))
	object:resetScale(math3d.vector(1.5))
	
	object:addTag(c'Warrior')
	object:addTag(c(team))
	
	return object
end

return factory
