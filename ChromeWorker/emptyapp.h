#ifndef EMPTYAPP_H
#define EMPTYAPP_H

#include "include/cef_app.h"


class EmptyApp : public CefApp {
 public:
    EmptyApp();

 private:
  IMPLEMENT_REFCOUNTING(EmptyApp);
  DISALLOW_COPY_AND_ASSIGN(EmptyApp);
};

#endif // EMPTYAPP_H
