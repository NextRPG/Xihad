local pathToExe			= '../../../Release/FontTool.exe'
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
-- + Relative loading cost for all supported formats: 
-- 		ppm(very slow) > pcx(341) > png(40.6)  > jpg(25.8) > tga(1.3) > bmp(0.8)
-- + Image size: 
--		png(1.69M) < tga(8.00M)
-- 		jpg(1.02M) < pcx(4.82M) < bmp(6M) < ppm(25M)
-- + Formats that support alpha channel
--		png, tga
------ Don't use ppm and pcx
------ If you does 	wanna alpha, choose from { png, tga }
------ If you don't wanna alpha, choose from { jpg, bmp }
local outputImageFormat = 'tga'
local useAlphaChannel 	= true

---
-- Font file format. Use binary can speed up font loading, but not readable for debug purpose
local outputBinary		= true

------------------------------------------------------------------------------------
local paramList = { 
	pathToExe, fontFileDir..'/'..fontFileName, 
	'-o'..outputFilePrefix, 
	'-w'..fontPixelWidth, 	'-h'..fontPixelHeight, 
	'-W'..singleImageWidth, '-H'..singleImageHeight, 
	'-f'..outputImageFormat, 
	outputBinary and '-b' or '-B',
	useAlphaChannel and '-a' or '-A',
}

local arg = table.concat( paramList, " ")
arg = string.gsub(arg, '/', '\\')

print("Executing: ", arg)
os.execute(arg)
