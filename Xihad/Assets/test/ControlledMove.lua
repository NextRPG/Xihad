local ControlledMove = { ydir = 0, dontMove = true, speed = 10 }
ControlledMove.__index = ControlledMove
local uiHandle = require("GUIHandle")

function ControlledMove.new( moveParam, object )
	local o = {}
	setmetatable(o, ControlledMove)
	
	print("Controlled object's ID: ", object:getID())
	if moveParam then o.speed = moveParam.speed end
	return o
end

function ControlledMove:setTarget( ndir )
	if ndir.x == 0 and ndir.z == 0 then
		self.dontMove = true
		return
	else
		self.dontMove = false
	end

	-- (-180, 180]
	self.ydir = math.atan2(ndir.x, ndir.z) / math.pi * 180	
end

function ControlledMove:onUpdate()
	local rot = self.object:getRotation()
	local x,y,z = rot:xyz()
	y = y % 360
	y = y>180 and y - 360 or (y <= -180 and y + 360 or y)
	
	if self.ydir ~= y then
		local target = self.ydir
		local dy = 0
		local max = Time.change * 720
		if target-y > 180  then target = target - 360 end
		if target-y < -180 then target = target + 360 end

		if target > y then
			dy = self.ydir-y > max and  max or self.ydir-y
		else
			dy = y-self.ydir > max and -max or self.ydir-y
		end

		self.object:concatRotate(math3d.vector(0, dy, 0))
	end

	if not self.dontMove then
		local rad = self.ydir/180*math.pi
		local dst = Time.change*self.speed
		self.object:concatTranslate(math.sin(rad)*dst, 0, math.cos(rad)*dst)
	end
end

function ControlledMove:acquire()
	local anim = self.object:findComponent(c"AnimatedMesh")
	local move = self
	
	local controller = { status = "idle 1", direction = { x = 0, z = 0 } }
	function controller:updateMotion()
		move:setTarget(self.direction)

		if self.direction.x ~= 0 or self.direction.z ~= 0 then
			self.next = "walk"
		else
			self.next = "idle 1"
		end

		if self.status ~= "jump" and self.next ~= self.status then
			self.status = self.next
			anim:playAnimation(c(self.status))
			self.next = nil
		end
	end

	function controller:onKeyDown(e)
		-- if is jumping then do nothing
		if e.key == "SPACE" and self.status ~= "jump" then
			self.status = "jump"
			self.next = "idle 1"
			anim:playAnimation(c"jump", function()
				self.status = self.next
				self.next = nil
				anim:playAnimation(c(self.status))
			end)
			return true
		end

		if e.key == "UP" then
			self.direction.z = 1
		elseif e.key == "LEFT" then
			self.direction.x = -1
		elseif e.key == "RIGHT" then
			self.direction.x = 1
		elseif e.key == "DOWN" then
			self.direction.z = -1
		end
		-- Test cegui
		if e.key == "P" then
			uiHandle:subscribeEvent("CommandSelected", function (args)
				print("!!!")
			end)
		elseif e.key == "J" then
			uiHandle:showWindow("CommandWindow", 
			{ 
				["技能"] = { shortcut = "A", list = { ["技能1"] = true , ["技能2"] = false } }, 
				["道具"] = { list = { ["道具1"] = true} }, 
				["待机"] = { disabled = true } 
			})
			

		elseif e.key == "Q" then
			uiHandle:hideWindow("CommandWindow")
		elseif e.key == "I" then
			damageNumber = damageNumber or 0
			uiHandle:showWindow("AttackDamage", { damage = damageNumber})
			damageNumber = (damageNumber + 5)%1000	
		end
		--
		self:updateMotion()
		return true
	end 

	function controller:onKeyUp( e )
		if e.key == "UP" then
			self.direction.z = 0
		elseif e.key == "LEFT" then
			self.direction.x = 0
		elseif e.key == "RIGHT" then
			self.direction.x = 0
		elseif e.key == "DOWN" then
			self.direction.z = 0
		end

		self:updateMotion()
		return true
	end

	self.object:getScene():pushController(controller)
end

return ControlledMove