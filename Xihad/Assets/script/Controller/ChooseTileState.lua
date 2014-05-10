local base = require 'Controller.PlayerState'

local ChooseTileState = setmetatable({
	reachableHandle = nil,
	attackableHandle= nil,
	prevSourceLocation = nil,
	lastTile = nil,
}, base)
ChooseTileState.__index = ChooseTileState

function ChooseTileState.new(...)
	local o = setmetatable(base.new(...), ChooseTileState)
	
	o.translateModifier = o.camera:lockedMove(nil, 0)
	o.translateModifier:setEnabled(false)
	
	return o
end

function ChooseTileState:onStateEnter(state, prev)
	local warrior = self.commandList:getSource()
	if prev == 'ChooseCommand' then
		assert(self.prevSourceLocation)
		local warriorBarrier = warrior:findPeer(c'Barrier')
		local tile = g_chessboard:getTile(self.prevSourceLocation)
		
		assert(tile)
		warriorBarrier:setTile(tile)
	end
	
	self.prevSourceLocation = nil
	
	local reachable = g_chessboard:getReachableTiles(warrior)
	self.reachableHandle = self.painter:mark(reachable, 'Reachable')
	self.camera:focus(warrior:getHostObject())
end

function ChooseTileState:onStateExit()
	self:_safeClear('reachableHandle')
	self:_safeClear('attackableHandle')
end

function ChooseTileState:onBack()
	self.camera:focus(nil)
	return 'back'
end

function ChooseTileState:needCDWhenTouch()
	return false
end

function ChooseTileState:needCDWhenHover()
	return false
end

function ChooseTileState:onTileSelected(tile)
	-- show tile info
	local warrior = self.commandList.source
	if tile:canStay(warrior) then

		local destLocation= tile:getLocation()
		if not g_chessboard:canReach(warrior, destLocation) then
			print('can not reach')
		else
			local destHandle = self.painter:mark({ tile }, 'Destination')
			
			self.prevSourceLocation = warrior:getLocation()
			self.commandList:setLocation(destLocation)
			self.executor:move(warrior:getHostObject(), destLocation)
			self.painter:clear(destHandle)
			return 'next'
		end
	else
		self.ui:warning('not stayable')
	end
end

local function toScreenCoord(pos)
	local x, y = g_collision:getScreenCoordFromPosition(pos)
	return math3d.vector(x, y, 0)
end

function ChooseTileState:onTileHovered(tile)
	if self.lastTile ~= tile then
		local warrior = self.commandList:getSource()
		
		if g_chessboard:canReach(warrior, tile:getLocation()) then
			self.ui:showTileInfo(tile)
			
			self:_safeClear('attackableHandle')
			
			local skillCaster = warrior:findPeer(c'SkillCaster')
			local range = skillCaster:getCastableTiles(tile:getLocation())
			self.attackableHandle = self.painter:mark(range, 'Attack')
			
			local leftBottom = toScreenCoord(tile:getLeftBottomVector())
			local rightBottom= toScreenCoord(tile:getRightBottomVector())
			local leftTop = toScreenCoord(tile:getLeftTopVector())
			local rightTop= toScreenCoord(tile:getRightTopVector())
			
			local lowerTri = math3d.triangle(leftBottom, rightBottom, leftTop)
			local upperTri = math3d.triangle(rightBottom, rightTop, leftTop)
			
			local area = lowerTri:getArea() + upperTri:getArea()
			if area > tile:getArea()*30 then
				print('enough', area)
				self.translateModifier:setTarget(nil)
			else
				print('too small')
				self.translateModifier:setSpeed(60)
				self.translateModifier:setTarget(tile:getCenterVector())
				self.translateModifier:setEnabled(true)
				return
			end
			-- local width, height = g_window:getScreenSize()
			-- local widthRatio, heightRatio = 0.1, 0.1
			-- local lower = math3d.vector(width*widthRatio, height*heightRatio, 0)
			-- local upper = math3d.vector(width*(1 - 2*widthRatio), height*(1- 2*heightRatio), 0)
			-- local aabb = math3d.aabb(lower)
			-- aabb:addPoint(upper)
			
			-- if not aabb:contains(leftBottom) then
			-- 	print('LB out')
			-- elseif not aabb:contains(rightBottom) then
			-- 	print('RB out')
			-- elseif not aabb:contains(leftTop) then
			-- 	print('LT out')
			-- elseif not aabb:contains(rightTop) then
			-- 	print('RT out')
			-- else
			-- 	return
			-- end
			
			-- self.translateModifier:setSpeed(90)
			-- self.translateModifier:setTarget(tile:getCenterVector())
			-- self.translateModifier:setEnabled(true)
		else
			self:_safeClear('attackableHandle')
		end
		
		self.lastTile = tile
	end
end

return ChooseTileState
