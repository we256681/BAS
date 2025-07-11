#include "devtoolsactionempty.h"

void DevToolsActionEmpty::Run()
{
  State = Finished;
  Result->Success();
}
