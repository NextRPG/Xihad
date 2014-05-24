local Utils = require "assets.ui.StaticUtils"
local findWindow = Utils.findWindow

local effectIconMap = {
	[-3] = "MapEffectDownToThree",
	[-2] = "MapEffectDownToTwo",
	[-1] = "MapEffectDownToOne",
	[0] = "MapEffectToZero",
	[1] = "MapEffectUpToOne",
	[2] = "MapEffectUpToTwo",
	[3] = "MapEffectUpToThree",
}
local attrIconMap = {
	["攻击力"] = "XihadUI/Map/sword_icon",
}
local function getWidgetConfig(effect)
	if not effect then return "", "" end

	local attrIcon = attrIconMap[effect.attrName] or ""
	local event = effectIconMap[effect.effectLevel]
	if effect.effectLevel == 0 or not event then
		print("effectLevel should not be :", effect.effectLevel)
	end
	-- local tooltip = effect.attrName..(effect.effectLevel > 0 and "上升" or "下降")
	
	return attrIcon, event --, tooltip
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
		local left, event, tooltip = getWidgetConfig(effects[i])
		iconWidget[i]:setProperty("LeftIcon", left)
		iconWidget[i]:fireEvent(event, CEGUI.WindowEventArgs(iconWidget[i]))
		-- iconWidget[i]:setTooltipText(tooltip)
	end
	
	window:fireEvent("FrameShow", CEGUI.WindowEventArgs:new(window))
	return window
end

function MapTileInfo:close()
	local window = findWindow("MapTileInfo")
	window:fireEvent("FrameHide", CEGUI.WindowEventArgs:new(window))
end

return MapTileInfo