#ifndef WEBINTERFACERESOURCECONTROLLER_H
#define WEBINTERFACERESOURCECONTROLLER_H

#include "engine_global.h"
#include "iresourcecontroller.h"
#include <QVariantMap>

using namespace BrowserAutomationStudioFramework;

class ENGINESHARED_EXPORT WebInterfaceResourceController : public IResourceController
{
    Q_OBJECT
    QVariantMap Data;
public:
    explicit WebInterfaceResourceController(QObject *parent = 0);
    QVariantMap GetData();
    void SetData(const QVariantMap& Data);
    virtual void FromViewToModel(IResources * resources, bool Clear = true);
    virtual void FromModelToView(IResources * resources, bool Clear = true);

};


#endif // WEBINTERFACERESOURCECONTROLLER_H
