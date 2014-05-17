local Utils = require "assets.ui.StaticUtils"
local findWindow = Utils.findWindow

local effectIconMap = {
	[-3] = "XihadUI/Map/effect_down_three",
	[-2] = "XihadUI/Map/effect_down_two",
	[-1] = "XihadUI/Map/effect_down_one",
	[1] = "XihadUI/Map/effect_up_one",
	[2] = "XihadUI/Map/effect_up_two",
	[3] = "XihadUI/Map/effect_up_three",
}
local attrIconMap = {
	["攻击力"] = "XihadUI/Map/sword_icon",
}
local function getIcons(effect)
	if not effect then return "", "" end

	local attrIcon = attrIconMap[effect.attrName] or ""
	local effectIcon = effectIconMap[effect.effectLevel] or ""
	local tooltip = effect.attrName..(effect.effectLevel > 0 and "上升" or "下降")
	
	return attrIcon, effectIcon 
end

local MapTileInfo = {
}
--[[------------------------------------------------------------------------
	MapTileInfo
		+ 可选参数：
    - 图标
    - 名称
    - 属性icon + 箭头（数量表示强度1-3, -3 - -1）
    {
		name = "草地",
		effects = {
			[1] = { attrName = "Attack", effectLevel = -3 },
			[2] = { attrNmae = "", effectLevel = 1 }
		}
    }
--]]------------------------------------------------------------------------
function MapTileInfo:show(args)
	local window = findWindow("MapTileInfo")
	local tileType = args.name and args.name or ""
	window:setText(tileType)
	
	local iconWidget = {
		window:getChild("Attr1"),
		window:getChild("Attr2"),
		window:getChild("Attr3"),
	}
	
	local effects = args.effects
	
	for i=1,3 do
		local left, right, tooltip = getIcons(effects[i])
		iconWidget[i]:setProperty("LeftIcon", left)
		iconWidget[i]:setProperty("RightIcon", right)
		-- iconWidget[i]:setTooltipText(tooltip)
	end
	
	window:fireEvent("FrameShow", CEGUI.WindowEventArgs:new(window))
end

function MapTileInfo:close()
	local window = findWindow("MapTileInfo")
	window:fireEvent("FrameHide", CEGUI.WindowEventArgs:new(window))
end

return MapTileInfo