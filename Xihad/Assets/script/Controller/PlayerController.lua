local StateMachine = require 'StateMachine'


function init()
	sm = StateMachine.new('ChooseHero')
	sm:setTransition('ChooseHero', 'next', 'ChooseTile')
	
	sm:setTransition('ChooseTile', 'next', 'ChooseCommand')
	sm:setTransition('ChooseTile', 'back', 'ChooseHero')
	sm:setTransition('ChooseTile', 'fail', 'ChooseHero')
	
	sm:setTransition('ChooseCommand', 'next', 'ChooseTarget')
	sm:setTransition('ChooseCommand', 'back', 'ChooseTile')
	sm:setTransition('ChooseCommand', 'fail', 'ChooseHero')
	
	sm:setTransition('ChooseTarget', 'next', 'ChooseHero')
	sm:setTransition('ChooseTarget', 'back', 'ChooseCommand')
	sm:setTransition('ChooseTarget', 'fail', 'ChooseHero')
end
