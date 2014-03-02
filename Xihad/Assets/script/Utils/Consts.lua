

Consts = {
	-- map
	TILE_WIDTH = 10, 
	TILE_HEIGHT = 10,
	-- characterProperties
	properties = {
		"physicalAttack",
		"physicalDefense",
		"magicAttack",
		"magicDefense", 
		"maxAP",			
		"maxHP"
	},
	-- characterEquipmentLocation
	parts = {
		"weapon",
		"armor",
		"accessory"
	},

	directions = {LEFT = {x= -1, y = 0}, RIGHT = {x = 1, y = 0}, UP = {x = 0, y = 1}, DOWN = {x = 0, y = -1}}

}

package.loaded[ "Consts" ] = Consts