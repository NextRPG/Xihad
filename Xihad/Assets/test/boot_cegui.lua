package.cpath = package.cpath ..";../Debug/?.dll" --.."../Debug/?.dll"
require "cegui"

local guiUpdater = createCEGUIUpdateHandler(engine:getWindow())
scene:appendUpdateHandler(guiUpdater)
scene:pushController(guiUpdater:getEventReceiver())
