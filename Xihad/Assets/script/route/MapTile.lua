local algo = require 'std.Algorithm'

local MapTile = {
	location = nil,	-- location of this tile
	barriers = nil,
	indexed = nil,
}
MapTile.__index = MapTile

function MapTile.new(location)
	return setmetatable({
			location = location:copy(),
			barriers = {},
			indexed = {},
		}, MapTile)
end

function MapTile:getLocation()
	return self.location:copy()
end

local no_indexed_data = {}
function MapTile:onBarrierAdded(e, optKey)
	if e:getTile() ~= self then
		error('Attempt to add illegal barrier to tile')
	end
	
	local value
	if optKey ~= nil then
		if self.indexed[optKey] ~= nil then
			error(string.format('Duplicated key %q at Loc[%d, %d]', optKey, self.location:xy()))
		end
		
		self.indexed[optKey] = e
		value = optKey
	else
		value = no_indexed_data
	end
	
	assert(value ~= nil)
	self.barriers[e] = value
	
	for existed, otherKey in pairs(self.barriers) do
		if existed ~= e then
			existed:inhabitWith(e, optKey)
			e:inhabitWith(existed, otherKey)
		end
	end
end

function MapTile:onBarrierRemoved(e)
	if e:getTile() ~= self then
		error('Attempt to remove illegal barrier to tile')
	end

	local indexKey = self.barriers[e]
	if indexKey ~= no_indexed_data then
		assert(self.indexed[indexKey] == e)
		self.indexed[indexKey] = nil
	end
	
	self.barriers[e] = nil
	
	for existed, otherKey in pairs(self.barriers) do
		existed:leaveFrom(e, indexKey)
		e:leaveFrom(existed, otherKey)
	end
end

function MapTile:findBarrierByKey(key)
	return self.indexed[key]
end

function MapTile:isVacancy()
	return algo.all_of_t(self.barriers, function (e, v)
		return e:keepVacancy()
	end)
end

function MapTile:permitCasting(warrior, skill)
	return 	skill:canCastToVacancy() and self:isVacancy() or
			algo.any_of_t(self.barriers, function(e, v)
				return e:permitCasting(warrior, skill)
			end)
end

function MapTile:canPass(warrior)
	return algo.all_of_t(self.barriers, function(e, v)
		return e:canPass(warrior)
	end)
end

function MapTile:canStay(warrior)
	return algo.all_of_t(self.barriers, function(e, v)
		return e:canStay(warrior)
	end)
end

function MapTile:getActionPointCost(warrior)
	local accum = 0
	for e,_ in pairs(self.barriers) do
		accum = accum + e:getActionPointCost(warrior)
	end
	
	return math.max(1, accum)
end

function MapTile:_find_surveyable_barrier()
	for barrier, _ in pairs(self.barriers) do
		local surveyType = barrier:getSurveyType(warrior)
		if surveyType ~= nil then
			return barrier, surveyType
		end
	end
end

function MapTile:getSurveyType(warrior)
	local barrier, surveyType = self:_find_surveyable_barrier()
	return surveyType
end

function MapTile:onSurveyed(warrior)
	local barrier, surveyType = self:_find_surveyable_barrier()
	if barrier then
		return barrier:onSurveyed(warrior)
	end
	
	return false
end

return MapTile
