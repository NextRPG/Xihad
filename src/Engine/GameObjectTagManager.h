#pragma once
#include "GameObjectDepends.h"
#include <unordered_map>
#include <boost/array.hpp>
#include <set>
#include "GameObject.h"

namespace xihad { namespace ngn
{
	class GameObjectTagManager : public GameObjectDepends::TagManagerT
	{
	private:
		typedef std::set<Object*> ObjectGroup;
		typedef std::vector<ObjectGroup> TaggedObjectsMap;

	public:
		GameObjectTagManager();

		virtual void onTagAdded(Object* obj, unsigned tagIndex);
		
		virtual void onTagRemoved(Object* obj, unsigned tagIndex);
		
		virtual unsigned unprojectTag(const Tag& tag, bool addIfDontExists) const;

		virtual const ObjectGroup* findObjectsByTag(const Tag& tag) const;
		
		// clear all tags, only use it when you're to destroy the scene
		void clear();

		virtual unsigned getUsedTagCount() const
		{
			return tag2Index.size();
		}

	private:
		const std::string& checkTag(unsigned tagIndex);

	private:
		mutable TaggedObjectsMap taggedObjects;
		mutable boost::array<Tag, MaxTagCount> index2Tag;	// for checking purpose
		mutable std::unordered_map<Tag, unsigned> tag2Index;
	};
}}

