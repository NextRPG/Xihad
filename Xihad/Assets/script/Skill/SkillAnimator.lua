local SkillAnimator = { }
SkillAnimator.__index = SkillAnimator

function SkillAnimator.new()
	return setmetatable({ }, SkillAnimator)
end

function SkillAnimator:animate(sourceWarrior, targetTile)
	local sourceAnimatable = sourceWarrior:findPeer(c'AnimatedMesh')
	if not sourceAnimatable then
		error('invalid sourceWarrior without AnimatedMesh')
	end
	
	local targetWarrior = targetTile:getWarrior()
	if targetWarrior then
		-- TODO
	end
	
	local terrian = targetTile:getTerrian()
end

return SkillAnimator