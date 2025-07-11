#include "recordprocesscommunication.h"
#include <QXmlStreamWriter>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QApplication>
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    RecordProcessCommunication::RecordProcessCommunication(QObject *parent) :
        IRecordProcessCommunication(parent)
    {
        Comunicator = 0;
        CanSend = false;
        IsTesting = false;
        CanSendIsChanged = false;
        IsPlayingScript = false;
    }

    void RecordProcessCommunication::SendIsChanged()
    {
        if(Comunicator && CanSendIsChanged && CanSend)
        {
            CanSendIsChanged = false;
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeTextElement("IsChanged","");
            Comunicator->Send(WriteString);
        }
    }


    QString RecordProcessCommunication::ConvertResources(const QList<IRecordProcessCommunication::ResourceDescription>& Resources)
    {
        QJsonArray array;
        for(ResourceDescription res:Resources)
        {
            QJsonObject obj;
            obj["name"] = res.Name;
            obj["description"] = res.Description;
            obj["type"] = res.Type;
            array.append(obj);
        }

        QJsonDocument doc;
        doc.setArray(array);
        QString res = doc.toJson();
        return res;
    }

    void RecordProcessCommunication::SendCode(const QString& Code,const QString& Schema, const QString& EmbeddedLanguageData, bool IsTesting, const QString& InterfaceState, const QString& ScriptEngineVersion, const QString& ApplicationEngineVersion)
    {
        this->IsTesting = IsTesting;

        if(Comunicator && CanSend)
        {

            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeStartElement("SetCode");
            xmlWriter.writeAttribute("Schema", Schema);
            xmlWriter.writeAttribute("IsTesting", QString::number(IsTesting));

                xmlWriter.writeStartElement("Code");
                xmlWriter.writeCharacters(Code);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("EmbeddedLanguageData");
                xmlWriter.writeCharacters(EmbeddedLanguageData);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("ApplicationEngineVersion");
                xmlWriter.writeCharacters(ApplicationEngineVersion);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("ScriptEngineVersion");
                xmlWriter.writeCharacters(ScriptEngineVersion);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("InterfaceState");
                xmlWriter.writeCharacters(InterfaceState);
                xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
            Comunicator->Send(WriteString);
            SendData.clear();
            SendDataSchema.clear();
            SendEmbeddedData.clear();
            SendInterfaceState.clear();
            SendScriptEngineVersion.clear();
            SendApplicationEngineVersion.clear();

        }else
        {
            SendData = Code;
            SendDataSchema = Schema;
            SendEmbeddedData = EmbeddedLanguageData;
            SendInterfaceState = InterfaceState;
            SendScriptEngineVersion = ScriptEngineVersion;
            SendApplicationEngineVersion = ApplicationEngineVersion;
        }
    }

    void RecordProcessCommunication::CheckResult(QString CheckId,bool IsSuccess,QString ErrorString)
    {

        if(Comunicator && CanSend)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeStartElement("CheckResult");
            xmlWriter.writeAttribute("CheckId", CheckId);
            xmlWriter.writeAttribute("IsSuccess", (IsSuccess ? "true" : "false"));
            xmlWriter.writeAttribute("ErrorString", ErrorString);
            xmlWriter.writeEndElement();
            Comunicator->Send(WriteString);
        }
    }

    void RecordProcessCommunication::RunTaskInRecordMode(int ResultId, QString Function, QString Parameters)
    {
        if(Comunicator && CanSend)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeStartElement("RunTask");
            xmlWriter.writeAttribute("FunctionName", Function);
            xmlWriter.writeAttribute("Parameters", Parameters);
            xmlWriter.writeAttribute("ResultId", QString::number(ResultId));
            xmlWriter.writeEndElement();
            Comunicator->Send(WriteString);
        }else
        {
            RunTaskResultId = ResultId;
            RunTaskFunction = Function;
            RunTaskParameters = Parameters;
        }
    }


    void RecordProcessCommunication::StopRecorder()
    {
        emit Stop();
    }

    void RecordProcessCommunication::SetWindow(const QString& Window)
    {
        if(Comunicator && CanSend)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeStartElement("SetWindow");
            xmlWriter.writeAttribute("is_play", QString::number(IsPlayingScript));
            xmlWriter.writeCharacters(Window);
            xmlWriter.writeEndElement();
            Comunicator->Send(WriteString);
            this->Window.clear();
        }else
        {
            this->Window = Window;
        }
    }

    void RecordProcessCommunication::HighlightAction(const QString& ActionId)
    {
        if(Comunicator && CanSend)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeTextElement("HighlightAction",ActionId);
            Comunicator->Send(WriteString);
        }
    }



    void RecordProcessCommunication::RestoreOriginalStage()
    {
        if(Comunicator && CanSend)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeTextElement("RestoreOriginalStage","");
            Comunicator->Send(WriteString);
        }
    }

    void RecordProcessCommunication::SendResources(const QList<IRecordProcessCommunication::ResourceDescription>& Resources)
    {
        if(Comunicator && CanSend)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeTextElement("SetResources",ConvertResources(Resources));
            Comunicator->Send(WriteString);
            SendResourcesString.clear();
        }else
        {
            SendResourcesString = ConvertResources(Resources);
        }

    }

    void RecordProcessCommunication::ReceivedData(const QString& text)
    {
        if(text.contains("END_BAS_TEST") && IsTesting)
        {
            qApp->closeAllWindows();
            qApp->quit();
            return;
        }
        QXmlStreamReader xmlReader(text);
        while(!xmlReader.atEnd() && !xmlReader.hasError())
        {
            QXmlStreamReader::TokenType token = xmlReader.readNext();

            if(xmlReader.name() == "WaitCode" && token == QXmlStreamReader::StartElement)
            {
                foreach(QXmlStreamAttribute attr, xmlReader.attributes())
                {
                    if(attr.name() == "is_play")
                    {
                        IsPlayingScript = (bool)attr.value().toString().toInt();
                    }
                }
                xmlReader.readNext();
            }
            if(xmlReader.name() == "ReceivedCode" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                bool CanSendIsChangedSaved = CanSendIsChanged;
                CanSendIsChanged = false;
                emit ReceiveCode(xmlReader.text().toString());
                CanSendIsChanged = CanSendIsChangedSaved;
            }
            if(xmlReader.name() == "RunTaskResult" && token == QXmlStreamReader::StartElement)
            {

                int ResultId = -1;

                foreach(QXmlStreamAttribute attr, xmlReader.attributes())
                {
                    if(attr.name() == "ResultId")
                    {
                        ResultId = attr.value().toString().toInt();
                    }
                }

                xmlReader.readNext();
                QString Result = xmlReader.text().toString();

                emit RunTaskResult(ResultId, Result);
            }
            if(xmlReader.name() == "EmbeddedData" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Data = xmlReader.text().toString();
                emit EmbeddedData(Data);
            }
            if(xmlReader.name() == "LoadScript" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                emit LoadScript(xmlReader.text().toString());
            }
            if(xmlReader.name() == "MaximizeWindow" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                emit MaximizeWindow();
            }
            if(xmlReader.name() == "WindowAttached" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                emit WindowAttached();
            }
            if(xmlReader.name() == "StartBackup" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                emit StartBackup();
            }
            if(xmlReader.name() == "Interrupt" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                emit Interrupt();
            }
            if(xmlReader.name() == "PrepareFunction" && token == QXmlStreamReader::StartElement)
            {
                QString FunctionName;
                foreach(QXmlStreamAttribute attr, xmlReader.attributes())
                {
                    if(attr.name() == "FunctionName")
                    {
                        FunctionName = attr.value().toString();
                    }
                }
                xmlReader.readNext();
                QString Data = xmlReader.text().toString();

                emit PrepareFunctionResult(FunctionName, Data);
            }
            if(xmlReader.name() == "SaveInterface" && token == QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                QString Data = xmlReader.text().toString();
                emit SaveInterface(Data);
            }
            if(xmlReader.name() == "Terminate" && token == QXmlStreamReader::StartElement)
            {
                emit Stop();
                /*if(Comunicator)
                    Comunicator->AbortByUser();*/
            }
            if(xmlReader.name() == "Restart" && token == QXmlStreamReader::StartElement)
            {
                emit NeedRestart();
                emit Stop();
                /*if(Comunicator)
                    Comunicator->AbortByUser();*/
            }
        }
        if (xmlReader.hasError())
        {
            return;
        }
    }

    void RecordProcessCommunication::SetCanSendDataTrue()
    {
        CanSend = true;
        if(!SendData.isEmpty() && Comunicator)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeStartElement("SetCode");
            xmlWriter.writeAttribute("Schema", SendDataSchema);
            xmlWriter.writeAttribute("IsTesting", QString::number(IsTesting));

                xmlWriter.writeStartElement("Code");
                xmlWriter.writeCharacters(SendData);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("EmbeddedLanguageData");
                xmlWriter.writeCharacters(SendEmbeddedData);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("ApplicationEngineVersion");
                xmlWriter.writeCharacters(SendApplicationEngineVersion);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("ScriptEngineVersion");
                xmlWriter.writeCharacters(SendScriptEngineVersion);
                xmlWriter.writeEndElement();
                xmlWriter.writeStartElement("InterfaceState");
                xmlWriter.writeCharacters(SendInterfaceState);
                xmlWriter.writeEndElement();

            xmlWriter.writeEndElement();
            Comunicator->Send(WriteString);
            SendData.clear();
            SendDataSchema.clear();
            SendEmbeddedData.clear();
            SendInterfaceState.clear();
            SendScriptEngineVersion.clear();
            SendApplicationEngineVersion.clear();
        }

        if(!SendResourcesString.isEmpty() && Comunicator)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeTextElement("SetResources",SendResourcesString);
            Comunicator->Send(WriteString);
            SendResourcesString.clear();
        }

        if(RunTaskResultId >= 0 && Comunicator)
        {
            QString WriteString;

            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeStartElement("RunTask");
            xmlWriter.writeAttribute("FunctionName", RunTaskFunction);
            xmlWriter.writeAttribute("Parameters", RunTaskParameters);
            xmlWriter.writeAttribute("ResultId", QString::number(RunTaskResultId));
            xmlWriter.writeEndElement();

            Comunicator->Send(WriteString);

            RunTaskResultId = -1;
            RunTaskFunction.clear();
            RunTaskParameters.clear();
        }

        if(!Window.isEmpty() && Comunicator)
        {
            SetWindow(Window);
        }
    }

    void RecordProcessCommunication::SetCanSendDataFalse()
    {
        CanSend = false;
        SendData.clear();
        SendResourcesString.clear();
        SendEmbeddedData.clear();
        Comunicator = 0;
    }

    void RecordProcessCommunication::Closed()
    {
        CanSendIsChanged = false;
        SetCanSendDataFalse();
    }

    void RecordProcessCommunication::OnRecord()
    {
        this->IsPlayingScript = false;
    }

    void RecordProcessCommunication::OnRun()
    {
        this->IsPlayingScript = false;
    }

    void RecordProcessCommunication::BackupDone(QString FullPath)
    {
        if(Comunicator && CanSend)
        {
            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeTextElement("BackupDone",FullPath);
            Comunicator->Send(WriteString);
        }
    }

    void RecordProcessCommunication::InstallProcessComunicator(IProcessComunicator *Comunicator)
    {
        CanSend = false;
        CanSendIsChanged = true;
        this->Comunicator = Comunicator;
        connect(Comunicator,SIGNAL(Received(QString)),this,SLOT(ReceivedData(QString)));
        connect(Comunicator,SIGNAL(ProcessStarted()),this,SLOT(SetCanSendDataTrue()));
        connect(Comunicator,SIGNAL(ProcessFinished()),this,SLOT(SetCanSendDataFalse()));
    }

    void RecordProcessCommunication::PrepareFunction(QString FunctionName)
    {

        if(Comunicator && CanSend)
        {

            QString WriteString;
            QXmlStreamWriter xmlWriter(&WriteString);
            xmlWriter.writeTextElement("PrepareFunction",FunctionName);
            Comunicator->Send(WriteString);
        }
    }
}
