local h1 = g_scene:createObject(c"hero")
h1:addTag(c"friend")

local h2 = g_scene:createObject(c"hero2")
h2:addTag(c"friend")
h2:addTag(c"dead")

for i=1,10 do
	g_scene:createUniqueObject(c"enemy")	-- optional arg [parent, addHeaderToTags]
end

if g_scene:findObject(c"hero") then 
	print("find hero")
else 
	print("no hero")
end

print("Objects with tag hero:")
for obj in g_scene:objectsWithTag("friend") do
	print(obj:getID())
end
print()

print("Objects with tag enemy")
for obj in g_scene:objectsWithTag("enemy") do
	print(obj:getID())
end
print()
	