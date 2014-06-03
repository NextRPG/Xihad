---
--[[
技能需求：
	+ 基础伤害值
	+ 职业属性同技能属性
	+ 技能需要有属性（风、火、冰、电、光、暗）
	+ 技能的附加状态（成功率与角色的能力有关吗？）
	+ 是否可以对自身使用？可以对队友使用？可以对敌人使用？可以对空地使用？
	+ 击退距离（不是水平方向怎么办？）
	+ 攻击范围与攻击距离
	+ 目前只有粒子效果的技能
--]]

local Skill = {
	name  	= 'unknown',
	
	range 	= nil,
	fitler  = nil,
	animator= nil,
	resolvers = nil,
}
Skill.__index = Skill

function Skill.new(name, range, filter, animator)
	local obj = setmetatable({
			name  = name,
			filter= filter,
			range = range,
			animator = animator,
			resolvers= {},
		}, Skill)
	return obj
end

function Skill:addResolver(resolver)
	assert(resolver)
	table.insert(self.resolvers, resolver)
end

function Skill:resolve(sourceWarrior, impactLocation, chessboard)
	local results = {}
	
	self.range:traverseImpactLocations(impactLocation, function(loc)
		local tile = chessboard:getTile(loc)
		
		if not tile then 
			return 
		end
		
		local relativeLoc = loc - impactLocation
		for barrier, _ in pairs(tile.barriers) do
		repeat
			if not barrier.newHitResult then
				break	-- continue
			end
			
			if not barrier:permitCasting(sourceWarrior, self) then
				break	-- continue
			end
			
			local result = barrier:newHitResult(sourceWarrior)
			for _, resolver in ipairs(self.resolvers) do
				resolver:resolve(sourceWarrior, barrier, relativeLoc, result)
			end
			
			if result:isValid() then
				assert(results[barrier] == nil)
				results[barrier] = result
			end
		until true
		end
	end)
	
	return results
end

function Skill:playAnimation(sourceWarrior, targetTile, listener)
	self.animator:animate(sourceWarrior:getHostObject(), targetTile, listener)
end

function Skill:isMultiTarget()
	return self.range:isMultiTarget()
end

function Skill:getName()
	return self.name
end

function Skill:getImpactTiles(chessboard, impactCenter)
	local array = {}
	self.range:traverseImpactLocations(impactCenter, function (loc)
		local tile = chessboard:getTile(loc)
		if tile then
			table.insert(array, tile)
		end
	end)
	
	return array
end

function Skill:getAllImpactTiles(chessboard, launcherLocation, set)
	set = set or {}
	
	self.range:traverseAllImpactLocations(launcherLocation, nil, 
		function (loc)
			local tile = chessboard:getTile(loc)
			
			if tile then
				set[tile] = true
			end
		end
	)
	
	return set
end

function Skill:getLaunchableTiles(chessboard, sourceWarrior, launcherLocation, set)
	set = set or {}
	launcherLocation = launcherLocation or sourceWarrior:getLocation()
	
	self.range:traverseLaunchableLocations(launcherLocation, 
		function (loc)
			local tile = chessboard:getTile(loc)
			if not tile then return end
			
			if tile:permitCasting(sourceWarrior, self) then
				set[tile] = true
			end
		end
	)
	
	return set
end

function Skill:getRange()
	return self.range
end

function Skill:canCastToVacancy()
	return self.filter.toVacancy
end

function Skill:canCastToSelf()
	return self.filter.toSelf
end

function Skill:canCastToLeague()
	return self.filter.toLeague
end

function Skill:canCastToEnemy()
	return self.filter.toEnemy
end

return Skill
