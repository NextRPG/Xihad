#include "Component.h"

namespace xihad { namespace ngn
{
	Component::Component( const std::string& typeName, GameObject& host ) : 
		mTypeName(typeName), mHost(&host)
	{
	}

	Component::~Component()
	{
	}

	const std::string& Component::getComponentName() const
	{
		return mTypeName;
	}

	GameObject* Component::getHostObject() const
	{
		return mHost;
	}
}}


