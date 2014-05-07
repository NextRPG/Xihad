local ControlledMove = { ydir = 0, dontMove = true, speed = 10 }
ControlledMove.__index = ControlledMove

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
	local object = self:getHostObject()
	local rot = object:getRotation()
	local x,y,z = rot:xyz()
	y = y % 360
	y = y>180 and y - 360 or (y <= -180 and y + 360 or y) -- (-180, 180]
	
	if self.ydir ~= y then
		local target = self.ydir
		local dy = 0
		local max = g_time.change * 720
		
		-- 旋转小角度
		if target-y > 180  then target = target - 360 end
		if target-y < -180 then target = target + 360 end

		dy = target-y
		dy = dy>max and max or (dy<-max and -max or dy)
		
		object:concatRotate(math3d.vector(0, dy, 0))
	end
	
	if not self.dontMove then
		local rad = self.ydir/180*math.pi
		local dst = g_time.change*self.speed
		object:concatTranslate(math3d.vector(math.sin(rad)*dst, 0, math.cos(rad)*dst))
	end
end

function ControlledMove:acquire()
	local anim = self:getHostObject():findComponent(c"AnimatedMesh")
	local move = self
	
	local controller = { status = "idle", direction = { x = 0, z = 0 } }
	function controller:updateMotion()
		move:setTarget(self.direction)

		if self.direction.x ~= 0 or self.direction.z ~= 0 then
			self.next = "walk"
		else
			self.next = "idle"
		end

		if self.status ~= "magic1" and self.next ~= self.status then
			self.status = self.next
			if anim then anim:playAnimation(c(self.status)) end
			self.next = nil
		end
	end

	function controller:onKeyDown(e)
		-- if is jumping then do nothing
		local magic = 'magic2'
		if e.key == "SPACE" and self.status ~= magic then
			self.status = magic
			self.next = "idle"
			if anim then
				anim:playAnimation(c(magic), function()
					self.status = self.next
					self.next = nil
					anim:playAnimation(c(self.status))
				end)
			end
			return 0
		end

		if e.key == "UP" then
			self.direction.z = 1
		elseif e.key == "LEFT" then
			self.direction.x = -1
		elseif e.key == "RIGHT" then
			self.direction.x = 1
		elseif e.key == "DOWN" then
			self.direction.z = -1
		else
			print("no handler") io.flush()
			return 1
		end
		
		self:updateMotion()
		return 0
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
		else
			return 1
		end

		self:updateMotion()
		return 0
	end

	self.object:getScene():pushController(controller)
	controller:drop()
end

return ControlledMove