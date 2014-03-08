#pragma once
#include <CEGUI\String.h>

class ITypedContent
{
public:
	virtual ~ITypedContent() { }

	virtual const CEGUI::String& getType() const = 0;
};

