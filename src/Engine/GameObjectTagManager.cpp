#include "GameObjectTagManager.h"
#include <CppBase\xassert.h>
#include <CppBase\StdMap.h>
#include <CppBase\XiAssert.h>
#include "GameObject.h"

using namespace std;
namespace xihad { namespace ngn
{
	GameObjectTagManager::GameObjectTagManager() :
		tag2Index(MaxTagCount*2) { }


	void GameObjectTagManager::onTagAdded(Object* obj, unsigned tagIndex)
	{
		xassert(obj->hasTag(checkTag(tagIndex)));

		auto res = taggedObjects[tagIndex].insert(obj);

		xassert(res.second && "The GameObject already has this tag"); // insert success
	}

	void GameObjectTagManager::onTagRemoved(Object* obj, unsigned tagIndex)
	{
		xassert(!obj->hasTag(checkTag(tagIndex)));

		// Maybe clear have been invoked
		if (!taggedObjects[tagIndex].empty()) 
			XiAssert::areEqual(taggedObjects[tagIndex].erase(obj), 1u);
	}

	void GameObjectTagManager::clear()
	{
		taggedObjects.clear();
		taggedObjects.resize(getUsedTagCount());
	}

	const GameObjectTagManager::ObjectGroup* 
		GameObjectTagManager::findObjectsByTag(const Tag& tag) const
	{
		unsigned tagIndex = unprojectTag(tag, false);
		
		if (tagIndex == InvalidIndex) return 0;

		return taggedObjects[tagIndex].empty() ? 0 : &taggedObjects[tagIndex];
	}

	unsigned GameObjectTagManager::unprojectTag( const Tag& tag, bool addIfDontExists ) const
	{
		if (const unsigned* index = StdMap::findValuePtr(tag2Index, tag))
			return *index;

		if (!addIfDontExists) return InvalidIndex;

		unsigned newIndex = tag2Index[tag] = tag2Index.size();
		if (newIndex >= MaxTagCount)
			throw std::exception("Tag count exceed max tag count");

		taggedObjects.push_back(ObjectGroup());
		index2Tag[newIndex] = tag;
		return newIndex;
	}

	const std::string& GameObjectTagManager::checkTag( unsigned tagIndex )
	{
		xassert(tagIndex < tag2Index.size());
		
		return index2Tag[tagIndex];
	}

}}

