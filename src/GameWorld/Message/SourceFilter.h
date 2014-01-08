#pragma once

namespace xihad { namespace ngn 
{
	class SourceFilter
	{
	public:
		virtual ~SourceFilter() {}
		virtual bool accept(GameObject& pSource) const = 0;
	};
}}
