local camMovement = {
}

function camMovement.moveToPosition(cameraObject, translate, target)
	
end

function camMovement.moveToCharacter(cameraObject, character, lookDir)
	local target = character:getTranslate()
	camMovement.moveToPosition(cameraObject, target-lookDir, target)
end

function camMovement.moveToBattle(cameraObject, character, tile)
	local target = (character:getTranslate() + point2vector(tile)) * 0.5 + math3d.vector(0, 20 ,0)
	local x1, y1 = (point2vector(tile) - characterObject:getTranslate()):xyz()
	local x2 = math.sqrt(900 / (1 + (x1 * x1) / (y1 * y1)))
	local y2 = math.sqrt(900 - x2 * x2)
	local translate = action.destTarget + math3d.vector( - x2, 10, - y2)
	backAction = {destination = self.cameraObject:getTranslate(), destTarget = ccom:getTarget()}
end

function camMovement.backToNormal()
	
end


