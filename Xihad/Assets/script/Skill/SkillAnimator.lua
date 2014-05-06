local SkillAnimator = { }
SkillAnimator.__index = SkillAnimator

function SkillAnimator.new()
	local obj = setmetatable({ }, SkillAnimator)
	return obj
end

function SkillAnimator:animate(sourceWarrior, targetTile)
	local amesh = sourceWarrior:findPeer(c'AnimatedMesh')
	if not amesh then
		error('invalid sourceWarrior without AnimatedMesh')
	end
	
	local targetWarrior = targetTile:getWarrior()
	if targetWarrior then
		-- TODO
	end
	
	local terrian = targetTile:getTerrian()
end

return SkillAnimator