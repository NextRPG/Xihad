#pragma once

namespace xihad { namespace ngn 
{
	class GameObject;
	class SourceFilter
	{
	public:
		virtual ~SourceFilter() {}
		virtual bool accept(const GameObject& pSource) const = 0;
	};
}}
