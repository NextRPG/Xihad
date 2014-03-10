#pragma once

namespace xihad { namespace ngn
{
	class GameScene;
	class TagListener;
	class ComponentFactory;
	struct GameObjectDepends
	{
	public:
		GameScene*			scene;
		TagListener*		listener;
		ComponentFactory*	factory;
	};
}}

