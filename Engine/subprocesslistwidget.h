#ifndef SUBPROCESSLISTWIDGET_H
#define SUBPROCESSLISTWIDGET_H
#include "engine_global.h"


#include <QCheckBox>
#include <QWidget>
#include "flowlayout.h"
#include "iprocesscomunicator.h"
#include <QHash>
#include <QLabel>
#include <QScrollArea>

#include "browserlistitem.h"
namespace Ui {
class SubprocessListWidget;
}


#include "iskincontroller.h"

namespace BrowserAutomationStudioFramework
{

    class ENGINESHARED_EXPORT SubprocessListWidget : public QWidget
    {
        Q_OBJECT
        FlowLayout *Layout;
        QHash<BrowserListItem *,IProcessComunicator*> Data;
        QHash<int, BrowserListItem *> Indexes;
        bool AutoStart;
        bool BrowsersHidden;

    public:
        explicit SubprocessListWidget(QWidget *parent = 0);
        ~SubprocessListWidget();
        void Clear();
        void Add(IProcessComunicator* Communicator, int index);
        void SetAutoStart(bool AutoStart);
        void SetBrowsersHidden(bool BrowsersHidden);


    private:
        Ui::SubprocessListWidget *ui;
        void changeEvent(QEvent *e);
        void UpdateItemPosition(BrowserListItem *Item);
    private slots:
        void ChangedCheckState(bool state);
        void Received(const QString & Message);
        void CommunicatorDestroyed();
    };
}
#endif // SUBPROCESSLISTWIDGET_H
