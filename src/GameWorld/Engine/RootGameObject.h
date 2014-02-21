#pragma once
#include "GameObject.h"

namespace xihad { namespace ngn
{
	class RootGameObject : public GameObject
	{
	public:
		RootGameObject(GameObjectDepends* depends, IdArgType id) :
			GameObject(depends, id)
		{
		}

		virtual bool identityParent() const override
		{
			return true;
		}

		virtual void setParent(GameObject* parent) override
		{
			GameObject::setParent(nullptr);
		}

		virtual void resetScale(const vector3df& s) override
		{
		}

		virtual void resetRotate(const vector3df& r) override
		{
		}

		virtual void resetTranslate(const vector3df& t) override
		{
		}

	};
}}

