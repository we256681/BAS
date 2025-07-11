#include "systemtraynotifier.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    SystemTrayNotifier::SystemTrayNotifier(QObject *parent) :
        ISystemTrayNotifier(parent)
    {
        TrayIcon = new QSystemTrayIcon(this);

        connect(TrayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),SLOT(Activated(QSystemTrayIcon::ActivationReason)));
        //Movie = new QMovie(":/engine/images/LogoAnimation.gif",QByteArray(), this);

        //connect(Movie, SIGNAL(frameChanged(int)), this, SLOT(UpdateIcon()));
        //IsAnimated = false;
        CustomIcon = false;

    }
    void SystemTrayNotifier::SetCustomIcon(QIcon Icon)
    {
        CustomIcon = true;
        TrayIcon->setIcon(Icon);
        //Movie->stop();
        //disconnect(Movie, SIGNAL(frameChanged(int)), this, SLOT(UpdateIcon()));
    }


    void SystemTrayNotifier::ShowMessage(const QString& title,const QString& message)
    {
        TrayIcon->showMessage(title,message);
    }

    void SystemTrayNotifier::Activated(QSystemTrayIcon::ActivationReason Reason)
    {
        switch(Reason)
        {
            case QSystemTrayIcon::Trigger:
                emit Show();
            break;
        }
    }

    void SystemTrayNotifier::SetContextmenu(QMenu * menu)
    {
        TrayIcon->setContextMenu(menu);
    }

    void SystemTrayNotifier::UpdateIcon()
    {
        /*if(CustomIcon)
            return;
        if(IsAnimated)
        {
            TrayIcon->setIcon(Movie->currentPixmap());
        }*/
    }

    void SystemTrayNotifier::ShowAnimation()
    {
        /*TrayIcon->show();

        if(CustomIcon)
            return;
        IsAnimated = true;
        Movie->start();*/

    }

    void SystemTrayNotifier::ShowStaticPicture()
    {
        /*

        if(CustomIcon)
            return;
        IsAnimated = false;
        Movie->stop();

        TrayIcon->setIcon(QIcon(":/engine/images/Logo.png"));*/
    }

    void SystemTrayNotifier::Init()
    {
        if(!CustomIcon)
            TrayIcon->setIcon(QIcon(":/engine/images/Logo.png"));
        TrayIcon->show();

        ShowStaticPicture();
    }
    void SystemTrayNotifier::Start()
    {
        ShowAnimation();
    }
    void SystemTrayNotifier::Stop()
    {
        ShowStaticPicture();
    }
}
