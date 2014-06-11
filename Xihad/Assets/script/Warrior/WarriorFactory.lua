local ItemRegistry = require 'Item.ItemRegistry'
local SkillRegistry= require 'Skill.SkillRegistry'
local ObjectAction = require 'HighAction.ObjectAction'
local RenderAction = require 'HighAction.RenderAction'
local ActionAdapter= require 'Action.ActionAdapter'
local SpanVariable = require 'Action.SpanVariable'
local SpanColor = require 'Action.SpanColor'
local AsConditionFactory = require 'Async.AsConditionFactory'

local factory = {
	highlightActiveWarrior = nil,	
}
factory.__index = factory

function factory.new(team)
	local duration = 0.3
	return setmetatable({
			highlightActiveWarrior = {
				onRoundBegin = function (self, warrior)
					local render = warrior:findPeer(c'Render')
					
					if render then
						local spanColor = SpanColor.new(nil, Color.new(Color.white))
						local action = RenderAction.diffuse(render, spanColor, duration)
						ActionAdapter.fit(render:getHostObject(), action)
					end
				end,
				
				onRoundEnd = function (self, warrior, mode)
					local render = warrior:findPeer(c'Render')
					
					if render then
						local spanColor = SpanColor.new(nil, Color.new(Color.black))
						local action = RenderAction.diffuse(render, spanColor, duration)
						ActionAdapter.fit(render:getHostObject(), action)
						
						if coroutine.running() and mode ~= 'immediate' then
							AsConditionFactory.waitAction(action)
						end
					end
				end
			}
		}, factory)
end

function factory:getCreationParam(name)
	return {
		mesh  = "Assets/model/".. name .. "/" .. name ..  ".b3d", 
		clips = "Assets/model/".. name .. "/" .. name ..  ".clip" 
	}
end

function factory:create(team, name, data)
	local object = g_scene:createObject(c(name))
	
	if type(data.career) == 'string' then
		local CareerRegistry = require 'Career.CareerRegistry'
		data.career = CareerRegistry.findCareerByName(data.career)
	end
	
	-- Warrior
	data.team = team
	local warrior = object:appendComponent(c'Warrior', data)
	
	-- Animated mesh
	local param= self:getCreationParam(data.model)
	local anim = object:appendComponent(c"AnimatedMesh", param)
	anim:createSelector(c"stupid") 
	anim:playAnimation(c"idle")
	anim:setTransitionTime(0.15)
	warrior:addRoundListener(self.highlightActiveWarrior)
	
	local parcel = object:appendComponent(c'Parcel')
	local equiper= object:appendComponent(c'Equiper')
	local barrier= object:appendComponent(c'WarriorBarrier')
	local caster = object:appendComponent(c'SkillCaster')
	
	-- TODO remove
	if data.model == 'ninja' then
		object:resetScale(math3d.vector(1.5))
	end
	
	object:addTag(c'Warrior')
	object:addTag(c(team))
	
	if data.tags then
		for _, tag in ipairs(data.tags) do
			object:addTag(c(tag))
		end
	end
	
	-- learn skills
	if data.skills then
		local skill
		for skillName, count in pairs(data.skills) do
			skill = SkillRegistry.findSkillByName(skillName)
			caster:learnSkill(skill, count)
		end
	end
	
	-- gain item
	if data.items then
		local item 
		for itemName, count in pairs(data.items) do
			item = ItemRegistry.findItemByName(itemName)
			parcel:gainItem(item, count)
		end
	end
	
	-- TODO FIXME
	if team == 'Enemy' then
		local Location = require 'route.Location'
		local Approaching = require 'Tactic.Approaching'
		local EnemyGrader = require 'Tactic.EnemyGrader'
		local CastGrader  = require 'Tactic.CastGrader'
		object:appendComponent(c'ECLTactic', {
				enemy = EnemyGrader.nearest(),
				
				cast = CastGrader.maxDamage(),
				
				location = function (warrior, enemy, cast, location)
					return 1, true
				end,
				
				-- approaching = Approaching.fixedTarget(Location.new(4, 1))
				approaching = Approaching.enemyGrader(EnemyGrader.nearest(), 'Hero'),
			})
	else
		data.grader = data.career:getGrader()
		object:appendComponent(c'Leveler', data)
	end
	
	-- hook	
	warrior:addPropertyListener('HitPoint', function (warrior, pname, prev)
		if not prev then return end
		local delta = warrior:getHitPoint() - prev
		if delta < 0 then
			local parent = warrior:getHostObject()
			local damageObject = g_scene:createUniqueObject(c'damage', parent)
			local damageText = damageObject:appendComponent(c'Text', {
					text = math.abs(delta),
					font = 'Assets/font/digit.xft',
				})
			
			damageText:getMaterial(0):setZBufferCompareFunc('true')
			damageText:adjustToHeight(2.5)
			damageText:setColor(Color.new(Color.red))
			
			local extent = parent:findComponent(c'Render'):getAABB():extent()
			local _, height = extent:xyz()
			damageObject:resetTranslate(math3d.vector(0, height*0.8, 0))
			local var = SpanVariable.new(nil, math3d.vector(0, height*1.2, 0))
			local action = ObjectAction.move(damageObject, var, 8)
			action:addFinishListener(function ()
				damageObject:stop()
			end)
			
			ActionAdapter.fit(damageObject, action)
		end
	end)
	
	warrior:addPropertyListener('Dead', function (warrior, pname, prev)
		if warrior:isDead() then
			local animator = warrior:findPeer(c'AnimatedMesh')
			animator:playAnimation(c'die', false)
			AsConditionFactory.waitAnimation(animator)
			
			g_scheduler:schedule(function ()
				-- TODO remove this evil code 
				warrior:getHostObject():stop()
			end)
		end
	end)
	
	-- TODO
	local buffEffects = {}
	warrior:setExclusiveEffectListener({
			onEffectBind = function (self, warrior, effect, lock)
				print('------------- bind effect')
				if type(effect.getFieldEquation) ~= 'function' then
					return
				end
				local field, eq = effect:getFieldEquation()
				-- local isPromote = warrior:get
				-- buffEffects[field]:
				
				local ParticleLoadEnv = require 'Particle.ParticleLoadEnv'
				local host = warrior:getHostObject()
				local env = ParticleLoadEnv.newSingle(host)
				env:inflate('effect.ATKBuff')
				
				local pobject = env:getParticleObject()
				pobject:setParent(host)
				buffEffects[field] = pobject
			end,
			
			onEffectUnbind = function (self, warrior, effect, lock)
				print('------------- unbind effect')
				if type(effect.getFieldEquation) ~= 'function' then
					return
				end
				local field, eq = effect:getFieldEquation()
				
				if buffEffects[field] ~= nil then
					buffEffects[field]:stop()
				end
			end
		})
	
	
	warrior:deactivate()
	return object
end

return factory
