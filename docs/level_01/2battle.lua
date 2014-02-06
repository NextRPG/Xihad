assert(arg[1], "arg[1] can't be nil")
arg[2] = arg[2] or string.gsub(arg[1], "lua", "battle")

local map = dofile(arg[1])
io.output(arg[2])
io.write("return")

function serialize( o )
	if type(o) == "number" then
		io.write(o)
	elseif type(o) == "string" then
		io.write(string.format("%q", o))
	elseif type(o) == "table" then
		io.write(" {\n")
		for k,v in pairs(o) do
			-- io.write(" ", k, " = ")
			io.write(" [")
			serialize(k)
			io.write("] = ")
			serialize(v)
			io.write(",\n")
		end
		io.write("}")
	else
		error("cannot serialize a " .. type(o))
	end
end

function getTableByName( table, name )
	for i,v in ipairs(map[table]) do
		if name == v.name then
			return v
		end
	end
	error("can't find the " .. table .. " - " .. name)
end

function getLayer( name )
	return getTableByName("layers", name)
end

function getTileSet( name )
	return getTableByName("tilesets", name)
end

function getTile( id )
	for i,v in ipairs(getTileSet("texture").tiles) do
		if tonumber(id) == i then
			for k,v2 in pairs(v.properties) do
				if tonumber(v2) ~= nil then
					v.properties[k] = tonumber(v2)
				end
			end
			return v.properties
		end
	end
end

function clone( t, t2 )
	local t2 = t2 or {}
	for k,v in pairs(t) do
		if type(v) == "table" then
			t2[k] = {}
			clone(v, t2[k])
		else
			t2[k] = v
		end
	end
	return t2
end



local chessboard = {}
chessboard.width, chessboard.height = map.width, map.height
for i=0,map.width - 1 do
	for j=0,map.height - 1 do
		assert(getLayer( "map" ), "map nil")
		local tile = getTile(getLayer( "map" ).data[j * map.width + i + 1])
		tile.x = i
		tile.y = map.height - j - 1
		chessboard[#chessboard + 1] = clone(tile)
	end
end

function initCharacters( name , characters )
	local layer = getLayer( name )
	local width = map.tilewidth
	local height = map.tileheight
	for i,character in ipairs(layer.objects) do
		local temp = {
			name = character.name,
			x = math.floor(character.x / width),
			y = math.floor(character.y / height)
		}
		for i,v in ipairs(character.properties) do
			if tonumber(v) ~= nil then
				temp[k] = tonumber(v)
			end
		end
		characters[i] = temp
	end
end

local heros = {}
initCharacters("heros", heros)

local AIs = {}
initCharacters("AIs", AIs)

local battle = {}
battle.chessboard = chessboard
battle.heros = heros
battle.AIs = AIs
serialize(battle)