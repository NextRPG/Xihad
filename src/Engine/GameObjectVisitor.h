#pragma once

namespace xihad { namespace ngn
{
	class GameObject;
	class GameObjectVisitor
	{
	public:
		virtual ~GameObjectVisitor() {}
		virtual void onVisitObject(GameObject& obj) = 0;
	};
}}

