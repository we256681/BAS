#include "webinterfaceapi.h"
#include <QDir>
#include <QStorageInfo>
#include <QXmlStreamReader>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QFileInfo>
#include <windows.h>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    WebInterfaceApi::WebInterfaceApi(QObject *parent) : QObject(parent)
    {
        WebInterface = 0;
        Worker = 0;
        ReportDataVisualizer = 0;
        ResourcesVisualizer = 0;
        DatabaseState = 0;
        DatabaseConnector = 0;
    }

    void WebInterfaceApi::FileDialogFinished(int Result)
    {

        QFileDialog * Dialog = qobject_cast<QFileDialog *>(sender());

        Dialog->deleteLater();

        QStringList ResultStringList = Dialog->selectedFiles();
        QString ResultString;
        if(!ResultStringList.empty())
            ResultString = ResultStringList.first();


        if(!ResultString.isEmpty())
        {
            LastFolder = QFileInfo(ResultString).absoluteDir().absolutePath();
        }

        QString MessageType = Dialog->property("MessageType").toString();
        bool IsAsync = Dialog->property("IsAsync").toBool();
        int Id = Dialog->property("Id").toInt();

        WebInterface->SendRaw(MessageType,IsAsync,Id,ResultString);
    }

    void WebInterfaceApi::Received(const QString& value)
    {
        QString data;
        int x = 0,y = 0,width = 0,height = 0;
        int id = 0;
        bool found = false;
        bool foundtabs = false;
        QString json;
        int idtabs = 0;
        QXmlStreamReader xmlReader(value);
        while(!xmlReader.atEnd() && !xmlReader.hasError())
        {

            QXmlStreamReader::TokenType token = xmlReader.readNext();

            if(xmlReader.name() == "View" && token == QXmlStreamReader::StartElement)
            {
                found = true;
                foreach(QXmlStreamAttribute attr, xmlReader.attributes())
                {
                    if(attr.name() == "data")
                    {
                        data = attr.value().toString();
                    }
                    if(attr.name() == "id")
                    {
                        id = attr.value().toInt();
                    }
                    if(attr.name() == "x")
                    {
                        x = attr.value().toInt();
                    }

                    if(attr.name() == "y")
                    {
                        y = attr.value().toInt();
                    }

                    if(attr.name() == "width")
                    {
                        width = attr.value().toInt();
                    }

                    if(attr.name() == "height")
                    {
                        height = attr.value().toInt();
                    }

                }
            }

            if(xmlReader.name() == "GetTabs" && token == QXmlStreamReader::StartElement)
            {
                foundtabs = true;
                foreach(QXmlStreamAttribute attr, xmlReader.attributes())
                {
                    if(attr.name() == "id")
                    {
                        idtabs = attr.value().toInt();
                    }
                }
                xmlReader.readNext();
                json = xmlReader.text().toString();
            }


            if(SendHideBrowserEvent && xmlReader.name() == "Minimized" && token == QXmlStreamReader::StartElement)
            {
                IProcessComunicator* Communicator = qobject_cast<IProcessComunicator*>(sender());
                int BrowserId = Communicator->property("BrowserId").toInt();

                WebInterface->Send("hide_browser",false,0,"browser_id",BrowserId);

                return;
            }


            if(xmlReader.name() == "ManualControlIndicatorStart" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Message = xmlReader.text().toString();
                IProcessComunicator* Communicator = qobject_cast<IProcessComunicator*>(sender());
                int BrowserId = Communicator->property("BrowserId").toInt();
                if(!WebInterface)
                    return;
                WebInterface->Send("manual_control_start",false,0,"browser_id",BrowserId,"message",Message);
                return;
            }

            if(xmlReader.name() == "ManualControlIndicatorStop" && token == QXmlStreamReader::StartElement)
            {
                IProcessComunicator* Communicator = qobject_cast<IProcessComunicator*>(sender());
                int BrowserId = Communicator->property("BrowserId").toInt();
                if(!WebInterface)
                    return;
                WebInterface->Send("manual_control_end",false,0,"browser_id",BrowserId);
                return;
            }
        }
        if(found)
        {
            QMutableMapIterator<IProcessComunicator*, BrowserViewData> i(BrowserViewMap);
            while (i.hasNext())
            {
                i.next();
                if(i.value().RequestId == id && i.value().Active)
                {
                    if(WebInterface)
                    {
                        QVariantMap Res;
                        Res["image"] = data;
                        Res["width"] = width;
                        Res["height"] = height;
                        Res["x"] = x;
                        Res["y"] = y;
                        WebInterface->SendRaw("view_browser",true,i.value().RequestId,Res);
                    }
                    i.value().Active = false;
                    i.value().RequestId = -1;

                    break;
                }
            }
        }
        if(foundtabs)
        {
            QMutableMapIterator<IProcessComunicator*, BrowserViewData> i(BrowserViewMap);
            while (i.hasNext())
            {
                i.next();
                if(i.value().GetTabsRequestId == idtabs && i.value().GetTabsActive)
                {
                    if(WebInterface)
                    {
                        QVariantMap Res;
                        Res["json"] = json;
                        WebInterface->SendRaw("get_tabs",true,i.value().GetTabsRequestId,Res);
                    }
                    i.value().GetTabsActive = false;
                    i.value().GetTabsRequestId = -1;

                    break;
                }
            }
        }

    }

    void WebInterfaceApi::SetSendHideBrowserEvents()
    {
        this->SendHideBrowserEvent = true;
    }


    void WebInterfaceApi::ShowBrowser()
    {
        IProcessComunicator* Communicator = qobject_cast<IProcessComunicator*>(sender());
        int BrowserId = Communicator->property("BrowserId").toInt();
        if(!WebInterface)
            return;
        WebInterface->Send("show_browser",false,0,"browser_id",BrowserId);
    }

    void WebInterfaceApi::HideBrowser()
    {
        IProcessComunicator* Communicator = qobject_cast<IProcessComunicator*>(sender());
        int BrowserId = Communicator->property("BrowserId").toInt();
        if(!WebInterface)
            return;
        WebInterface->Send("hide_browser",false,0,"browser_id",BrowserId);
    }


    void WebInterfaceApi::ThreadStarted()
    {
        if(!WebInterface)
            return;

        //From now on can send "start" event.
        IsFirstThreadStarted = true;

        //There was event, which didn't fired because of no thread has been started yet.
        if(IsPendingScriptStart)
        {
            ScriptStarted();
        }

        WebInterface->Send("thread_start",false,0);
    }

    void WebInterfaceApi::ScriptStarted()
    {
        if(!WebInterface)
            return;

        //Remove old pending "start" event, if any exist.
        IsPendingScriptStart = false;


        if(IsFirstThreadStarted)
        {
            //Thread already has been started, can send.
            WebInterface->Send("start",false,0);
        }
        else
        {
            //Thread has not yet been started, send later.
            IsPendingScriptStart = true;
        }

    }

    void WebInterfaceApi::ThreadSuccess()
    {
        if(!WebInterface)
            return;
        WebInterface->Send("thread_end",false,0,"success",true);
    }
    void WebInterfaceApi::ThreadFail(const QString& Message)
    {
        if(!WebInterface)
            return;

        if(Message == "Ok")
        {
            //Async function
            WebInterface->Send("thread_end",false,0,"success",true);
        }else
        {
            WebInterface->Send("thread_end",false,0,"success",false);
        }
    }

    void WebInterfaceApi::Init(IWebInterface *WebInterface)
    {
        this->WebInterface = WebInterface;
        disconnect(WebInterface,SIGNAL(ReceivedMessage(QString,QVariantMap,bool,int)),this,SLOT(ReceivedMessage(QString,QVariantMap,bool,int)));
        connect(WebInterface,SIGNAL(ReceivedMessage(QString,QVariantMap,bool,int)),this,SLOT(ReceivedMessage(QString,QVariantMap,bool,int)), Qt::QueuedConnection);
    }

    void WebInterfaceApi::SetMainWindow(QWidget * MainWindow)
    {
        this->MainWindow = MainWindow;
    }

    void WebInterfaceApi::ShowMainWindow()
    {
        if(!this->MainWindow)
        {
            return;
        }
        SwitchToThisWindow((HWND)MainWindow->winId(),true);
        HWND hCurWnd = ::GetForegroundWindow();
        DWORD dwMyID = ::GetCurrentThreadId();
        DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
        ::AttachThreadInput(dwCurID, dwMyID, TRUE);
        ::SetWindowPos((HWND)MainWindow->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ::SetWindowPos((HWND)MainWindow->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        ::SetForegroundWindow((HWND)MainWindow->winId());
        ::AttachThreadInput(dwCurID, dwMyID, FALSE);
        ::SetFocus((HWND)MainWindow->winId());
        ::SetActiveWindow((HWND)MainWindow->winId());
    }

    void WebInterfaceApi::RunTaskResult(int Id, QString Result)
    {
        if(!WebInterface)
            return;
        WebInterface->SendRaw("run_task",true,Id,Result);

    }


    void WebInterfaceApi::SetWorker(IMultiWorker *Worker)
    {
        this->Worker = Worker;
        if(Worker)
        {
            disconnect(Worker,SIGNAL(ThreadStarted()),this,SLOT(ThreadStarted()));
            disconnect(Worker,SIGNAL(ThreadFail(QString)),this,SLOT(ThreadFail(QString)));
            disconnect(Worker,SIGNAL(ThreadSuccess()),this,SLOT(ThreadSuccess()));
            disconnect(Worker,SIGNAL(WebInterfaceEvalSignal(QString)),this,SLOT(WebInterfaceEvalSignal(QString)));
            disconnect(Worker,SIGNAL(RunTaskResult(int,QString)),this,SLOT(RunTaskResult(int,QString)));
            disconnect(this,SIGNAL(StartThread(int)),Worker,SLOT(StartThreadWebInterface(int)));
            disconnect(this,SIGNAL(StopThread(int)),Worker,SLOT(StopThreadWebInterface(int)));
            disconnect(this,SIGNAL(RunTaskInThread(int,int,QString,QString)),Worker,SLOT(RunTaskInThreadWebInterface(int,int,QString,QString)));
            connect(Worker,SIGNAL(ThreadStarted()),this,SLOT(ThreadStarted()));
            connect(Worker,SIGNAL(ThreadFail(QString)),this,SLOT(ThreadFail(QString)));
            connect(Worker,SIGNAL(ThreadSuccess()),this,SLOT(ThreadSuccess()));
            connect(Worker,SIGNAL(WebInterfaceEvalSignal(QString)),this,SLOT(WebInterfaceEvalSignal(QString)));
            connect(Worker,SIGNAL(RunTaskResult(int,QString)),this,SLOT(RunTaskResult(int,QString)));
            connect(this,SIGNAL(StartThread(int)),Worker,SLOT(StartThreadWebInterface(int)));
            connect(this,SIGNAL(RunTaskInThread(int,int,QString,QString)),Worker,SLOT(RunTaskInThreadWebInterface(int,int,QString,QString)));
            connect(this,SIGNAL(StopThread(int)),Worker,SLOT(StopThreadWebInterface(int)));


        }
        if(Worker && Worker->GetLogger())
        {
            LastLogPath = Worker->GetLogger()->GetFileName();
        }
        if(Worker)
        {
            LastResultPathes.clear();
            for(int i = 0;i<9;i++)
            {
                LastResultPathes.append(QString());
            }
            if(Worker->GetResults1())
                LastResultPathes[0] = Worker->GetResults1()->GetFileName();
            if(Worker->GetResults2())
                LastResultPathes[1] = Worker->GetResults2()->GetFileName();
            if(Worker->GetResults3())
                LastResultPathes[2] = Worker->GetResults3()->GetFileName();
            if(Worker->GetResults4())
                LastResultPathes[3] = Worker->GetResults4()->GetFileName();
            if(Worker->GetResults5())
                LastResultPathes[4] = Worker->GetResults5()->GetFileName();
            if(Worker->GetResults6())
                LastResultPathes[5] = Worker->GetResults6()->GetFileName();
            if(Worker->GetResults7())
                LastResultPathes[6] = Worker->GetResults7()->GetFileName();
            if(Worker->GetResults8())
                LastResultPathes[7] = Worker->GetResults8()->GetFileName();
            if(Worker->GetResults9())
                LastResultPathes[8] = Worker->GetResults9()->GetFileName();
        }

    }

    void WebInterfaceApi::SetReportDataVisualizer(IReportDataVisualizer *ReportDataVisualizer)
    {
        this->ReportDataVisualizer = ReportDataVisualizer;
    }
    void WebInterfaceApi::SetResourcesVisualizer(IResourcesVisualizer *ResourcesVisualizer)
    {
        this->ResourcesVisualizer = ResourcesVisualizer;
    }

    void WebInterfaceApi::SetBrowserFactory(IBrowserFactory *BrowserFactory)
    {
        disconnect(BrowserFactory,SIGNAL(BrowserStart(IProcessComunicator*,int)),this,SLOT(BrowserStart(IProcessComunicator*,int)));
        disconnect(BrowserFactory,SIGNAL(BrowserStop(IProcessComunicator*,int)),this,SLOT(BrowserStop(IProcessComunicator*,int)));
        connect(BrowserFactory,SIGNAL(BrowserStart(IProcessComunicator*,int)),this,SLOT(BrowserStart(IProcessComunicator*,int)));
        connect(BrowserFactory,SIGNAL(BrowserStop(IProcessComunicator*,int)),this,SLOT(BrowserStop(IProcessComunicator*,int)));
    }
    void WebInterfaceApi::SetDatabaseState(IDatabaseState *DatabaseState)
    {
        this->DatabaseState = DatabaseState;
        disconnect(DatabaseState,SIGNAL(GroupsChanged()),this,SLOT(NotifyAboutNewDatabaseStructure()));
        connect(DatabaseState,SIGNAL(GroupsChanged()),this,SLOT(NotifyAboutNewDatabaseStructure()));
    }

    void WebInterfaceApi::SetDatabaseConnector(IDatabaseConnector *DatabaseConnector)
    {
        this->DatabaseConnector = DatabaseConnector;
    }


    void WebInterfaceApi::NotifyAboutNewDatabaseStructure()
    {
        if(!WebInterface)
            return;
        if(!DatabaseState)
            return;
        QString Json = this->DatabaseState->ToJson();
        WebInterface->Send("database_structure_changed",false,0,"structure",Json);
    }

    QString WebInterfaceApi::GetDatabaseStructure()
    {
        if(!WebInterface)
            return QString();
        if(!DatabaseState)
            return QString();
        return this->DatabaseState->ToJson();
    }

    void WebInterfaceApi::BrowserStart(IProcessComunicator* Comunicator, int Index)
    {
        if(!WebInterface)
            return;
        int BrowserId = qrand() % 1000000 + 10;
        Comunicator->setProperty("BrowserId", BrowserId);
        int TaskId = 0;
        if(Comunicator->property("TaskId").isValid())
            TaskId = Comunicator->property("TaskId").toInt();
        WebInterface->Send("browser_add",false,0,"browser_id",BrowserId,"thread_number",Index,"task_id",TaskId);
        BrowserViewData Data;
        Data.Communicator = Comunicator;
        Data.BrowserId = BrowserId;
        BrowserViewMap[Comunicator] = Data;
        CommunicatorMap[BrowserId] = Comunicator;
        disconnect(Comunicator,SIGNAL(Received(QString)),this,SLOT(Received(QString)));
        disconnect(Comunicator,SIGNAL(ShowBrowser()),this,SLOT(ShowBrowser()));
        disconnect(Comunicator,SIGNAL(HideBrowser()),this,SLOT(HideBrowser()));

        connect(Comunicator,SIGNAL(Received(QString)),this,SLOT(Received(QString)));
        connect(Comunicator,SIGNAL(ShowBrowser()),this,SLOT(ShowBrowser()));
        connect(Comunicator,SIGNAL(HideBrowser()),this,SLOT(HideBrowser()));
    }
    void WebInterfaceApi::BrowserStop(IProcessComunicator* Comunicator, int Index)
    {
        if(!WebInterface)
            return;
        int BrowserId = Comunicator->property("BrowserId").toInt();
        WebInterface->Send("browser_remove",false,0,"browser_id",BrowserId,"thread_number",Index);
        BrowserViewMap.remove(Comunicator);
        CommunicatorMap.remove(BrowserId);
    }

    void WebInterfaceApi::Reset()
    {
        this->ReportDataVisualizer = 0;
        this->Worker = 0;
        this->ResourcesVisualizer = 0;
        this->DatabaseState = 0;
    }

    void WebInterfaceApi::WebInterfaceEvalSignal(QString Script)
    {
        if(WebInterface && WebInterface->HasWebInterface())
        {
            WebInterface->Send("eval",false,0,"script",Script);
        }
    }

    void WebInterfaceApi::ReceivedMessage(QString MessageType, QVariantMap Data,bool IsAsync,int Id)
    {
        if(!WebInterface)
            return;


        if(MessageType == "show_scheduler")
        {
            emit ShowScheduler();
        }

        if(MessageType == "install_scheduler")
        {
            emit InstallScheduler();
        }

        if(MessageType == "database_select")
        {
            QVariantList res;
            if(DatabaseConnector)
            {
                DatabaseSelector Selector = DatabaseConnector->ConvertToDatabaseSelector(Data["Selector"]);
                Selector.TableId = Data["TableId"].toInt();
                QList<DatabaseItem> Items = DatabaseConnector->Select(Selector);

                for(DatabaseItem& Item: Items)
                {
                    QVariantMap I;
                    I["id"] = Item.Id;
                    QHash<int,QVariant>::iterator i;
                    QVariantMap dat;
                    for (i = Item.Data.begin(); i != Item.Data.end(); ++i)
                    {
                        dat[QString::number(i.key())] = i.value();
                    }
                    I["data"] = dat;

                    res.append(I);
                }
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,res);
        }

        if(MessageType == "database_count")
        {
            int res = 0;
            if(DatabaseConnector)
            {
                DatabaseSelector Selector = DatabaseConnector->ConvertToDatabaseSelector(Data["Selector"]);
                Selector.TableId = Data["TableId"].toInt();
                res = DatabaseConnector->Count(Selector);
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,res);
        }

        if(MessageType == "database_insert")
        {
            QString res;
            if(DatabaseConnector)
            {
                QList<DatabaseColumn> Columns = DatabaseConnector->GetColumns(Data["TableId"].toInt());


                QVariantMap ItemOriginal = Data["Item"].toMap();
                QHash<int,QVariant> Item;
                QVariantMap::iterator i;
                for (i = ItemOriginal.begin(); i != ItemOriginal.end(); ++i)
                {
                    bool IsDate = false;
                    for(DatabaseColumn & Column: Columns)
                    {
                        if(Column.Id == i.key().toInt())
                        {
                            IsDate = Column.Type == DatabaseColumn::Date;
                            break;
                        }
                    }
                    if(IsDate)
                    {
                        QDateTime timestamp;

                        if(i.value().toLongLong() > 0)
                        {
                            timestamp.setTime_t(i.value().toString().toLongLong() / 1000);
                        }
                        else
                            timestamp = QDateTime::currentDateTime();
                        Item[i.key().toInt()] = QVariant(timestamp);
                    }
                    else
                        Item[i.key().toInt()] = i.value();
                }
                DatabaseItem DItem;
                DItem.IsNull = false;
                DItem.Data = Item;

                DatabaseGroups Groups;
                Groups.IsNull = false;
                Groups.GroupIdList = Data["Groups"].toStringList();

                res = DatabaseConnector->Insert(Groups,DItem,Data["TableId"].toInt());
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,res);
        }

        if(MessageType == "database_update")
        {
            if(DatabaseConnector)
            {
                QList<DatabaseColumn> Columns = DatabaseConnector->GetColumns(Data["TableId"].toInt());

                QVariantMap ItemOriginal = Data["Item"].toMap();
                QHash<int,QVariant> Item;
                QVariantMap::iterator i;
                for (i = ItemOriginal.begin(); i != ItemOriginal.end(); ++i)
                {
                    bool IsDate = false;
                    for(DatabaseColumn & Column: Columns)
                    {
                        if(Column.Id == i.key().toInt())
                        {
                            IsDate = Column.Type == DatabaseColumn::Date;
                            break;
                        }
                    }
                    if(IsDate)
                    {
                        QDateTime timestamp;

                        if(i.value().toLongLong() > 0)
                        {
                            timestamp.setTime_t(i.value().toString().toLongLong() / 1000);
                        }
                        else
                            timestamp = QDateTime::currentDateTime();
                        Item[i.key().toInt()] = QVariant(timestamp);
                    }else
                    {
                        if(i.value().type() == QVariant::Double)
                        {
                            Item[i.key().toInt()] = i.value().toInt();
                        }else
                        {
                            Item[i.key().toInt()] = i.value();
                        }
                    }
                }
                DatabaseItem DItem;
                DItem.IsNull = false;
                DItem.Data = Item;
                DItem.Id = Data["ItemId"].toString();

                DatabaseConnector->Update(DItem,Data["TableId"].toInt());
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,QVariant());
        }

        if(MessageType == "database_delete")
        {
            if(DatabaseConnector)
            {
                DatabaseMassSelector Selector;
                Selector.Filters.IsNull = true;
                Selector.Groups.IsNull = true;
                Selector.Sort.IsNull = true;
                Selector.TableId = Data["TableId"].toInt();
                DatabaseItems Items;
                Items.IsNull = false;
                Items.IsInverseSelection = false;
                Items.SelectedRows = QSet<QString>::fromList(Data["Items"].toStringList());
                Selector.Items = Items;

                DatabaseConnector->Delete(Selector);
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,QVariant());
        }

        if(MessageType == "database_insert_group")
        {
            QString Res;
            if(DatabaseConnector)
            {
                DatabaseGroup Group;
                Group.CreatedAt = QDateTime::currentDateTime();
                Group.IsNull = false;
                Group.Name = Data["Group"].toMap()["Name"].toString();
                Group.Description = Data["Group"].toMap()["Description"].toString();

                Res = DatabaseConnector->InsertGroup(Group,Data["TableId"].toInt());
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,Res);
        }

        if(MessageType == "database_delete_group_no_data")
        {
            if(DatabaseConnector)
            {
                DatabaseGroups Groups;
                Groups.GroupIdList = QStringList()<<Data["GroupId"].toString();
                Groups.IsNull = false;

                DatabaseConnector->DeleteGroupNoData(Groups,Data["TableId"].toInt());
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,QVariant());
        }

        if(MessageType == "database_delete_group_with_data")
        {
            if(DatabaseConnector)
            {
                DatabaseGroups Groups;
                Groups.GroupIdList = QStringList()<<Data["GroupId"].toString();
                Groups.IsNull = false;

                DatabaseConnector->DeleteGroupWithData(Groups,Data["TableId"].toInt());
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,QVariant());
        }

        if(MessageType == "database_update_group")
        {
            if(DatabaseConnector)
            {
                DatabaseGroup Group;
                Group.CreatedAt = QDateTime::currentDateTime();
                Group.IsNull = false;
                Group.Id = Data["GroupId"].toString();
                Group.Name = Data["Group"].toMap()["Name"].toString();
                Group.Description = Data["Group"].toMap()["Description"].toString();


                DatabaseConnector->UpdateGroup(Group,Data["TableId"].toInt());
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,QVariant());
        }

        if(MessageType == "database_move_to_group")
        {
            if(DatabaseConnector)
            {
                DatabaseMassSelector Selector;
                Selector.Filters.IsNull = true;
                Selector.Groups.IsNull = true;
                Selector.Sort.IsNull = true;
                DatabaseItems Items;
                Items.IsNull = false;
                Items.IsInverseSelection = false;
                Items.SelectedRows = QSet<QString>::fromList(Data["Items"].toStringList());
                Selector.TableId = Data["TableId"].toInt();
                Selector.Items = Items;

                DatabaseConnector->MoveToGroup(Selector, Data["GroupId"].toString());
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,QVariant());
        }
        if(MessageType == "database_copy_to_group")
        {
            if(DatabaseConnector)
            {
                DatabaseMassSelector Selector;
                Selector.Filters.IsNull = true;
                Selector.Groups.IsNull = true;
                Selector.Sort.IsNull = true;
                DatabaseItems Items;
                Items.IsNull = false;
                Items.IsInverseSelection = false;
                Items.SelectedRows = QSet<QString>::fromList(Data["Items"].toStringList());
                Selector.TableId = Data["TableId"].toInt();
                Selector.Items = Items;

                DatabaseConnector->CopyToGroup(Selector, Data["GroupId"].toString());
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,QVariant());
        }

        if(MessageType == "set_global_variable")
        {
            if(Worker && Worker->GetProperties())
            {
                Worker->GetProperties()->Set("basglobal", Data["name"].toString(), Data["value"].toString());
            }
            WebInterface->Send(MessageType,IsAsync,Id);
        }

        if(MessageType == "set_interface_language")
        {
            QSettings Settings("settings.ini",QSettings::IniFormat);
            Settings.setValue("DefaultLanguageScript",Data["lang"].toString());
            Settings.sync();
        }

        if(MessageType == "get_global_variable")
        {
            QString Res = "\"\"";
            if(Worker && Worker->GetProperties())
            {
                QString Raw = Worker->GetProperties()->Get("basglobal", Data["name"].toString());
                if(!Raw.isEmpty())
                    Res = Raw;
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,Res);
        }

        if(MessageType == "show_database_manager")
        {
            emit ShowDatabase();
        }

        if(MessageType == "show_browser")
        {
            int BrowserId = Data["browser_id"].toInt();

            if(CommunicatorMap.contains(BrowserId))
            {
                IProcessComunicator* ProcessComunicator = CommunicatorMap[BrowserId];
                if(ProcessComunicator)
                {
                    ProcessComunicator->OnShowBrowser();
                    ProcessComunicator->Send(QString("<Visible>1</Visible>"));
                }
            }
        }

        if(MessageType == "hide_browser")
        {
            int BrowserId = Data["browser_id"].toInt();

            if(CommunicatorMap.contains(BrowserId))
            {
                IProcessComunicator* ProcessComunicator = CommunicatorMap[BrowserId];
                if(ProcessComunicator)
                {
                    ProcessComunicator->OnHideBrowser();
                    ProcessComunicator->Send(QString("<Visible>0</Visible>"));
                }
            }
        }

        if(MessageType == "view_browser")
        {
            int BrowserId = Data["browser_id"].toInt();

            IProcessComunicator* ProcessComunicator = 0;
            QMutableMapIterator<IProcessComunicator*, BrowserViewData> i(BrowserViewMap);
            while (i.hasNext())
            {
                i.next();
                if(i.value().BrowserId == BrowserId && !i.value().Active)
                {
                    ProcessComunicator = i.key();
                    i.value().RequestId = Id;
                    i.value().Active = true;
                    break;
                }
            }
            if(ProcessComunicator)
            {
                ProcessComunicator->Send(QString("<View>") + QString::number(Id) + QString("</View>"));
            }else
            {
                QVariantMap Res;
                Res["image"] = QString();
                Res["width"] = 0;
                Res["height"] = 0;
                Res["x"] = 0;
                Res["y"] = 0;
                WebInterface->SendRaw(MessageType,true,Id,Res);
            }
        }


        if(MessageType == "get_tabs")
        {
            int BrowserId = Data["browser_id"].toInt();

            IProcessComunicator* ProcessComunicator = 0;
            QMutableMapIterator<IProcessComunicator*, BrowserViewData> i(BrowserViewMap);
            while (i.hasNext())
            {
                i.next();
                if(i.value().BrowserId == BrowserId && !i.value().GetTabsActive)
                {
                    ProcessComunicator = i.key();
                    i.value().GetTabsRequestId = Id;
                    i.value().GetTabsActive = true;
                    break;
                }
            }
            if(ProcessComunicator)
            {
                ProcessComunicator->Send(QString("<GetTabs>") + QString::number(Id) + QString("</GetTabs>"));
            }else
            {
                QVariantMap Res;
                Res["json"] = QString("{\"index\":-1,\"urls\":[]}");
                WebInterface->SendRaw(MessageType,true,Id,Res);
            }
        }

        if(MessageType == "download_log")
        {
            QString res;
            if(Worker && Worker->GetLogger())
            {
                QFile f(Worker->GetLogger()->GetFileName());
                if (f.open(QFile::ReadOnly))
                {
                    res = QString::fromUtf8(f.readAll());
                }
                f.close();
            }else if(!LastLogPath.isEmpty())
            {
                QFile f(LastLogPath);
                if (f.open(QFile::ReadOnly))
                {
                    res = QString::fromUtf8(f.readAll());
                }
                f.close();
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,res);
        }

        if(MessageType == "script_report")
        {
            QString res;
            if(ReportDataVisualizer)
            {
                res = ReportDataVisualizer->PrepareText();
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,res);
        }

        if(MessageType == "start_thread")
        {
            int ThreadId = Data["thread_id"].toInt();
            emit StartThread(ThreadId);
        }

        if(MessageType == "stop_thread")
        {
            int ThreadId = Data["thread_id"].toInt();
            emit StopThread(ThreadId);
        }

        if(MessageType == "run_task")
        {
            int ThreadId = Data["thread_id"].toInt();
            QString FunctionName = Data["function_name"].toString();
            QString Parameters = Data["params"].toString();
            emit RunTaskInThread(ThreadId, Id, FunctionName, Parameters);
        }


        if(MessageType == "open_file_dialog")
        {
            bool IsDir = false;
            if(Data.contains("is_dir"))
            {
                IsDir = Data["is_dir"].toBool();
            }

            QString Caption;
            if(IsDir)
            {
                Caption = tr("Open Directory");
            }else
            {
                Caption = tr("Open File");
            }

            if(Data.contains("caption"))
            {
                Caption = Data["caption"].toString();
            }

            QString Dir = LastFolder;
            if(Data.contains("dir"))
            {
                Dir = Data["dir"].toString();
            }

            QString Filter;
            if(Data.contains("filter"))
            {
                Filter = Data["filter"].toString();
            }

            if(IsDir)
            {
                ShowMainWindow();
                QFileDialog *Dialog = new QFileDialog();
                Dialog->setDirectory(Dir);
                Dialog->setWindowTitle(Caption);
                Dialog->setFileMode(QFileDialog::Directory);
                Dialog->setParent(MainWindow);
                Dialog->open();

                Dialog->setProperty("MessageType",MessageType);
                Dialog->setProperty("IsAsync",IsAsync);
                Dialog->setProperty("Id",Id);

                connect(Dialog,SIGNAL(finished(int)),this,SLOT(FileDialogFinished(int)));

                Dialog->open();

                //Result = QFileDialog::getExistingDirectory(MainWindow, Caption, Dir);
            }else
            {
                ShowMainWindow();
                QFileDialog *Dialog = new QFileDialog();
                Dialog->setDirectory(Dir);
                Dialog->setWindowTitle(Caption);
                Dialog->setFileMode(QFileDialog::ExistingFiles);
                Dialog->setParent(MainWindow);
                Dialog->setNameFilter(Filter);
                Dialog->open();

                Dialog->setProperty("MessageType",MessageType);
                Dialog->setProperty("IsAsync",IsAsync);
                Dialog->setProperty("Id",Id);

                connect(Dialog,SIGNAL(finished(int)),this,SLOT(FileDialogFinished(int)));

                Dialog->open();

                //Result = QFileDialog::getOpenFileName(MainWindow, Caption, Dir, Filter);
            }


        }

        if(MessageType == "save_file_dialog")
        {
            QString Caption = tr("Save File");

            if(Data.contains("caption"))
            {
                Caption = Data["caption"].toString();
            }

            QString Dir = LastFolder;
            if(Data.contains("dir"))
            {
                Dir = Data["dir"].toString();
            }

            QString Filter;
            if(Data.contains("filter"))
            {
                Filter = Data["filter"].toString();
            }


            ShowMainWindow();
            //Result = QFileDialog::getSaveFileName(MainWindow, Caption, Dir, Filter);
            QFileDialog *Dialog = new QFileDialog();
            Dialog->setDirectory(Dir);
            Dialog->setWindowTitle(Caption);
            Dialog->setFileMode(QFileDialog::AnyFile);
            Dialog->setParent(MainWindow);
            Dialog->setNameFilter(Filter);
            Dialog->open();

            Dialog->setProperty("MessageType",MessageType);
            Dialog->setProperty("IsAsync",IsAsync);
            Dialog->setProperty("Id",Id);

            connect(Dialog,SIGNAL(finished(int)),this,SLOT(FileDialogFinished(int)));

            Dialog->open();

        }

        if(MessageType == "resources_report")
        {
            QString res;
            if(ResourcesVisualizer)
            {
                res = ResourcesVisualizer->PrepareText();
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,res);
        }


        if(MessageType == "download_result")
        {
            QString res;
            QString FileName;
            int Index = Data["number"].toInt();

            if(Worker)
            {
                if(Index == 1)
                    FileName = Worker->GetResults1()->GetFileName();
                if(Index == 2)
                    FileName = Worker->GetResults2()->GetFileName();
                if(Index == 3)
                    FileName = Worker->GetResults3()->GetFileName();
                if(Index == 4)
                    FileName = Worker->GetResults4()->GetFileName();
                if(Index == 5)
                    FileName = Worker->GetResults5()->GetFileName();
                if(Index == 6)
                    FileName = Worker->GetResults6()->GetFileName();
                if(Index == 7)
                    FileName = Worker->GetResults7()->GetFileName();
                if(Index == 8)
                    FileName = Worker->GetResults8()->GetFileName();
                if(Index == 9)
                    FileName = Worker->GetResults9()->GetFileName();
            }

            if(FileName.isEmpty() && Index - 1 < LastResultPathes.length())
            {
                FileName = LastResultPathes.at(Index - 1);
            }

            if(!FileName.isEmpty())
            {
                QFile f(FileName);
                if (f.open(QFile::ReadOnly))
                {
                    res = QString::fromUtf8(f.readAll());
                }
                f.close();
            }
            WebInterface->SendRaw(MessageType,IsAsync,Id,res);
        }

        if(MessageType == "stop")
        {
            bool IsInstant = Data["is_instant"].toBool();
            if(IsInstant)
                emit AbortInstant();
            else
                emit AbortNotInstant();
        }

        if(MessageType == "restart")
        {
            emit Restart();
        }

        if(IsAsync)
        {
            if(MessageType == "folder_content")
            {
                QStringList PathList = Data["folder"].toStringList();
                QVariantList Res;

                if(PathList.isEmpty())
                {
                    for(const QStorageInfo &storage: QStorageInfo::mountedVolumes())
                    {
                        if (storage.isValid() && storage.isReady() && !storage.isReadOnly())
                        {
                            QVariantMap ToAdd;
                            ToAdd["is_folder"] = true;
                            ToAdd["name"] = storage.rootPath().split("/").first();
                            Res.append(ToAdd);
                        }
                    }
                }else
                {
                    QString Folder = PathList.join("/") + QString("/");
                    //qDebug()<<Folder;
                    QDir directory(Folder);

                    QStringList items = directory.entryList(QDir::AllDirs | QDir::NoDot | QDir::NoDotDot);
                    for(QString& item: items)
                    {
                        QVariantMap ToAdd;
                        ToAdd["is_folder"] = true;
                        ToAdd["name"] = item;
                        Res.append(ToAdd);
                    }
                    items = directory.entryList(QDir::Files | QDir::NoDot | QDir::NoDotDot);
                    for(QString& item: items)
                    {
                        QVariantMap ToAdd;
                        ToAdd["is_folder"] = false;
                        ToAdd["name"] = item;
                        Res.append(ToAdd);
                    }
                    //        qDebug()<<Res;
                }

                WebInterface->SendRaw(MessageType,IsAsync,Id,Res);
            }
        }
    }

}
