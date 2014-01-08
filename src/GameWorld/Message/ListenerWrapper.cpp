#include "ListenerWrapper.h"
#include "SourceFilter.h"
#include "CppBase\xassert.h"
#include "MessageListener.h"
#include "MessageParam.h"

namespace xihad { namespace ngn 
{
	ListenerWrapper::ListenerWrapper( MessageListener* listener, SourceFilter* filter /*= nullptr*/ )
		: mListener(listener), mFilter(filter)
	{
		xassert(listener);
	}

	void ListenerWrapper::receive( GameObject& pSource, const MessageParam& pParam )
	{
		if (mFilter == nullptr || mFilter->accept(pSource))
		{
			mListener->receive(pSource, pParam);
		}
	}
}}
