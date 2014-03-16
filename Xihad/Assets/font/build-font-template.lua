local useReleaseVersion = true
local fontFileDir 		= 'F:/Projects/JAVA/_magold/assets/font'
local fontFileName		= 'fangzheng.ttf'
local outputFilePrefix	= string.sub(fontFileName, 1, string.find(fontFileName, '%.')-1)

--- 
-- Font size in pixel
local fontPixelWidth 	= 20
local fontPixelHeight 	= 0		-- 0 means equal to width

---
-- Single image size in pixel
local singleImageWidth 	= 2048
local singleImageHeight = 1024	-- 0 means equal to width

---
-- Image format
local outputImageFormat = 'bmp'

------------------------------------------------------------------------------------
local pathToExe
if useReleaseVersion then
	pathToExe = '../../../Release/FontTool.exe'
else
	pathToExe = '../../../Debug/FontTool.exe'
end

local paramList = { 
	pathToExe, fontFileDir..'/'..fontFileName, 
	'-o'..outputFilePrefix, 
	'-w'..fontPixelWidth, '-h'..fontPixelHeight, 
	'-W'..singleImageWidth, '-H'..singleImageHeight, 
	'-f'..outputImageFormat
}

local arg = table.concat( paramList, " ")
arg = string.gsub(arg, '/', '\\')

print("Executing: ", arg)
os.execute(arg)
