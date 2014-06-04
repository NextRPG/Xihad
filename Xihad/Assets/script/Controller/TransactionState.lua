local base = require 'Controller.PlayerState'
local Table= require 'std.Table'
local XihadTileSet = require 'Chessboard.XihadTileSet'
local ObjectAction = require 'HighAction.ObjectAction'
local ActionFactory= require 'Action.ActionFactory'
local ActionAdapter= require 'Action.ActionAdapter'

local TransactionState = setmetatable({
	tradableHandle = nil,
	transactingWarrior = nil,
	sourceOnLeft = false,
}, base)
TransactionState.__index = TransactionState

function TransactionState.new(...)
	return setmetatable(base.new(...), TransactionState)
end

function TransactionState:_getExchanableSet()
	return XihadTileSet.create(self:_getSource():exchangables())
end

function TransactionState:_getExchanableArray()
	return Table.extractKeys(self:_getExchanableSet())
end

local function getScreenXOf(warrior)
	local worldPos = warrior:getHostObject():getTranslate()
	return (g_collision:getScreenCoordFromPosition(worldPos))
end

function TransactionState:_beginTransaction(warrior)
	local source = self:_getSource()
	self.transactingWarrior = warrior
	self.sourceOnLeft = getScreenXOf(source) < getScreenXOf(warrior)
	
	local left, right = source, warrior
	if not self.sourceOnLeft then
		left, right = right, left
	end
	self.ui:showTransaction(left, right)
end

function TransactionState:_closeTransaction()
	if self:isTransacting() then
		self.ui:closeTransaction()
		self.transactingWarrior = nil
		self.camera:ascendBack()
	end
end

function TransactionState:onStateEnter()
	-- highlight those can trade with
	assert(not self.tradableHandle)
	self:_markRange(self:_getExchanableArray(), 'Tradable', 'tradableHandle')
end

function TransactionState:onBack()
	self:_closeTransaction()
	self:_clearHandle()
	return 'back'
end

function TransactionState:_clearHandle()
	self:_safeClear('tradableHandle')
end

function TransactionState:isTransacting()
	return self.transactingWarrior ~= nil
end

function TransactionState:needTouch()
	return not self:isTransacting()
end

function TransactionState:needHover()
	return not self:isTransacting()
end

function TransactionState:needCDWhenTouch()
	return false
end

function TransactionState:needCDWhenHover()
	return false
end

function TransactionState:_apply_transaction(model)
	local master, guest = self:_getSource(), self.transactingWarrior
	local masterView, guestView = model.masterParcel, model.guestParcel
	
	if not self.sourceOnLeft then
		masterView, guestView = guestView, masterView
	end
	self.executor:transact(master, guest, masterView, guestView)
end

function TransactionState:onUICommand(command, model)
	if command == 'Complete' then
		if not self:isTransacting() then
			io.stderr:write('No transactingWarrior')
			return
		end

		self:_apply_transaction(model)
		self:_closeTransaction()
		return 'done'
	elseif command == 'Cancel' then
		return self:onBack()
	end
end

function TransactionState:_from_to_rot(from, to)
	return ObjectAction.rotateYTo(from, to:getTranslate(), 90/0.2)
end

function TransactionState:_face_to_face(obj1, obj2)
	local action1 = self:_from_to_rot(obj1, obj2)
	local action2 = self:_from_to_rot(obj2, obj1)
	local parallel = ActionFactory.parallel{ action1, action2 }
	ActionAdapter.fit(obj1, parallel)
end

function TransactionState:onTileSelected(tile)
	assert(not self:isTransacting())
	
	local set = self:_getExchanableSet()
	if set[tile] and tile:getWarrior() then
		self:_clearHandle()
		
		-- rotate both
		local guest = tile:getWarrior()
		self:_face_to_face(guest:getHostObject(), self:_getSourceObject())
		self.camera:descendIntoTransaction(guest:getHostObject())
		self:_beginTransaction(guest)
	else
		print('not exchanable')
	end
end

function TransactionState:onTileHovered(tile)
	-- TODO: Blink?
end

return TransactionState
