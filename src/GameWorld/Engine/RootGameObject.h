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

		virtual void resetScale(float sx, float sy, float sz) override
		{
		}

		virtual void resetRotate(float rx, float ry, float rz) override
		{
		}

		virtual void resetTranslate(float tx, float ty, float tz) override
		{
		}

	};
}}

