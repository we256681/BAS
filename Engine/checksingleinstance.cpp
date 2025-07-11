#include "checksingleinstance.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    CheckSingleInstance::CheckSingleInstance(QObject *parent) : QObject(parent)
    {

    }

    bool CheckSingleInstance::Check(const QString& Key)
    {
        sharedMemory.setKey(Key);

        // when  can create it only if it doesn't exist
        if (sharedMemory.create(5000))
        {
            sharedMemory.lock();
            *(char*)sharedMemory.data() = '\0';
            sharedMemory.unlock();

            return false;
        }
        // it exits, so we can attach it?!
        else if (sharedMemory.attach()){
            return true;
        }
        else{
            // error
        }

        return true;


    }

}
