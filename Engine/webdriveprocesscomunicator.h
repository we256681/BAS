#ifndef WEBDRIVEPROCESSCOMUNICATOR_H
#define WEBDRIVEPROCESSCOMUNICATOR_H

#include "engine_global.h"
#include "iprocesscomunicator.h"
#include "ihttpclientfactory.h"
#include "webdriverbrowserviewer.h"


#include <QObject>
#include <QProcess>
#include <QFile>
#include <QSharedPointer>
#include "ilogger.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT WebDriveProcessComunicator : public IProcessComunicator
    {
        Q_OBJECT
        bool IsClosed;
        bool IsRecord;
        QString RecordId;
        QString BrowserEngine;
        bool _IsFail;
        QString _FailString;
        bool _IsDie;
        QString _DieString;
        int Id;
        QString SessionId;
        QString ProfileId;
        QSharedPointer<QFile> ProfileLock;
        QString MLAKey;
        bool JustStarted;
        bool _HasProcess;
        int Port;
        IHttpClientFactory *HttpClientFactory;
        QString NextResponce;
        QString NextUrl;
        QString ApiLocation;
        QString NextProfileIdDelete;
        QString OnlineProfileSearchPattern;
        int WidthDelta;
        int HeightDelta;
        int GeneralTimeout = 60000;
        int BrowserTimeout = 60000;
        int ScrollX = 0;
        int ScrollY = 0;
        int MouseX = 0;
        int MouseY = 0;
        int PositionTopLeftX = 0;
        int PositionTopLeftY = 0;
        int PositionRightBottomX = 0;
        int PositionRightBottomY = 0;
        QString MouseUrl;
        int LastMouseMoveX;
        int LastMouseMoveY;
        QString ElementId;
        QString ElementCommand;
        QString ElementArgumentA;
        QString ElementArgumentB;
        QString ElementSelector;

        qint64 Timestamp;
        ILogger *Logger;

        //Frame search
        bool IsInsideFrame = false;
        QString ExpandFramesSelector;
        QString ExpandFramesScript;
        QByteArray ExpandFramesSlot;
        QObject* ExpandFramesObject = 0;
        int ExpandFramesX = 0;
        int ExpandFramesY = 0;
        bool IsScrollFrame;

        //Adjust coordinates
        QString ScriptToExecuteAfterAdjust;
        QByteArray SlotToExecuteAfterAjust;
        QObject* ObjectToExecuteAfterAjust = 0;
        QString AdjustSelector;


        //Helper code
        QString SearchFunctions;

        QString SendReceivedResponse;


        WebDriverBrowserViewer* Viewer;
        QString ChromePID;

    public:
        explicit WebDriveProcessComunicator(QObject *parent = 0);
        ~WebDriveProcessComunicator();

        virtual void SetLocation(const QString& Location);
        virtual void SetRecord(bool IsRecord);
        virtual void CreateProcess(const QStringList& arguments, bool IsTemporaryProfile);
        virtual void ConnectToProcess(const QString& key_in, const QString& key_out);
        virtual void Send(const QString& value);
        virtual void Abort();
        virtual void AbortByUser();
        virtual bool IsError();
        virtual QString ErrorString();

        //Should thread fail
        virtual bool IsFail();
        virtual QString FailString();

        //Should script die
        virtual bool IsDie();
        virtual QString DieString();

        virtual bool HasProcess();
        virtual qint64 GetPID();
        virtual void SetGeneralTimeout(int Timeout);

        int GetId();
        void SetBrowserEngine(const QString& BrowserEngine);
        void SetPort(int Port);
        void SetHttpClientFactory(IHttpClientFactory *HttpClientFactory);

        //ProfileInfo
        virtual bool GetIsOnlineProfile();
        virtual QString GetProfileId();

        void SetLogger(ILogger * Logger);
        void SetIsRecord();
        void SetRecordId(const QString& RecordId);

        void OnShowBrowser();
        void OnHideBrowser();

    private slots:
        /* Custom Logic */
        void ResponceScreenshot();
        void ResponceScreenshotScroll();
        void ResponceScreenshotForWidget();
        void ResponceResizeGuess();
        void ResponceResizeExact();
        void ProcessCreatedFinal();
        void SetInitialTimeoutPageLoad();
        void SetInitialTimeoutScript();
        void AfterTimeoutChanged();
        void LoadFinal();
        void MouseMoveAfterScroll();
        void MouseMoveAfterScrollBeforeClick();
        void MouseClickAfterScroll();
        void NeedScreenshot();
        void RenderBase64AfterScroll();
        void RenderBase64Final();
        void RemoveOnlineProfileId();
        void ResponceStartUsingExtensions();
        void ResizeGuess();

        /* Helpers */
        void SendNextResponce();
        void SearchProfilesResult();
        void SendNextResponceAndDeleteSender();
        void Api(const QString& Method, const QString& Data, const QString& Url, const QString& Responce, QObject *object, const char * slot);
        void Api(const QString& Method, const QString& Data, const QString& Url, const QString& Responce);
        void Type(const QString& Text,int Interval, QObject *object, const char * slot);
        void ScrollToCoordinates(int X, int Y, QObject *object, const char * slot);
        void FindElementAndExecuteScript(const QString& Selector, const QString& Script, QObject *object, const char * slot, bool DoScroll);
        void FindElementAndExecuteScriptInternal(const QString& Selector, const QString& Script, QObject *object, const char * slot, bool IsFirst, bool DoScroll);
        void SendReceived(const QString& Text);
        void AfterRestoreFrame();
        void ExecuteElementAction();
        void Log(const QString& Text,const QString& Arg1 = QString(),const QString& Arg2 = QString(),const QString& Arg3 = QString());

        IHttpClient * LogHttpClientAndDelete(QObject *Client);
        QString LogHttpClientDeleteAndGetValue(QObject *Client);
        QString GenerateJson(const QString& Name1, const QVariant& Value1,const QString& Name2 = QString(), const QVariant& Value2 = QVariant(),const QString& Name3 = QString(), const QVariant& Value3 = QVariant(),const QString& Name4 = QString(), const QVariant& Value4 = QVariant());
        QString EscapeStringJavascript(const QString &Text);
        QStringList ParseJsonArray(const QString &JsonArrayAsString);
        QString GenerateJsonArray(const QStringList &JsonArray);


        /* Frames */
        void ExpandFrames(const QString& Selector, const QString& Script, QObject *object, const char * slot);
        void MoveToTopFrame(const QString& Selector, const QString& Script, QObject *object, const char * slot);
        void SetFrameChangeArguments(const QString& Selector, const QString& Script, QObject *object, const char * slot);
        void ChangeFrameResult();
        void FailedToSearchForFrame();

        /* Adjust */
        void FindElementScrollAndExecuteScriptAdjustCoordinates(const QString& Selector, const QString& Script, QObject *object, const char * slot);
        void AfterAdjust();
        void AfterGetMainWindowScroll();
        void AfterAdjustAndResetFrame();


        /* Tools */
        void SetIsClosed();
    signals:
        void NewSession(QStringList arguments, QString BrowserEngine, int CommunicatorId);
        void ViewerData(QImage,int,int);
    public slots:
        virtual void Suspend();
        void SessionCreated(int, const QString&,int,bool,const QString&,bool,const QString&);
        void LockProfile(int, const QString&);
        void ResetViewer();
    };
}

#endif // WEBDRIVEPROCESSCOMUNICATOR_H
