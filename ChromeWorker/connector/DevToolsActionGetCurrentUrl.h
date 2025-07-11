#ifndef DEVTOOLSACTIONGETCURRENTURL_H
#define DEVTOOLSACTIONGETCURRENTURL_H

#include "DevToolsMultiAction.h"


class DevToolsActionGetCurrentUrl : public DevToolsMultiAction
{
public:
	virtual void Run();
	virtual void OnActionFinished();
};

#endif // DEVTOOLSACTIONGETCURRENTURL_H