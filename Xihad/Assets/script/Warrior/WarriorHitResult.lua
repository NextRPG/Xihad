local Class= require 'std.Class'
local Coordinate2D = require 'route.Coordinate2D'
local ProbabilityValue = require 'Warrior.ProbabilityValue'
local AsConditionFactory = require 'Async.AsConditionFactory'
local WarriorHitResult = {
	repelDistance = 0,
	recoveries = nil,
	damages = nil,
	effects = nil,
	
	random = math.random,
}
WarriorHitResult.__index = WarriorHitResult

function WarriorHitResult.new(source)
	return setmetatable({
			recoveries = ProbabilityValue.new(),
			damages = ProbabilityValue.new(),
			effects = {},
		}, WarriorHitResult)
end


------------------------------------------------------------------------------
-- Construct purpose begin
Class.delegate(WarriorHitResult, 'addDamage', 	 'damages', 'addValue')

Class.delegate(WarriorHitResult, 'addRecovery',	   'recoveries', 'addValue')

function WarriorHitResult:addSkillEffect(effect, probability)
	self.effects[effect] = probability
end

function WarriorHitResult:addRepel(dist)
	self.repelDistance = self.repelDistance + dist
end
-- Construct purpose end
------------------------------------------------------------------------------


------------------------------------------------------------------------------
-- Test functions begin
Class.delegate(WarriorHitResult, 'hasDamage',	 'damages', 'empty')
Class.delegate(WarriorHitResult, 'getMinDamage', 'damages', 'getMin')
Class.delegate(WarriorHitResult, 'getMaxDamage', 'damages', 'getMax')
Class.delegate(WarriorHitResult, 'randomDamage', 'damages', 'random')

Class.delegate(WarriorHitResult, 'hasRecovery',	   'recoveries', 'empty')
Class.delegate(WarriorHitResult, 'getMinRecovery', 'recoveries', 'getMin')
Class.delegate(WarriorHitResult, 'getMaxRecovery', 'recoveries', 'getMax')
Class.delegate(WarriorHitResult, 'randomRecovery', 'recoveries', 'random')

function WarriorHitResult:hasSkillEffect()
	return next(self.effects) ~= nil
end

function WarriorHitResult:canRepel()
	return self.repelDistance ~= 0
end

function WarriorHitResult:isValid()
	return self:hasDamage() 
		or self:hasRecovery()
		or self:canRepel() 
		or self:hasSkillEffect()
end
-- Test functions end
------------------------------------------------------------------------------


------------------------------------------------------------------------------
-- apply begin
function WarriorHitResult:_getNormalDamage(sourceWarrior, targetWarrior)
	return self:randomDamage()
end

function WarriorHitResult:_getHitRate(sourceWarrior, targetWarrior)
	return 1
end

function WarriorHitResult:_getKillRate(sourceWarrior, targetWarrior)
	return 0
end

function WarriorHitResult:_determineDamage(sourceWarrior, targetWarrior)
	local random = WarriorHitResult.random
	if random() >= self:_getHitRate(sourceWarrior, targetWarrior) then
		-- miss
		print('damage miss')
		return 1
	elseif random() < self:_getKillRate(sourceWarrior, targetWarrior) then
		-- TODO how to present kill
		-- self.sourceWarrior:launchKill()
		print('damage kill')
		return targetWarrior:getHitPoint()
	else-- normal
		print('damage normal')
		return self:_getNormalDamage(sourceWarrior, targetWarrior)
	end
end

function WarriorHitResult:_applyDamage(sourceWarrior, targetWarrior)
	return targetWarrior:takeDamage(
				self:_determineDamage(sourceWarrior, targetWarrior))
end

function WarriorHitResult:_applyRecovery(targetWarrior)
	return targetWarrior:takeRecovery(self:randomRecovery())
end

function WarriorHitResult:_applyEffect(targetWarrior)
	for effect, probability in pairs(self.effects) do
		if WarriorHitResult.random() < probability then
			effect:copy():bindExclusive(targetWarrior, 'skill')
		end
	end
end

function WarriorHitResult:_checkWarrior(targetBarrier)
	local targetWarrior = targetBarrier:findPeer(c'Warrior')
	assert(targetWarrior ~= nil)
	return targetWarrior
end

function WarriorHitResult:apply(sourceWarrior, targetBarrier, lis)
	local targetWarrior = self:_checkWarrior(targetBarrier)
	
	if self:hasDamage() then
		local damage = self:_applyDamage(sourceWarrior, targetWarrior)
		lis:onResult(targetBarrier, 'damage', damage)
		
		if targetWarrior:isDead() then
			lis:onResult(targetBarrier, 'dead', true)
		end
	end
	
	if self:hasRecovery() then
		local recovery = self:_applyRecovery(targetWarrior)
		lis:onResult(targetBarrier, 'recovery', recovery)
	end
	
	if self:hasSkillEffect() then
		self:_applyEffect(targetWarrior)
	end
end
-- apply end
------------------------------------------------------------------------------


function WarriorHitResult:onHitBegin(sourceWarrior, targetBarrier)
	if self:hasDamage() then
		targetBarrier:findPeer(c'AnimatedMesh'):playAnimation(c'hit', false)
	end
	
	if self:canRepel() then
		local targetWarrior = self:_checkWarrior(targetBarrier)
		local currentLoc = targetWarrior:getLocation()
		local dir = Coordinate2D.build8Directions_YAxis(
						currentLoc - sourceWarrior:getLocation())
		
		local stopLoc, stopDst = currentLoc:copy(), 0
		repeat
			stopDst = stopDst + 1
			stopLoc:add(dir)
			
			local stopTile = g_chessboard:getTile(stopLoc)
			if not stopTile or not stopTile:canPass(targetWarrior) then
				break -- This is the stop tile
			end
		until stopDst > self.repelDistance
		
		local stayTile, stayLoc, stayDst = nil, stopLoc, stopDst
		repeat
			stayDst = stayDst - 1
			stayLoc:sub(dir)
			stayTile = g_chessboard:getTile(stayLoc)
			assert(stayTile)
		until stayDst == 0 or stayTile:canStay(targetWarrior)
			
		if stayLoc ~= currentLoc then
			local ObjectAction = require 'HighAction.ObjectAction'
			local SpanVariable = require 'Action.SpanVariable'
			local EaseFunction = require 'Ease.EaseFunction'
			local ActionAdapter= require 'Action.ActionAdapter'
			local obj = targetWarrior:getHostObject()
			local var = SpanVariable.new(nil, stayTile:getCenterVector())
			local ease= EaseFunction.wrapOut(EaseFunction[3])
			local action = ObjectAction.move(obj, var, 50, ease)
			targetBarrier:setTile(stayTile)
			ActionAdapter.fit(obj, action)
		end
	end
end

function WarriorHitResult:onHitEnd(sourceWarrior, targetBarrier)
	if self:hasDamage() then
		local targetWarrior = self:_checkWarrior(targetBarrier)
		
		local animator = targetWarrior:findPeer(c'AnimatedMesh')
		if not targetWarrior:isDead() then
			animator:playAnimation(c'idle')
		end
	end
end

return WarriorHitResult
