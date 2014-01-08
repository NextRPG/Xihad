--- 
-- 一个统计各种因素后给出结果的计算器,一般数值越大越可能被选中
-- @module ScoreBoard
-- @author wangxuanyi
-- @license MIT
-- @copyright NextRPG

---
-- ratio记录相同序号的排名列表的分数所占比例
-- result记录计算出的结果
-- @tab result 
-- @tparam {float, ...} ratio
-- @string key
local ScoreBoard = {

}

function ScoreBoard.new( o )
	assert(type(o) == "table", "prototype must be a table")
	setmetatable(o, {__index = ScoreBoard})
	o.data = {}
	o.ratio = {}
	return o
end

---
-- 加上一个key列表
-- tparam {string, ...} list
function ScoreBoard:appendKey( list )
	for i,key in ipairs(list) do
		self.data[key] = {}
	end
end

---
-- 加上一个列表和它所对应的ratio
-- @tab list
-- @float ratio
function ScoreBoard:appendValue( list, ratio )
	self.ratio[#self.ratio + 1] = ratio
	local columnIndex = #self.ratio	
	for i,v in ipairs(list) do
		self.data[v.name][columnIndex] = i
	end
end

---
-- 计算出结果并返回分值最高的一个单位
function ScoreBoard:getResult(  )
	local testValue = 0
	for i,v in ipairs(self.ratio) do
		testValue = testValue + v
	end
	assert(testValue == 1, "not the correct ratio")
	for k,list in pairs(self.data) do
		list.result = 0
		for i,ratio in ipairs(self.ratio) do
			list.result = list.result + ratio * list[i]
		end
	end
	local maxName = ""
	local maxScore = 0
	for k,list in pairs(self.data) do
		if list.result > maxScore then
			maxName, maxScore = k, list.result
		end
	end
	return maxName
end

return ScoreBoard