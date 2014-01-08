#pragma once

namespace xihad { namespace ngn
{
	class BaseVisitable;
	class BaseVisitor
	{
	public:
		virtual ~BaseVisitor() {}
		virtual bool handleUnkownVisitable(BaseVisitable&) = 0;
	};

	template <typename T>
	class Visitor
	{
	public:
		virtual void visit(T&) = 0;
	};
}}

