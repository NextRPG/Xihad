local factory = {
	team = nil,
}
factory.__index = factory

function factory.new(team)
	local o = {
		team = team,
 	}
 	
 	setmetatable(o, factory)
 	return o
end

function factory:create(data, tile)
	local object = g_scene:createObject(c(data.name))
	object:appendComponent(c'Character')
	
	
end
