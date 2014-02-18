local Chessboard = require "Chessboard"
local SkillManager = require "SkillManager"
local StateMachine = require "StateMachine"
local PathFinder = require "PathFinder"

local BattleManager = {
	currentState = "showCharacter"
}

function BattleManager:init( manager1, manager2 )
	self.manager = manager2

	stateMachine = StateMachine.new()
	stateMachine:addState("showCharacter")
	stateMachine:addTransition("showCharacter", "showTile",
		function ( object, etype )
			return etype == "lPressed" and object 
					and object:hasTag(c"Hero") 
					and self.manager:checkAvailable(object)
		end,
		function ( object )
			self.manager:onSelectCharacter(object)
		end)
	stateMachine:addTransition("showCharacter", "showCharacter",
		function (  )
			return self.manager:checkRoundOver() or self.manager == manager2
		end, 
		function (  )
			stateMachine:pendingAndAction(function (  )
				self.manager = manager2
				self.manager:roundStart() 
				print("doing something")
				self.manager:runActors()
				self.manager = manager1
				self.manager:roundStart()
			end)

		end)

	stateMachine:addState("showTile")
	stateMachine:addTransition("showTile", "showTile",
		function ( object, etype )
			return etype == "lPressed" and object and object:hasTag(c"Hero")
		end,
		function ( object )
			Chessboard:recoverArea(PathFinder)	
			PathFinder:cleanUp()
			self.manager:onSelectCharacter(object)
		end)
	stateMachine:addTransition("showTile", "showSkill",
		function ( object, etype )
			return etype == "lPressed" and object and object:hasTag(c"Tile")
		end,
		function ( object )
			stateMachine:pendingAndAction(function (  )
				Chessboard:recoverArea(PathFinder)
				self.manager:onSelectTile(object)
				SkillManager:onShowSkills(self.manager.currentCharacter)
			end)
		end)

	stateMachine:addState("showSkill")
	stateMachine:addTransition("showSkill", "showCharacter",
		function ( key )
			return key == "K"
		end,
		function ( key )
		end)
	stateMachine:addTransition("showSkill", "showTargetRange",
		function ( key )
			return type(key) == "string" and tonumber(key) ~= nil
		end,
		function ( key )
			SkillManager:onSelectSkill(key)
		end)

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
			return etype == "lPressed" and object and object:hasTag(c"Tile")
		end,
		function ( object )
			SkillManager:onCastSkill(object)
		end)

	stateMachine:setInitial("showCharacter")
	self.stateMachine = stateMachine

	scene:pushController(self)
end

function BattleManager:onMouseEvent( e )
	if e.type == "lPressed" then
		print(getSelectedObject())
	end
	self.stateMachine:update(getSelectedObject(), e.type)
	return true
end

function BattleManager:onKeyUp( e )
	self.stateMachine:update(e.key)
	return true
end

return BattleManager