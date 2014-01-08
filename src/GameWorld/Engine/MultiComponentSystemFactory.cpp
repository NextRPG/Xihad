#include "MultiComponentSystemFactory.h"
#include "GameScene.h"

using namespace std;
namespace xihad { namespace ngn
{
	MultiComponentSystemFactory::MultiComponentSystemFactory( const string& mainSystem ) :
		mBaseType(mainSystem)
	{
		registerType(mainSystem);
	}

	ComponentSystem* MultiComponentSystemFactory::create( 
		GameScene* scene, const string& typeName )
	{
		if (isBaseType(typeName))
		{
			return createMainSystem(scene);
		}
		else if (containsType(typeName))
		{
			return scene->requireSystem(mBaseType);
		}
		else
		{
			return nullptr;
		}
	}

	InheritenceTree::RegisterResult MultiComponentSystemFactory::registerDerivedType( const std::string& sysname )
	{
		return registerType(sysname, mBaseType);
	}

}}

