local SRPGCamera = {
	cameraObject = nil,
	cameraControl= nil,
}
SRPGCamera.__index = SRPGCamera

function SRPGCamera.new(id)
	local o = setmetatable({}, SRPGCamera)
	o.cameraObject = g_scene:createObject(c(id))
	if not o.cameraObject then
		error('Failed to create camera object with the specified name')
	end
	
	o.cameraControl= o.cameraObject:appendComponent(c"Camera")
	o.cameraObject:resetTranslate(math3d.vector(0, 30, 0))
	o.cameraControl:setUpVector(math3d.vector(0, 1, 0))
	
	-- TODO DELETE
	-- o.cameraControl:setLookDirection(math3d.vector(0, 0, 1))
	o.cameraControl:setTarget(math3d.vector(35, 10, 25))
	
	return o
end

function SRPGCamera:focus()
	
end

function SRPGCamera:follow()
	
end

-- function SRPGCamera:...()
-- end

return SRPGCamera