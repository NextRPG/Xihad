package.cpath = '../Release/?.dll;'
 local m3d = require "math3d"

--[[
VECTOR3D
--]]
-- constructors and setter
local v000 = m3d.vector()		-- 全0
local v222 = m3d.vector(2)		-- 全2
local v123 = m3d.vector(1, 2, 3)-- 1, 2, 3
local cpy2 = v123:copy()		-- 1, 2, 3
v123:set(nil, 4)	-- only change y value, keep x, z
v123:set(nil, 2, 3)	-- keep x, reset y and z

-- 算术操作符重载
local v345 = v123 + v222
local v101 = v123 - v222
local v246 = v123 * v222
local vhal = v123 / v222
local vneg = -v123

math3d.lerp(v222, v345, 0.5)

-- 比较操作符重载
assert (v123  < v222)
assert (not (v222 <= v123))
assert (not (v123 == v222))
assert (not (v123 > v222))
assert (v222 >= v123)

-- 操作函数
assert(math.abs(v123:length() - 14^0.5) < 0.01)
assert(v123:length2() == 14)
assert(math.abs(v123:distance(v000) - 14^0.5) < 0.01)
assert(v123:distance2(v000) == 14)
assert(v123:dot(v123) == 14)
v123:cross(v123) -- result == (0, 0, 0)
v123:normalize(); assert(v123:length() == 1); v123:set(1, 2, 3) -- normal self

-- rotate(axis, angle[, center])
local v001 = m3d.vector(0, 0, 1)
local axis = "y"; 				v001:rotate(axis, 90)	-- 以原点为旋转中心，绕y轴旋转90度
local axis = m3d.vector(0,1,0); v001:rotate(axis, 90, m3d.vector(0, 0, -1)) -- 以(0,0,-1)为旋转中心，绕(0,1,0)旋转90度

local dirToTarget = m3d.vector(1, 1, 0)
-- get the euler angle which will rotate (0, 0, 1) vector to face the target dir
local eulerVec = dirToTarget:horizontalAngle()

--[[
QUATERNION
--]]
local identity = m3d.quaternion()					-- 不做任何旋转的四元数
local q1 = m3d.quaternion(45, 90, 120)				-- 将欧拉角转化为四元数
local q2 = m3d.quaternion(60, m3d.vector(1,2,3))	-- 绕(1,2,3)向量旋转60°的四元数
local q3 = m3d.quaternion(m3d.vector(1,0,0), m3d.vector(0,1,0))	-- 将 vec1 旋转到 vec2 的四元数
local q4 = m3d.quaternion(1,2,3,4)	-- 直接设置四元数 x,y,z,w ，不建议使用
local qc = m3d.quaternion(q1)

qc:set(45, 90, 120)				-- 将欧拉角转化为四元数
qc:set(60, m3d.vector(1,2,3))	-- 绕(1,2,3)向量旋转60°的四元数
qc:set(m3d.vector(1,0,0), m3d.vector(0,1,0))	-- 将 vec1 旋转到 vec2 的四元数
qc:set(q1)
qc:set(1,2,3,4)	-- 直接设置四元数 x,y,z,w ，不建议使用
local qc = q1:copy()

local x,y,z,w = qc:xyzw()	-- 取得 x,y,z,w
qc:makeIdentity()
qc:makeInverse()
qc:normalize()		-- 一般不需要主动 normalize ，除非直接设置 x,y,z,w 的值

local a,b = q2:toAngleAxis()	-- a == 60, b 平行于(1,2,3)
local rx, ry, rz = q2:toEuler()	-- 等价的欧拉角表示

-- 四元数插值
local q3 = q1:lerpTo(q2, 0.3)	-- 0~1
local q4 = q1:slerpTo(q2, 0.4)
q3:set(q1)
q3:set(45,90,120)
q3:set(90, m3d.vector(0, 1, 0))
local rotatedVec = q3*m3d.vector(0, 0, 1)	-- 旋转向量

-- local mat1 = m3d.matrix()
-- local mat2 = m3d.matrix()
-- local mat = mat1 * mat2
-- local out = mat * m3d:vector()
-- mat:transform(out)
-- mat:setTranslate()
-- mat:setScale()
-- mat:setRorate()
-- local mt = mat:transpose()
-- local mi = mat:inverse() -- can be nil


-- 插值函数，支持 vector, quaternion 的插值
local v222 = m3d.lerp(m3d.vector(1,1,1), m3d.vector(3,3,3), 0.5)
local q = m3d.lerp(q1, q2, 0.2)

--[[
Axis-Aligned Bounding-Box
--]]
local aabb = m3d.aabb(m3d.vector(0, 0, 1)) -- box contain a point (0, 0, 1)
local copy = aabb:copy()
aabb:addPoint(m3d.vector(1, 0, 0))
assert(aabb:center() == m3d.vector(0.5, 0, 0.5))-- (maxEdge - minEdge) / 2
assert(aabb:extent() == m3d.vector(1, 0, 1)) 	-- (maxEdge - minEdge)
assert(aabb:volume() == 0)	-- 体积
assert(not aabb:empty())	-- minEdge == maxEdge
assert(aabb:area() == 2)	-- 表面积
assert(aabb:minEdge() == m3d.vector())
assert(aabb:maxEdge() == m3d.vector(1, 0, 1))
aabb:addBox(copy)

assert(aabb:contains(m3d.vector(0,0,0)))		-- Test contains point
--assert(aabb:contains(m3d.line(```)))			-- Test contains line
assert(aabb:contains(copy))						-- Test contains box
assert(not aabb:fullContains(m3d.vector(0,0,0)))-- Test full contains point (no face)
--assert(aabb:fullContains(m3d.line(```)))		-- Test full contains line (no face)
assert(not aabb:fullContains(copy))				-- Test full contains box (no face)
--assert(aabb:intersects(m3d.line(```)))		-- Test intersects line
assert(aabb:intersects(copy))					-- Test intersects box
--assert(aabb:intersects(m3d.plane(```)))		-- Test intersects plane

local line = m3d.line(m3d.vector(0, 0, 0), m3d.vector(1, 1, 1)) -- 线段，开始点为 (0,0,0) 结束点为 (1, 1, 1)
local copy = line:copy()
line:vector()	-- (end-start)
line:middle()	-- (end+start)/2
line:length()	-- line:vector():length()
line:length2()	-- line:vector():length2()
line:closestP2P(m3d.vector(0, 1, 0))	-- 线段上离指定点最近的点
line:start():set(1,2,3)		-- 修改线段的起点
line:finish():set(2,3,4)	-- 修改线段的终点

local point = m3d.vector(1, 2, 3)
local normal = m3d.vector(1, 1, 1); normal:normalize();
local plane = m3d.plane(point, normal)
local p2 = m3d.plane(normal, 5)	-- 法向量为 normal ，与原点距离为 5 （法向量方向为负方向）
local p3 = m3d.plane(point, m3d.vector(1,1,1), m3d.vector(2,2,2)) -- 三点确定一个平面
local lookdir = -normal
assert(plane:isFrontFacing(lookdir))
assert(plane:distanceToPoint(point) == 0)
assert(math.abs(normal:dot(plane:memberPoint()-point)) < 0.001)
assert(plane:relationToPoint(point) == "planar")
assert(plane:relationToPoint(m3d.vector(2,3,4)) == "front")
assert(plane:relationToPoint(m3d.vector(0,1,2)) == "back")
-- plane:set(```) 参数同构造函数的参数

print("Test Complete")
os.execute("pause")