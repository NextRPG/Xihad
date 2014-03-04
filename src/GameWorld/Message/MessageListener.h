#pragma once

namespace xihad { namespace ngn
{
	class MessageParam;
	class GameObject;
	class MessageListener
	{
	public:
		typedef MessageParam Parameter;

	public:
		virtual ~MessageListener() {} 
		virtual void receive(GameObject& pSource, const Parameter& pEvent) = 0;
	};
}}