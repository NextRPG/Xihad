local dialogue = Dialogue.new()
-- dialogue:setNextEvent('left-click')

local speaker = dialogue:addSpeaker('name', icon, getPosition)
-- speaker:setIcon()


----
-- 1. 显示对话内容，对话框位置
-- 2. 显示头像，头像横轴相对位置
-- 3. 配置速度，即时显示
-- 4. 关闭对话框
-- 5. callback
function Speaker:speak(sentence)
	getPosition()
	show(sentence, x, y)
	setIconX(0)
	self.condition:wait()
end

speaker:speak('fsdgjlaksjglksdjalgdsj')
speaker:speak('fsdgjlaksjglksdjalgdsj')
speaker:speak('fsdgjlaksjglksdjalgdsj')
