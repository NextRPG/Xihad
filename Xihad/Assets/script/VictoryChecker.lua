VictoryChecker = {

}
package.loaded["VictoryChecker"] = VictoryChecker


function VictoryChecker:new( o )
	o = o or {}

	setmetatable(o, self)
	self.__index = self
	return o
end

function VictoryChecker:check(  )
	
end