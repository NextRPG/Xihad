local Chessboard = require "ColoringManager"
local SkillManager = require "SkillManager"
local StateMachine = require "StateMachine"
local SkillManager = require "SkillManager"
local PathFinder = require "PathFinder"
local Publisher = require "Publisher"

--- 
-- 控制战斗流程
-- @module BattleManager
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local BattleManager = {}


function BattleManager:init( manager1, manager2 )
	self.manager = manager2

	local stateMachine = StateMachine.new()
	self.stateMachine = stateMachine
	

	self:addShowNothing(manager1, manager2)
	self:addShowCharacter( manager1, manager2 )
	self:addShowTile()
	self:addShowAITile()
	self:addShowSkill()
	self:addShowTargetRange()
	global:appendUpdateHandler(self)
	inherit(self, Publisher)

	stateMachine:setInitial("showNothing")
end

function BattleManager:AI2Hero( manager1, manager2 )
	self.manager = manager2
	self.manager:roundStart()
	print("round AI start")
	self:changeState("round", self.manager.team)
	self.manager:runActors()
	self.manager = manager1
	self.manager:roundStart()
	print("round Hero start")
	self:changeState("round", self.manager.team)
end

function BattleManager:addShowNothing( manager1, manager2 )
	local stateMachine = self.stateMachine

	stateMachine:addState("showNothing")

	stateMachine:addTransition("showNothing", "showCharacter",
	function (  )
		return self.manager == manager1
	end, 
	function (  )
		self.manager = manager1
		self.manager:roundStart()
	end)

	stateMachine:addTransition("showNothing", "showCharacter",
	function (  )
		return self.manager == manager2
	end, 
	function (  )
		self:AI2Hero(manager1, manager2)
	end)

end

function BattleManager:addShowCharacter( manager1, manager2 )

	local stateMachine = self.stateMachine

	stateMachine:addState("showCharacter")
	stateMachine:addTransition("showCharacter", "showTile",
		function ( object, etype )
			return etype == "lClicked" and object 
					and object:hasTag(c"Hero") 
					and self.manager:checkAvailable(object)
		end,
		function ( object )
			self.manager:onSelectCharacter(object)
		end)
	stateMachine:addTransition("showCharacter", "showAITile",
		function ( object, etype )
			return etype == "lClicked" and object 
					and object:hasTag(c"AI") 
					and self.manager:checkAvailable(object)
		end,
		function ( object )
			self.manager:onSelectCharacter(object)
		end)
	stateMachine:addTransition("showCharacter", "showCharacter",
		function (  )
			return self.manager:checkRoundOver()
		end, 
		function (  )
			self:AI2Hero(manager1, manager2)
		end)
end

function BattleManager:addShowTile(  )

	local stateMachine = self.stateMachine
	stateMachine:addState("showTile")
	stateMachine:addTransition("showTile", "showTile",
		function ( object, etype )
			return etype == "lClicked" and object and object:hasTag(c"Hero") and self.manager:checkAvailable(object)
		end,
		function ( object )
			self.manager:onSelectCharacter(object)
		end)
	stateMachine:addTransition("showTile", "showSkill",
		function ( object, etype )
			return etype == "lClicked" and object and object:hasTag(c"Tile") and PathFinder:hasTile(object:findComponent(c"MapTile"):getLocation())
		end,
		function ( object )
			self.manager:onSelectTile(object:findComponent(c"MapTile"))
			SkillManager:onShowSkills(self.manager.currentCharacter)
		end)
	stateMachine:addTransition("showTile", "showCharacter", 
		function ( object, etype )
			return etype == "rUplift"
		end,
		function (  )
			Chessboard:clearAll()
		end)
	stateMachine:addTransition("showTile", "showAITile", 
		function ( object, etype )
			return etype == "lClicked" and object and object:hasTag(c"AI")
		end, 
		function ( object )
			self.manager:onSelectCharacter(object)
		end)
end

function BattleManager:addShowAITile(  )
	local stateMachine = self.stateMachine
	stateMachine:addState("showAITile")
	stateMachine:addTransition("showAITile", "showAITile",
		function ( object, etype )
			return etype == "lClicked" and object and object:hasTag(c"AI")
		end,
		function ( object )
			self.manager:onSelectCharacter(object)
		end)
	stateMachine:addTransition("showAITile", "showTile",
		function ( object, etype )
			return etype == "lClicked" and object and object:hasTag(c"Hero")
		end,
		function ( object )
			self.manager:onSelectCharacter(object)
		end)

end

function BattleManager:addShowSkill(  )
	local stateMachine = self.stateMachine

	stateMachine:addState("showSkill")
	stateMachine:addTransition("showSkill", "showCharacter",
		function ( key )
			return key == "K"
		end,
		function ( key )
			local character = self.manager.currentCharacter:findComponent(c"Character")
			character.states.TURNOVER = true
		end)
	stateMachine:addTransition("showSkill", "showTargetRange",
		function ( key )
			return type(key) == "string" and tonumber(key) ~= nil and tonumber(key) <= 3 
		end,
		function ( key )
			SkillManager:onSelectSkill(key)
		end)
	stateMachine:addTransition("showSkill", "showTile",
		function ( object, etype )
			return etype == "rUplift"
		end,
		function (  )
			self.manager:back2ShowCharacter()
		end)
end

function BattleManager:addShowTargetRange(  )
	local stateMachine = self.stateMachine
		stateMachine:addState("showTargetRange")
	stateMachine:addTransition("showTargetRange", "showTargetRange",
		function( object, etype )
			return etype == "mouseMoved" and object and object:hasTag(c"Tile")
		end,
		function ( object )
			SkillManager:onSelectTarget( object )
		end)
	stateMachine:addTransition("showTargetRange", "showCharacter",
		function ( object, etype )
			return etype == "lClicked" and object and object:hasTag(c"Tile")
		end,
		function ( object )
			SkillManager:onCastSkill(object:findComponent(c"MapTile"))
		end)
	stateMachine:addTransition("showTargetRange", "showSkill", 
		function ( object, etype )
			return etype == "rUplift"
		end, 
		function (  )
			SkillManager:back2ShowSkill()
		end)
end

function BattleManager:onUpdate(  )
	self.stateMachine:update()
end

function BattleManager:onMouseEvent( e )
	self.stateMachine:update(getSelectedObject(), e.type)
end

function BattleManager:onKeyUp( e )
	self.stateMachine:update(e.key)
end

return BattleManager