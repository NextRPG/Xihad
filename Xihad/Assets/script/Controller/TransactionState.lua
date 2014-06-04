local base = require 'Controller.PlayerState'
local Table= require 'std.Table'
local XihadTileSet = require 'Chessboard.XihadTileSet'

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

function TransactionState:_closeTransaction()
	if self:isTransacting() then
		self.ui:closeTransaction()
		self.transactingWarrior = nil
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

function TransactionState:_apply_warrior(warrior, parcelView)
	warrior:findPeer(c'Parcel'):restoreView(parcelView)
end

function TransactionState:onUICommand(model)
	if not self:isTransacting() then
		print('No transactingWarrior')
		return
	end
	
	self:_apply_warrior(self:_getSource(), model.masterParcel)
	self:_apply_warrior(self.transactingWarrior, model.guestParcel)
	self:_closeTransaction()
	return 'done'
end

function TransactionState:onTileSelected(tile)
	assert(not self:isTransacting())
	
	local set = self:_getExchanableSet()
	if set[tile] and tile:getWarrior() then
		print('beginTransaction')
		self:_clearHandle()
		self.transactingWarrior = tile:getWarrior()
		self.ui:showTransaction(self:_getSource(), self.transactingWarrior)
	else
		print('not exchanable')
	end
end

function TransactionState:onTileHovered(tile)
	-- TODO: Blink?
end

return TransactionState
