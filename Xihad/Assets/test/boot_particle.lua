package.path = package.path..";Assets/script/?.lua"
local Loader = require "particle.ParticleLoader"

local object = g_scene:createObject(c'lalala')
local pcomp = object:appendComponent(c"ParticleSystem")
Loader.load("Assets.effect.rain", pcomp)

local camObj = g_scene:createObject(c'cam')
camObj:appendComponent(c'Camera')
