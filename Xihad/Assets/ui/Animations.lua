local Utils = require "ui.StaticUtils"
local findWindow = Utils.findWindow

local animMgr = CEGUI.AnimationManager:getSingleton()
animMgr:loadAnimationsFromXML("Xihad.anims")

local animations = 	
{ 
	FrameShow = { "MapTileInfo" },
	FrameHide = { "MapTileInfo" },
	PullDown = { "ParcelPanel/ParcelExchange" },
	PullUp = { "ParcelPanel/ParcelExchange" },
	Unfold = { "NotificationPanel/Notification"},
	Fold = { "NotificationPanel/Notification"},
	UnfoldAndFold = { "NotificationPanel/Notification"},
	MouseClick = { "NotificationPanel/Notification/Mouse"}, 
	Unfold_F = { "GainExpPanel/GainExp" },
	Fold_F = { "GainExpPanel/GainExp" },
	
	TVOpenClose = { "FightStartPanel/FightStart" },
	FlyToLeft = { "FightStartPanel/FightStart/PanelT/VictoryShadow",  },
	FlyToRight = { "FightStartPanel/FightStart/PanelB/ConditionShadow" },
	
	ProgressGrow = { "GainExpPanel/GainExp" },
	ProgressGrowOver = { "GainExpPanel/GainExp" },
	
	MoveIn = { "LevelUp/LArea/LastLevel", },
	DelayedMoveIn = { "LevelUp/MArea/Arrow" },
	DelayedFadeIn = { "LevelUp/RArea/NewLevel" },
	
	MapEffectUpToOne = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
	MapEffectUpToTwo = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
	MapEffectUpToThree = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
	MapEffectDownToOne = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
	MapEffectDownToTwo = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
	MapEffectDownToThree = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
	MapEffectToZero = { "MapTileInfo/Attr1", "MapTileInfo/Attr2", "MapTileInfo/Attr3" },
	
	L2RArrowAnim = { "ParcelPanel/ParcelExchange/L2RArrow" },
	R2LArrowAnim = { "ParcelPanel/ParcelExchange/R2LArrow" },
}

for animName, targetList in pairs(animations) do
	local anim = animMgr:getAnimation(animName)
	for _,wnd  in pairs(targetList) do
		local targetWindow = findWindow(wnd)
		local ai = animMgr:instantiateAnimation(anim)
		ai:setTargetWindow(targetWindow)
	end
end