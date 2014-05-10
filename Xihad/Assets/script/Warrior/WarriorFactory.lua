local SkillRegistry = require 'Skill.SkillRegistry'

local factory = {
	highlightActiveWarrior = nil,	
}
factory.__index = factory

function factory.new(team)
	local AsConditionFactory = require 'Async.AsConditionFactory'
	local RenderAction = require 'HighAction.RenderAction'
	local ActionAdapter= require 'Action.ActionAdapter'
	local SpanVariable = require 'Action.SpanVariable'
	
	local duration = 0.3
	return setmetatable({
			highlightActiveWarrior = {
				onRoundBegin = function (self, warrior)
					local render = warrior:findPeer(c'Render')
					
					if render then
						local spanColor = SpanVariable.new(nil, Color.new(Color.white))
						local action = RenderAction.diffuse(render, spanColor, duration)
						ActionAdapter.fit(render:getHostObject(), action)
						
						-- spanColor = SpanVariable.new(nil, Color.new(Color.white))
						-- action = RenderAction.ambient(render, spanColor, duration)
						-- ActionAdapter.fit(render:getHostObject(), action)
						
						if coroutine.running() then
							AsConditionFactory.waitAction(action)
						end
					end
				end,
				
				onRoundEnd = function (self, warrior)
					local render = warrior:findPeer(c'Render')
					
					if render then
						local spanColor = SpanVariable.new(nil, Color.new(Color.black))
						local action = RenderAction.diffuse(render, spanColor, duration)
						ActionAdapter.fit(render:getHostObject(), action)
						
						-- local spanColor = SpanVariable.new(nil, Color.new(0xff555555))
						-- action = RenderAction.ambient(render, spanColor, duration)
						-- ActionAdapter.fit(render:getHostObject(), action)
						
						if coroutine.running() then
							AsConditionFactory.waitAction(action)
						end
					end
				end
			}
		}, factory)
end

function factory:create(team, name, data)
	local object = g_scene:createObject(c(name))
	
	-- Warrior
	data.team = team
	local warrior = object:appendComponent(c'Warrior', data)
	local barrier = object:appendComponent(c'WarriorBarrier')
	local skillCaster = object:appendComponent(c'SkillCaster')
	
	-- Animated mesh
	local param = { 
		mesh  = "Assets/model/".. data.model .. "/" .. data.model ..  ".b3d", 
		clips = "Assets/model/".. data.model .. "/" .. data.model ..  ".clip" 
	}
	local anim = object:appendComponent(c"AnimatedMesh", param)
	anim:createSelector(c"stupid") 
	anim:playAnimation(c"idle")
	anim:setTransitionTime(0.15)
	warrior:addRoundListener(self.highlightActiveWarrior)
	
	-- TODO
	if data.model == 'ninja' then
		object:resetScale(math3d.vector(1.5))
	end
	
	object:addTag(c'Warrior')
	object:addTag(c(team))
	
	-- learn skills
	if data.skills then
		local skill
		for skillName, count in pairs(data.skills) do
			skill = SkillRegistry.findSkillByName(skillName)
			skillCaster:learnSkill(skill, count)
		end
	end
	
	-- TODO FIXME
	if team == 'Enemy' then
		object:appendComponent(c'ECLTactic', {
				enemy = function (warrior, enemy) 
					local dist = enemy:getLocation():distance(warrior:getLocation())
					local enough = (dist == 1)
					return 100 - dist, enough
				end,
				
				cast = function (warrior, enemy, cast)
					return 1, true
				end,
				
				location = function (warrior, enemy, cast, location)
					return 1, true
				end,
			})
	end
	
	warrior:deactivate()
	return object
end

return factory
