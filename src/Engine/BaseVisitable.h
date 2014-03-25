#pragma once
#include "BaseVisitor.h"
#define DEFINE_VISITABLE \
			virtual bool accept(::xihad::ngn::BaseVisitor& guest) \
			{ return acceptImpl(*this, guest); }

namespace xihad { namespace ngn
{
	class BaseVisitable
	{
	public:
		virtual ~BaseVisitable() {}
		virtual bool accept(BaseVisitor&) = 0;

	protected:
		template <typename Derived>
		static bool acceptImpl(Derived& visited, BaseVisitor& guest)
		{
			if (Visitor<Derived>* p = dynamic_cast<Visitor<Derived>*>(&guest))
			{
				p->visit(visited);
				return true;
			}
			else
			{
				return guest.handleUnkownVisitable(visited);
			}
		}
	};
}}

