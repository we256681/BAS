#ifndef MULTITHREADING_H
#define MULTITHREADING_H
#include "include/base/cef_lock.h"
#include "include/wrapper/cef_helpers.h"
#include <string>

struct multithreading
{
    base::Lock _LockBrowserData;
    base::Lock _LockMapBrowserIdToTabNumber;
    base::Lock _LockHttpAuth;
    base::Lock _LockTimezone;
    base::Lock _LockFonts;
    base::Lock _LockPostManager;
};
extern multithreading mthreading;

#define LOCK_BROWSER_DATA base::AutoLock lock_scope(mthreading._LockBrowserData);
#define LOCK_MAP_BROWSER_ID_TO_TAB_NUMBER base::AutoLock lock_scope(mthreading._LockMapBrowserIdToTabNumber);
#define LOCK_HTTP_AUTH base::AutoLock lock_scope(mthreading._LockHttpAuth);
#define LOCK_TIMEZONE base::AutoLock lock_scope(mthreading._LockTimezone);
#define LOCK_FONTS base::AutoLock lock_scope(mthreading._LockFonts);
#define LOCK_POST_MANAGER base::AutoLock lock_scope(mthreading._LockPostManager);

std::string GetCurrentThreadString();


#endif // MULTITHREADING_H
