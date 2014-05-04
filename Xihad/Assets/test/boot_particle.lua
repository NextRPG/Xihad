require "math3d"
package.path = package.path..";Assets/script/?.lua"..";Assets/test/?.lua"
local Loader = require "particle.ParticleLoader"

local camObj = g_scene:createObject(c'cam')
local cam = camObj:appendComponent(c'Camera')
cam:setTargetFixed(false)
local ctrl = camObj:appendComponent(c'ControlledMove')
ctrl:acquire()

local cubeMesh = g_geometry:createCube(30, 5, 30)
local floor = g_scene:createObject(c"floor")
local fcomp = floor:appendComponent(c"Mesh")
fcomp:setMesh(cubeMesh)
fcomp:getMaterial(0):setColorMaterial('none')
fcomp:getMaterial(0):setDiffuseColor(Color.new(Color.red))
floor:concatTranslate(math3d.vector(0, -5, 0))
cubeMesh:drop()

local object = g_scene:createObject(c'lalala', camObj)
local pcomp = object:appendComponent(c"ParticleSystem")
Loader.load("Assets.effect.rain", pcomp)

local sun = g_scene:createObject(c'sun')
sun:appendComponent(c'Light')
sun:concatRotate(math3d.vector(90, 0, 0))
sun:concatTranslate(math3d.vector(20, 30, -5))
