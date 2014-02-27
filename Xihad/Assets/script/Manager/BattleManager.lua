local Chessboard = require "Chessboard"
local SkillManager = require "SkillManager"
local StateMachine = require "StateMachine"
local SkillManager = require "SkillManager"
--- 
-- 控制战斗流程
-- @module BattleManager
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

local PathFinder = require "PathFinder"

local BattleManager = {
	currentState = "showCharacter"
}

function BattleManager:init( manager1, manager2 )
	self.manager = manager2

	local stateMachine = StateMachine.new()
	self.stateMachine = stateMachine
	

	self:addShowNothing(manager1, manager2)
	self:addShowCharacter( manager1, manager2 )
	self:addShowTile()
	self:addShowSkill()
	self:addShowTargetRange()
	global:appendUpdateHandler(self)

	stateMachine:setInitial("showNothing")
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
		self.manager = manager2
		self.manager:roundStart() 
		self.manager:runActors()
		self.manager = manager1
		self.manager:roundStart()
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
			Chessboard:clearAll()
			self.manager:onSelectCharacter(object)
		end)
	stateMachine:addTransition("showCharacter", "showCharacter",
		function (  )
			return self.manager:checkRoundOver()
		end, 
		function (  )
			self.manager = manager2
			self.manager:roundStart() 
			self.manager:runActors()
			self.manager = manager1
			self.manager:roundStart()
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
			Chessboard:popArea(PathFinder)
			Chessboard:popArea(SkillManager.allTargets)
			self.manager:onSelectCharacter(object)
		end)
	stateMachine:addTransition("showTile", "showSkill",
		function ( object, etype )
			return etype == "lClicked" and object and object:hasTag(c"Tile") and PathFinder:hasTile(object:findComponent(c"Tile"))
		end,
		function ( object )
			Chessboard:pushArea(PathFinder, "ALPHA")
			Chessboard:pushArea(SkillManager.allTargets, "ALPHA")
			self.manager:onSelectTile(object:findComponent(c"Tile"))
			SkillManager:onShowSkills(self.manager.currentCharacter)
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
		end)
	stateMachine:addTransition("showSkill", "showTargetRange",
		function ( key )
			return type(key) == "string" and tonumber(key) ~= nil and tonumber(key) <= 3 
		end,
		function ( key )
			SkillManager:onSelectSkill(key)
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
			SkillManager:onCastSkill(object:findComponent(c"Tile"))
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