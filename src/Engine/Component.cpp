#include "Component.h"
#include "MemoryLeakDetector.h"
#include "GameObject.h"

namespace xihad { namespace ngn
{
	Component::Component( const std::string& typeName, GameObject& host ) : 
		mTypeName(typeName), mHost(&host)
	{
		XIHAD_MLD_NEW_OBJECT;
	}

	Component::~Component()
	{
		XIHAD_MLD_DEL_OBJECT;
	}

	const std::string& Component::getComponentName() const
	{
		return mTypeName;
	}

	GameObject* Component::getHostObject() const
	{
		return mHost;
	}

	Component* Component::findPeer(const std::string& typeName) const
	{
		return mHost->findComponent(typeName);
	}

}}


