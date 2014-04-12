local camMovement = {
}

function camMovement.parallelMove(cameraObject, finishPosition)
	local cameraControl = cameraObject:findComponent(c'Camera')
	cameraControl:setTargetFixed(false)
	
	ObjectAction.move(cameraObject, nil, finishPosition)
end

function camMovement.moveToCharacter(cameraObject, character)
	local finishPosition = character:getTranslate()
	camMovement.moveToPosition(cameraObject, finishPosition)
end

function camMovement.moveToBattle(cameraObject, character, tile)
	local finishPosition = (character:getTranslate() + point2vector(tile)) * 0.5 + math3d.vector(0, 20 ,0)
	local x1, y1 = (point2vector(tile) - characterObject:getTranslate()):xyz()
	local x2 = math.sqrt(900 / (1 + (x1 * x1) / (y1 * y1)))
	local y2 = math.sqrt(900 - x2 * x2)
	local translate = action.destTarget + math3d.vector( - x2, 10, - y2)
	backAction = {destination = self.cameraObject:getTranslate(), destTarget = ccom:getTarget()}
end

function camMovement.backToNormal()
	
end


