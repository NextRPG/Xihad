local MouseEventDispatcher = { 
	performCollisionDetection = true,
	listener = nil,
}
MouseEventDispatcher.__index = MouseEventDispatcher

function MouseEventDispatcher.new(listener)
	local obj = setmetatable({
			listener = listener,
		}, MouseEventDispatcher)
	return obj
end

function MouseEventDispatcher:dispatch(x, y)
	local ray = g_collision:getRayFromScreenCoord(x, y)
	
	if self.performCollisionDetection then
		local obj = g_collision:detect()
	
		if obj and obj:hasTag(c'Warrior') then
			return self.listener:onWarrior(obj)
		end
	end
	
	local ground = math3d.plane(math3d.vector(), math3d.vector(0, 1, 0))
	local met, ratio = math3d.intersects(ray, ground)
	
	local intersection = ray:getStart() + ray:vector()*ratio
	local location = XihadMapTile.projectToLocation(intersection)
	if tile then
		-- Test if there is any object on it
		self.listener:onTile()
	end
end

function MouseEventDispatcher:onTileDefault(tile)
	-- if there is a person standing on the tile, then invoke onPersonSelected()
	-- else invoke listener:onVacancy()
end

function MouseEventDispatcher:onWarriorDefault(warriorObj)
	if warriorObj:hasTag(c'Hero') then
		return self.listener:onHero(warriorObj)
	elseif warriorObj:hasTag(c'Enemy') then
		return self.listener:onEnemy(warriorObj)
	else
		print('Non-hero and Non-AI person found')
	end
end

return MouseEventDispatcher