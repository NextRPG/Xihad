local Utils = require "assets.ui.StaticUtils"
local findWindow = Utils.findWindow

local animMgr = CEGUI.AnimationManager:getSingleton()
animMgr:loadAnimationsFromXML("Xihad.anims")

local animations = 	{ 
						TextFadeIn = { "AttackDamageLabel"},
						
						Magnify = { "LeftDialog/TextArea", "RightDialog/TextArea" },
						Shrink = { "LeftDialog/TextArea", "RightDialog/TextArea" },
						
						PortraitLighter = { "LeftDialog/ImageArea", "RightDialog/ImageArea" },
						PortraitDarker = { "LeftDialog/ImageArea", "RightDialog/ImageArea" },

						DialogueClose = { "LeftDialog", "RightDialog" },
					
						FrameShow = { "MapTileInfo" },
						FrameHide = { "MapTileInfo" },
						
						["MapEffectUpToOne"] = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
						["MapEffectUpToTwo"] = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
						["MapEffectUpToThree"] = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
						["MapEffectDownToOne"] = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
						["MapEffectDownToTwo"] = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
						["MapEffectDownToThree"] = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
						["MapEffectToZero"] = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
					}

for animName, targetList in pairs(animations) do
	local anim = animMgr:getAnimation(animName)
	for _, name in ipairs(targetList) do
		local targetWindow = findWindow(name)
		animMgr:instantiateAnimation(anim):setTargetWindow(targetWindow)
	end
end