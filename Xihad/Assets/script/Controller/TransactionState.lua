local base = require 'Controller.PlayerState'
local Table= require 'std.Table'
local XihadTileSet = require 'Chessboard.XihadTileSet'
local ObjectAction = require 'HighAction.ObjectAction'
local ActionFactory= require 'Action.ActionFactory'
local ActionAdapter= require 'Action.ActionAdapter'
local AsConditionFactory = require 'Async.AsConditionFactory'

local TransactionState = setmetatable({
	tradableHandle = nil,
	transactingWarrior = nil,
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

function TransactionState:_beginTransaction(warrior)
	self.transactingWarrior = warrior
	self.ui:showTransaction(self:_getSource(), warrior)	
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
	
	self.executor:transact(master, guest, masterView, guestView)
end

function TransactionState:onUICommand(model)
	if not self:isTransacting() then
		print('No transactingWarrior')
		return
	end
	
	self:_apply_transaction(model)
	self:_closeTransaction()
	return 'done'
end

function TransactionState:_from_to_rot(from, to)
	return ObjectAction.rotateYTo(from, to:getTranslate(), 90/0.2)
end

function TransactionState:_face_to_face(obj1, obj2)
	local action1 = self:_from_to_rot(obj1, obj2)
	local action2 = self:_from_to_rot(obj2, obj1)
	local parallel = ActionFactory.parallel{ action1, action2 }
	ActionAdapter.fit(obj1, parallel)
	-- AsConditionFactory.waitAction(parallel)
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
