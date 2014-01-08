#pragma once

namespace xihad { namespace ngn 
{
	class GameObject;
	class MessageListener;
	class SourceFilter;
	class MessageParam;
	class ListenerWrapper
	{
	public:
		explicit ListenerWrapper(MessageListener* listener, SourceFilter* filter = nullptr);

		void receive(GameObject& source, const MessageParam& param);

		bool operator==(const ListenerWrapper& other)
		{
			return mListener == other.mListener;
		}

		bool operator!=(const ListenerWrapper& other)
		{
			return !(*this == other);
		}

	private:
		MessageListener* mListener;
		SourceFilter* mFilter;
	};
}}
