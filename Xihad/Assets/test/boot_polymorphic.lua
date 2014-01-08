-- base class
local CBase = {}
CBase.__index = CBase
package.loaded["CBase"] = CBase

function CBase.new(p, obj)
	local res = {}
	setmetatable(res, ListenerManager)
	return res
end

function CBase:derive()
	print("derive in base")
end

function CBase:poly()
	print("poly in base")
end

function CBase.initSystem()
	print("init base system")
end

-- derived class
local CDerived = {}
CDerived.__index = CDerived
package.loaded["CDerived"] = CDerived

---[[
CDerived.__base = "CBase"
--]]
---[[
setmetatable(CDerived, CBase)
--]]

function CDerived.new(p, obj)
	local res = {}
	setmetatable(res, CDerived)
	return res
end

function CDerived:poly()
	print("poly in derived")
end

function CDerived.initSystem()
	print("init derived system")
end


-- test case
local go = scene:createObject(c"sb")
local c1 = go:appendComponent(c"CDerived")
local c2 = go:searchComponent(c"CBase")
assert(c1==c2)
c2:poly()
c2:derive()