local h1 = scene:createObject(c"hero")
h1:addTag(c"friend")

local h2 = scene:createObject(c"hero2")
h2:addTag(c"friend")
h2:addTag(c"dead")

for i=1,10 do
	scene:createUniqueObject(c"enemy")	-- optional arg [parent, addHeaderToTags]
end

if scene:findObject(c"hero") then 
	print("find hero")
else 
	print("no hero")
end

print("Objects with tag hero:")
for obj in scene:objectsWithTag("friend") do
	print(obj:getID())
end
print()

print("Objects with tag enemy")
for obj in scene:objectsWithTag("enemy") do
	print(obj:getID())
end
print()
	