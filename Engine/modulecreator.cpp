#include "modulecreator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QRegularExpression>
#include <QMessageBox>
#include "zip/JlCompress.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{

    static bool recurseCopyAddDir(QDir d, QDir t, const QStringList& e)
    {
        QStringList qsl = d.entryList(QStringList()<<"*",QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

        foreach (QString file, qsl)
        {
            QFileInfo finfo(d.absoluteFilePath(file));

            if (finfo.isSymLink())
                return true;

            if (finfo.isDir())
            {
                if(!t.mkdir(file))
                {
                    return false;
                }

                QDir sd(finfo.filePath());
                if(!recurseCopyAddDir(sd, QDir(t.absoluteFilePath(file)),e))
                {
                    return false;
                }
            } else
            {
                bool Exclude = false;
                for(const QString& f:e)
                {
                    if(finfo.absoluteFilePath().contains(f))
                    {
                        Exclude = true;
                        break;
                    }
                }
                if(!Exclude)
                {
                    if(!QFile::copy(finfo.absoluteFilePath(),t.absoluteFilePath(file)))
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    ModuleCreator::ModuleCreator(QObject *parent) :
        IModuleCreator(parent)
    {
    }

    void ModuleCreator::SetScript(const QString& Script)
    {
        this->Script = Script;
    }

    void ModuleCreator::SetModuleInfo(const QString& ModuleInfoText)
    {
        ModuleInfo.clear();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(ModuleInfoText.toUtf8(), &err);
        if (err.error != QJsonParseError::NoError)
            return;

        if(!doc.isObject())
            return;

        ModuleInfo = doc.object().toVariantMap();
    }

    QString ModuleCreator::GetScript()
    {
        return Script;
    }

    QString ModuleCreator::GetModuleInfo()
    {
        QByteArray Data = QJsonDocument::fromVariant(ModuleInfo).toJson();
        return QString::fromUtf8(Data);
    }


    void ModuleCreator::SetProperty(const QString& Key, const QString& Value)
    {
        ModuleInfo.insert(Key,Value);
    }

    QString ModuleCreator::GetProperty(const QString& Key)
    {
        return ModuleInfo.value(Key).toString();
    }

    QString ModuleCreator::GetRandomString()
    {
       const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz");
       const int randomStringLength = 8;

       QString randomString;
       for(int i=0; i<randomStringLength; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }
       return randomString;
    }

    QString ModuleCreator::GetEmbeddedId()
    {
       const QString possibleCharacters("abcdefghijklmnopqrstuvwxyz0123456789");
       const int randomStringLength = 11;

       QString randomString;
       for(int i=0; i<randomStringLength; ++i)
       {
           int index = qrand() % possibleCharacters.length();
           QChar nextChar = possibleCharacters.at(index);
           randomString.append(nextChar);
       }
       return randomString;
    }

    QString ModuleCreator::PrepareCode(const QString& Code, bool UseFunctionMap, QMap<QString,QString>& FunctionMap)
    {
        QString Res = Code;
        {
            QRegularExpression Regexp("section_start\\(\\\"[^\\\"]*\\\"\\s*\\,\\s*\\d+\\)\\!");
            Res = Res.replace(Regexp, QString());
        }
        {
            QRegExp Regexp("\\/\\*Dat\\:.*\\*\\/");
            Regexp.setMinimal(true);

            Res.replace(Regexp,"");
        }
        {
            QRegularExpression Regexp("section_insert\\(\\)");
            Res = Res.replace(Regexp, QString());
        }
        {
            QRegularExpression Regexp("section_end\\(\\)!");
            Res = Res.replace(Regexp, QString());
        }
        int Index = 0;
        while(true)
        {
            Index = Res.indexOf("_call_function(",Index);
            if(Index < 0)
                break;
            Index += 15;
            int IndexLast = 0;
            int Len = 0;
            for( IndexLast = Index;IndexLast<Res.length();IndexLast++)
            {
                QChar CurrentChar = Res.at(IndexLast);
                if(CurrentChar.isSpace() || CurrentChar == ',')
                {
                    IndexLast--;
                    break;
                }
                Len++;
            }

            if(Len == 0)
            {
                continue;
            }

            QString FunctionName = Res.mid(Index,Len);
            if(!FunctionMap.contains(FunctionName))
            {
                continue;
            }

            QString NewFunctionName = FunctionMap.value(FunctionName);
            Res = Res.replace(Index,Len,NewFunctionName);
            Index += NewFunctionName.length();
        }
        return Res;
    }


    QString ModuleCreator::InstallModule(const QString& ModulePath, int OverwriteMode)
    {

        if(!QFileInfo(ModulePath).exists())
        {
            return tr("File %1 doesn't exist").arg(ModulePath);
        }

        if(!QFileInfo("custom").exists())
        {
            QDir dir("custom");
            dir.mkpath(".");
            /*{
                return tr("Failed to create custom directory");
            }*/
        }

        {
            QDir dir("modulecreate/temp");
            if(!dir.removeRecursively())
            {
                return tr("Failed to remove temp directory ") + dir.absolutePath();
            }
            if(!dir.mkpath("."))
            {
                return tr("Failed to create temp directory ") + dir.absolutePath();
            }
        }

        QDir dir(QString("modulecreate/temp/") + GetRandomString() + QString("/"));
        if(!dir.mkpath("."))
        {
            return tr("Failed to create temp directory ") + dir.absolutePath();
        }


        QStringList Files = JlCompress::extractDir(ModulePath, dir.absolutePath());
        if(Files.empty())
        {
            return tr("Failed to extract module");
        }

        dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QStringList dirList = dir.entryList();
        if(dirList.size() != 1)
        {
            return tr("Can't find folder containing module");
        }

        dir.cd(dirList.first());

        QString ManifestPath = dir.absoluteFilePath("manifest.json");

        if(!QFileInfo(ManifestPath).exists())
        {
            return tr("Can't find manifest.json file");
        }

        QFile Manifest(ManifestPath);

        QString ModuleName;
        QString ModuleNewVersion;

        if(Manifest.open(QIODevice::ReadOnly))
        {
            QByteArray json = Manifest.readAll();
            Manifest.close();
            QJsonParseError err;
            QJsonDocument doc = QJsonDocument::fromJson(json, &err);
            if(err.error != QJsonParseError::NoError)
            {
                return tr("Failed to parse manifest.json");
            }

            if(!doc.isObject())
            {
                return tr("manifest.json file has wrong format");
            }

            QJsonObject object = doc.object();

            if(!object.contains("name") || !object.value("name").isString())
            {
                return tr("manifest.json doesn't contain name property");
            }

            if(!object.contains("major_version") || !object.value("major_version").isDouble())
            {
                return tr("manifest.json doesn't contain major_version property");
            }

            if(!object.contains("minor_version") || !object.value("minor_version").isDouble())
            {
                return tr("manifest.json doesn't contain minor_version property");
            }

            ModuleNewVersion = QString::number(object.value("major_version").toInt()) + QString(".") + QString::number(object.value("minor_version").toInt());

            ModuleName = object.value("name").toString();
        }else
        {
            return tr("Can't read manifest.json file");
        }

        QStringList ModuleList;
        {
            QDir dir("custom");
            dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            ModuleList = dir.entryList();
        }

        if(ModuleList.contains(ModuleName))
        {
            //0 - Silent overwrite true
            //1 - Silent overwrite false
            //2 - Dialog
            if(OverwriteMode == 1)
            {
                return tr("Module already exists");
            }

            if(OverwriteMode == 2)
            {
                QFile Manifest(QString("custom/") + ModuleName + "/manifest.json");

                QString ModuleOldVersion;

                if(Manifest.open(QIODevice::ReadOnly))
                {
                    QByteArray json = Manifest.readAll();
                    Manifest.close();
                    QJsonParseError err;
                    QJsonDocument doc = QJsonDocument::fromJson(json, &err);
                    if(err.error == QJsonParseError::NoError && doc.isObject())
                    {
                        QJsonObject object = doc.object();

                        if(object.contains("major_version") && object.value("major_version").isDouble() && object.contains("minor_version") && object.value("minor_version").isDouble())
                        {
                            ModuleOldVersion = QString::number(object.value("major_version").toInt()) + QString(".") + QString::number(object.value("minor_version").toInt());
                        }
                    }
                }

                QMessageBox msgBox;


                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(QString(tr("Module '%1' already installed.\n\nCurrent version is %2.\nDo you want to override it with version %3?          \n")).arg(ModuleName).arg(ModuleOldVersion).arg(ModuleNewVersion));
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Cancel);
                if(msgBox.exec() == QMessageBox::Cancel)
                {
                    return tr("Module already exists");
                }


            }

            {

                //It is not always possible to remove folder, so we will try to clear its content.
                QDir dir(QString("custom/") + ModuleName);
                dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks);
                QStringList ModuleList = dir.entryList();
                for(QString& Module: ModuleList)
                {
                    QString Path = QString("custom/") + ModuleName + QString("/") + Module;
                    if(QFileInfo(Path).isDir())
                    {
                        if(!QDir(Path).removeRecursively())
                            return tr("Failed to remove folder ") + Path;
                    }else
                    {
                        if(!QFile(Path).remove())
                            return tr("Failed to remove file ") + Path;
                    }

                }


            }

        }

        {
            QDir dir(QString("custom/") + ModuleName);
            dir.mkpath(".");
        }

        if(!recurseCopyAddDir(dir,QDir(QString("custom/") + ModuleName),QStringList()))
        {
            return tr("Failed copy module");
        }

        dir.removeRecursively();

        return QString();
    }

    QString ModuleCreator::CreateModule(bool& ModuleInstalled, bool IsScriptRunning, QList<EmbeddedLanguage>& LanguageList, QList<EmbeddedModule>& ModuleList, QList<EmbeddedCodeItem>& CodeItems)
    {
        ModuleInstalled = false;
        {
            QDir dir("modulecreate/temp");
            dir.removeRecursively();
            dir.mkpath(".");
        }

        QDir dir(QString("modulecreate/temp/") + GetRandomString() + QString("/") + GetProperty("Name"));
        dir.mkpath(".");

        QFile EngineFile(dir.absoluteFilePath("engine.js"));
        EngineFile.open(QFile::WriteOnly);


        QJsonObject ManifestObject;

        QJsonArray CodeArray;
        QMap<QString,QString> EmbeddedMap;

        for(EmbeddedCodeItem& Code:CodeItems)
        {
            QJsonObject CodeObject;
            QString NewName;
            if(Code.DataType == 0)
                NewName = GetEmbeddedId();
            else
                NewName = Code.DataName;

            EmbeddedMap[Code.DataName] = NewName;

            CodeObject.insert("Data", QJsonValue::fromVariant(Code.Data));
            CodeObject.insert("DataName", QJsonValue::fromVariant(NewName));
            CodeObject.insert("DataType", QJsonValue::fromVariant(Code.DataType));
            CodeObject.insert("LanguageName", QJsonValue::fromVariant(Code.LanguageName));
            CodeObject.insert("LanguageVersion", QJsonValue::fromVariant(Code.LanguageVersion));
            CodeArray.append(CodeObject);
        }
        ManifestObject.insert("embeddeddata",CodeArray);

        QJsonArray LanguagesArray;
        for(EmbeddedLanguage& Language:LanguageList)
        {
            QJsonObject LanguageObject;
            LanguageObject.insert("Name", QJsonValue::fromVariant(Language.Name));
            LanguageObject.insert("Version", QJsonValue::fromVariant(Language.Version));
            LanguagesArray.append(LanguageObject);
        }
        ManifestObject.insert("languages",LanguagesArray);

        QJsonArray ModulesArray;
        for(EmbeddedModule& Module:ModuleList)
        {
            QJsonObject ModulesObject;
            ModulesObject.insert("LanguageName", QJsonValue::fromVariant(Module.LanguageName));
            ModulesObject.insert("LanguageVersion", QJsonValue::fromVariant(Module.LanguageVersion));
            ModulesObject.insert("ModuleName", QJsonValue::fromVariant(Module.ModuleName));
            ModulesObject.insert("ModuleVersion", QJsonValue::fromVariant(Module.ModuleVersion));
            ModulesArray.append(ModulesObject);
        }
        ManifestObject.insert("modules",ModulesArray);



        ManifestObject.insert("name", QJsonValue::fromVariant(GetProperty("Name")));

        QJsonObject InfoObject;
        InfoObject.insert("en",QJsonValue::fromVariant(GetProperty("DetailedDescriptionEn")));
        InfoObject.insert("ru",QJsonValue::fromVariant(GetProperty("DetailedDescriptionRu")));
        ManifestObject.insert("info", InfoObject);

        if(GetProperty("IsDefaultIcon") == "false")
        {
            ManifestObject.insert("icon", QJsonValue::fromVariant("icon.png"));
            QByteArray IconData = QByteArray::fromBase64(GetProperty("IconData").toUtf8());
            QFile IconFile(dir.absoluteFilePath("icon.png"));
            IconFile.open(QFile::WriteOnly);
            IconFile.write(IconData);
            IconFile.close();
        }else
        {
            ManifestObject.insert("icon", QJsonValue::fromVariant("icon.png"));
            QByteArray IconData;
            QFile f(":/engine/images/modules.png");
            if(f.open(QFile::ReadOnly))
            {
                IconData = f.readAll();
            }
            f.close();

            QFile IconFile(dir.absoluteFilePath("icon.png"));
            IconFile.open(QFile::WriteOnly);
            IconFile.write(IconData);
            IconFile.close();
        }

        ManifestObject.insert("description", QJsonValue::fromVariant(GetProperty("ShortDescriptionEn")));

        QJsonObject DescriptionObject;
        DescriptionObject.insert("en",QJsonValue::fromVariant(GetProperty("ShortDescriptionEn")));
        DescriptionObject.insert("ru",QJsonValue::fromVariant(GetProperty("ShortDescriptionRu")));
        ManifestObject.insert("description_small", DescriptionObject);

        ManifestObject.insert("major_version", QJsonValue::fromVariant(GetProperty("VersionMajor").toInt()));
        ManifestObject.insert("minor_version", QJsonValue::fromVariant(GetProperty("VersionMinor").toInt()));

        ManifestObject.insert("developer_name", QJsonValue::fromVariant(GetProperty("DeveloperName")));
        ManifestObject.insert("developer_email", QJsonValue::fromVariant(GetProperty("DeveloperMail")));
        ManifestObject.insert("developer_site", QJsonValue::fromVariant(GetProperty("DeveloperSite")));

        ManifestObject.insert("api_version", QJsonValue::fromVariant(1));
        ManifestObject.insert("is_autogenerated", QJsonValue::fromVariant(true));
        //ManifestObject.insert("is_custom", QJsonValue::fromVariant(true));
        ManifestObject.insert("localize", QJsonObject());
        ManifestObject.insert("browser", QJsonArray());
        ManifestObject.insert("depends", QJsonArray());
        QJsonArray EngineArray;
        EngineArray.append(QJsonValue::fromVariant("engine.js"));
        ManifestObject.insert("engine", EngineArray);


        QList<FunctionData> Data = ParseCode();

        QJsonArray ActionsArray;

        bool UseFuncitonMap = GetProperty("RandomizeFunctionName") == "true" || GetProperty("RandomizeFunctionName").isEmpty();
        QMap<QString,QString> FunctionMap;

        QStringList ExcludedFunctions = GetProperty("ExcludedFunctions").split(",");

        if(UseFuncitonMap)
        {
            QStringList FunctionList;
            for(FunctionData& FuncData:Data)
            {
                QString NewName = GetProperty("Name") + QString("_") + FuncData.Name/* + QString("_") + GetRandomString()*/;
                if(!ExcludedFunctions.contains(NewName))
                    FunctionList.append(NewName);
                FunctionMap.insert(FuncData.Name,NewName);
            }
            ManifestObject.insert("autogenerated_functions", QJsonValue::fromVariant(FunctionList));
        }else
        {
            QStringList FunctionList;
            for(FunctionData& FuncData:Data)
            {
                if(!ExcludedFunctions.contains(FuncData.Name))
                    FunctionList.append(FuncData.Name);
            }
            ManifestObject.insert("autogenerated_functions", QJsonValue::fromVariant(FunctionList));
        }


        for(FunctionData& FuncData:Data)
        {
            if(ExcludedFunctions.contains(FuncData.Name) || FuncData.Name == "OnApplicationStart")
            {
                continue;
            }
            QString NewFunctionName;
            if(UseFuncitonMap)
            {
                NewFunctionName = FunctionMap.value(FuncData.Name);
            }

            QJsonObject ActionObject;
            QString ActionId = GetProperty("Name") + QString("_") + FuncData.Name;
            ActionObject.insert("name", QJsonValue::fromVariant(ActionId));

            QJsonObject DescriptionObject;
            DescriptionObject.insert("en",QJsonValue::fromVariant(FuncData.Name));
            DescriptionObject.insert("ru",QJsonValue::fromVariant(FuncData.Name));
            ActionObject.insert("description", DescriptionObject);

            ActionObject.insert("template", QJsonValue::fromVariant(""));
            ActionObject.insert("is_element", QJsonValue::fromVariant(false));
            ActionObject.insert("interface", QJsonValue::fromVariant(ActionId + QString("_interface.js")));
            ActionObject.insert("select", QJsonValue::fromVariant(ActionId + QString("_select.js")));

            QJsonArray CodeArray;
            QJsonObject CodeObject;

            CodeObject.insert("file", QJsonValue::fromVariant(ActionId + QString("_code.js")));
            CodeObject.insert("name", QJsonValue::fromVariant(ActionId + QString("_code")));

            CodeArray.append(CodeObject);

            ActionObject.insert("code", CodeArray);

            ActionsArray.append(ActionObject);

            WriteAction(
                        dir.absoluteFilePath(ActionId + QString("_interface.js")),
                        dir.absoluteFilePath(ActionId + QString("_select.js")),
                        dir.absoluteFilePath(ActionId + QString("_code.js")),
                        ActionId + QString("_code"),
                        FuncData,
                        UseFuncitonMap,
                        FunctionMap);

            QString Code = FuncData.Code;
            if(UseFuncitonMap)
            {
                QRegularExpression Regexp("^\\s*function\\s+[^\\(]+");
                Code = Code.replace(Regexp, QString("function ") + NewFunctionName);
            }

            QMap<QString, QString>::iterator i;
            for (i = EmbeddedMap.begin(); i != EmbeddedMap.end(); ++i)
            {
                Code = Code.replace(i.key(), i.value());
            }

            EngineFile.write(PrepareCode(Code,UseFuncitonMap,FunctionMap).toUtf8());
            EngineFile.write("\r\n\r\n");
        }

        ManifestObject.insert("actions", ActionsArray);


        QJsonDocument ManifestDocument(ManifestObject);

        QFile ManifestFile(dir.absoluteFilePath("manifest.json"));
        ManifestFile.open(QFile::WriteOnly);
        ManifestFile.write(ManifestDocument.toJson());
        ManifestFile.close();

        EngineFile.close();

        QDir DirArchive(QString("modulecreate/") + GetProperty("Name") + QString("/") + QDateTime::currentDateTime().toString("yyyy-MM-dd HH.mm.ss"));

        DirArchive.removeRecursively();
        DirArchive.mkpath(".");

        QString ResultPath = DirArchive.absoluteFilePath(GetProperty("Name") + QString(".zip"));
        dir.cdUp();
        QString CompressFolder = dir.absolutePath();

        JlCompress::compressDir(ResultPath,CompressFolder);

        bool IsInstallModule = GetProperty("InstallModule") == "true" || GetProperty("InstallModule").isEmpty();

        if(IsInstallModule)
        {
            if(IsScriptRunning)
            {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText(tr("Can't install module while script is running."));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();

            }else
            {

                QString InstallResult = InstallModule(ResultPath,0);

                if(!InstallResult.isEmpty())
                {
                    QMessageBox msgBox;
                    msgBox.setIcon(QMessageBox::Critical);
                    msgBox.setText(QString(tr("Failed to install module.\n\n%1             \n")).arg(InstallResult));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.exec();
                }else
                {
                    ModuleInstalled = true;
                }
            }
        }


        return DirArchive.absolutePath();
    }

    void ModuleCreator::WriteAction(const QString& InterfaceFilename,const QString& SelectFilename,const QString& CodeFilename,const QString& CodeId, FunctionData& Data, bool UseFunctionMap, QMap<QString,QString>& FunctionMap)
    {
        QFile InterfaceFile(InterfaceFilename);
        InterfaceFile.open(QFile::WriteOnly);
        QFile SelectFile(SelectFilename);
        SelectFile.open(QFile::WriteOnly);
        QFile CodeFile(CodeFilename);
        CodeFile.open(QFile::WriteOnly);


        InterfaceFile.write("<div class=\"container-fluid\">\r\n");

        QStringList SelectHash;
        QStringList CallHash;

        for(ParameterData& Param:Data.Parameters)
        {
            QString Id = GetRandomString();

            QString Parameter;

            if(Param.Type == "String")
            {
                Parameter = QString(R"DATA(
                       <%= _.template($('#input_constructor').html())({id:"%1", description:%2, default_selector: "string", disable_expression:true, disable_int:true, value_string: %3, help: {description: %4} }) %>
                   )DATA")
                   .arg(Id)
                   .arg(JavascriptEscape(Param.Name))
                   .arg(JavascriptEscape(Param.Defaults))
                   .arg(JavascriptEscape(Param.Description))
                    ;
            }else if(Param.Type == "Number")
            {
                Parameter = QString(R"DATA(
                       <%= _.template($('#input_constructor').html())({id:"%1", description:%2, default_selector: "int", disable_expression:true, disable_string:true, value_number: %3, min_number:-999999, max_number:999999, help: {description: %4} }) %>
                   )DATA")
                   .arg(Id)
                   .arg(JavascriptEscape(Param.Name))
                   .arg(Param.Defaults.isEmpty() ? QString("0") : Param.Defaults)
                   .arg(JavascriptEscape(Param.Description))
                    ;
            }else if(Param.Type == "Boolean")
            {
                Parameter = QString(R"DATA(
                       <%= _.template($('#input_constructor').html())({id:"%1", description:%2, default_selector: "string", variants: ["true","false"], disable_int:true, value_string: %3, help: {description: %4} }) %>
                   )DATA")
                   .arg(Id)
                   .arg(JavascriptEscape(Param.Name))
                   .arg(JavascriptEscape(Param.Defaults))
                   .arg(JavascriptEscape(Param.Description))
                    ;
            }else if(Param.Type == "Expression")
            {
                Parameter = QString(R"DATA(
                       <%= _.template($('#input_constructor').html())({id:"%1", description:%2, default_selector: "expression", disable_int:true, disable_string:true, value_string: %3, help: {description: %4} }) %>
                   )DATA")
                   .arg(Id)
                   .arg(JavascriptEscape(Param.Name))
                   .arg(JavascriptEscape(Param.Defaults))
                   .arg(JavascriptEscape(Param.Description))
                    ;
            }else if(Param.Type == "StringOrExpression")
            {
                Parameter = QString(R"DATA(
                       <%= _.template($('#input_constructor').html())({id:"%1", description:%2, default_selector: "string", disable_int:true, value_string: %3, help: {description: %4} }) %>
                   )DATA")
                   .arg(Id)
                   .arg(JavascriptEscape(Param.Name))
                   .arg(JavascriptEscape(Param.Defaults))
                   .arg(JavascriptEscape(Param.Description))
                    ;
            }else if(Param.Type == "NumberOrExpression")
            {
                Parameter = QString(R"DATA(
                       <%= _.template($('#input_constructor').html())({id:"%1", description:%2, default_selector: "int", disable_string:true, value_number: %3, min_number:-999999, max_number:999999, help: {description: %4} }) %>
                   )DATA")
                   .arg(Id)
                   .arg(JavascriptEscape(Param.Name))
                   .arg(Param.Defaults.isEmpty() ? QString("0") : Param.Defaults)
                   .arg(JavascriptEscape(Param.Description))
                    ;
            }

            Parameter = Parameter.trimmed();
            Parameter += "\r\n";
            InterfaceFile.write(Parameter.toUtf8());


            QString SelectData = QString(R"DATA(
                var %1 = GetInputConstructorValue("%1", loader);
                 if(%1["original"].length == 0)
                 {
                   Invalid(%2 + " is empty");
                   return;
                 }
            )DATA")
            .arg(Id)
            .arg(JavascriptEscape(Param.Name));

            SelectData = SelectData.trimmed();
            SelectData += "\r\n";
            SelectFile.write(SelectData.toUtf8());

            SelectHash.append(JavascriptEscape(Id) + QString(": ") + Id + QString("[\"updated\"]"));
            CallHash.append(JavascriptEscape(Param.Name) + QString(": (<%= ") + Id + QString(" %>)"));
        }





        if(Data.HasReturnValue)
        {
            QString ReturnValue = QString(R"DATA(
            <%= _.template($('#variable_constructor').html())({id:"Save", description:%1, default_variable: %2, help: {description: %3}}) %>
            )DATA")
              .arg(JavascriptEscape(tr("Result")))
              .arg(JavascriptEscape(Data.ReturnVariableName))
              .arg(JavascriptEscape(Data.ReturnValueDescription))
              ;


            ReturnValue = ReturnValue.trimmed();
            ReturnValue += "\r\n";
            InterfaceFile.write(ReturnValue.toUtf8());

            SelectHash.append(QString("\"variable\": \"VAR_\" + Save"));

            SelectFile.write("var Save = this.$el.find(\"#Save\").val().toUpperCase();\r\n");

        }

        {
            QString FunctionName = Data.Name;
            if(UseFunctionMap && FunctionMap.contains(FunctionName))
            {
                FunctionName = FunctionMap.value(FunctionName);
            }
            QString Code = QString(R"DATA(
              _call_function(%1,{ %2 })!

            )DATA")
               .arg(FunctionName)
               .arg(CallHash.join(","));

            Code = Code.trimmed();
            Code += "\r\n";
            CodeFile.write(Code.toUtf8());

        }

        if(Data.HasReturnValue)
        {
            QString CodeReturn =  QString("<%= variable %> = _result_function()\r\n");
            CodeFile.write(CodeReturn.toUtf8());

        }

        QString SelectFinal = QString(R"DATA(
        try{
          var code = loader.GetAdditionalData() + _.template($("#%1").html())({%2});
          code = Normalize(code,0);
          BrowserAutomationStudio_Append("", BrowserAutomationStudio_SaveControls() + code, action, DisableIfAdd);
        }catch(e)
        {}
        )DATA")


        .arg(CodeId)
        .arg(SelectHash.join(","));

        SelectFinal = SelectFinal.trimmed();
        SelectFinal += "\r\n";
        SelectFile.write(SelectFinal.toUtf8());

        InterfaceFile.write("</div>\r\n");
        InterfaceFile.write("<div class=\"tooltipinternal\">\r\n");
        QStringList LinesDescription = Data.Description.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
        int index = 0;
        for(QString &Line:LinesDescription)
        {
            if(index == 0)
                InterfaceFile.write("<div class=\"tr tooltip-paragraph-first-fold\">");
            else if(index == LinesDescription.size() - 1)
                InterfaceFile.write("<div class=\"tr tooltip-paragraph-last-fold\">");
            else
                InterfaceFile.write("<div class=\"tr tooltip-paragraph-fold\">");
            InterfaceFile.write(Line.toUtf8());
            InterfaceFile.write("</div>\r\n");
            index++;
        }
        InterfaceFile.write("</div>\r\n");
        InterfaceFile.write("<%= _.template($('#back').html())({action:\"executeandadd\", visible:true}) %>\r\n");

        InterfaceFile.close();
        SelectFile.close();
        CodeFile.close();
    }

    QString ModuleCreator::JavascriptEscape(const QString& Data)
    {
        QString res = QString::fromUtf8(QJsonDocument::fromVariant(QVariant(QStringList()<<Data)).toJson(QJsonDocument::Compact));
        res = res.remove(0,1);
        res = res.remove(res.length() - 1,1);
        return res;
    }

    QList<ModuleCreator::FunctionData> ModuleCreator::ParseCode()
    {
        QList<FunctionData> DataList;

        int CodeStart = -1;

        QRegularExpression Regexp("section_start\\((\\\"[^\\\"]+\\\")\\s*\\,\\s*\\d+\\)\\!\\s*function");
        Regexp.setPatternOptions(QRegularExpression::MultilineOption);
        QRegularExpressionMatchIterator RegexpIterator = Regexp.globalMatch(Script);


        while(RegexpIterator.hasNext())
        {
            QRegularExpressionMatch match = RegexpIterator.next();
            QString FunctionName = match.captured(1);

            QString FunctionDataRaw;
            {
                QScriptEngine Engine;
                FunctionDataRaw = Engine.evaluate(FunctionName).toString();
            }

            QJsonParseError err;
            QJsonDocument FunctionDocument = QJsonDocument::fromJson(FunctionDataRaw.toUtf8(),&err);

            if(!err.error && FunctionDocument.isObject())
            {
                QJsonObject FunctionObject = FunctionDocument.object();
                if(FunctionObject.contains("n") && FunctionObject.contains("fd"))
                {
                    QString FunctionNameString = FunctionObject.value("n").toString();
                    QString FunctionDataString = FunctionObject.value("fd").toString();

                    QJsonDocument FunctionDataDocument = QJsonDocument::fromJson(FunctionDataString.toUtf8(),&err);
                    if(!err.error && FunctionDataDocument.isObject())
                    {
                        QJsonObject FunctionDataObject = FunctionDataDocument.object();

                        FunctionData Data;
                        Data.Name = FunctionNameString;
                        Data.Description = FunctionDataObject.value("fd").toString();
                        Data.HasReturnValue = FunctionDataObject.value("hr").toBool();
                        Data.ReturnValueDescription = FunctionDataObject.value("rd").toString();
                        Data.ReturnVariableName = FunctionDataObject.value("rv").toString();

                        if(FunctionDataObject.value("p").isArray())
                        {
                            QJsonArray ParametersArray = FunctionDataObject.value("p").toArray();
                            for(QJsonValue Value: ParametersArray)
                            {
                                QJsonObject ParameterObject = Value.toObject();
                                ParameterData Param;

                                Param.Defaults = ParameterObject.value("defaults").toString();
                                Param.Description = ParameterObject.value("description").toString();
                                Param.Name = ParameterObject.value("name").toString();
                                Param.Type = ParameterObject.value("type").toString();

                                Data.Parameters.append(Param);
                            }
                        }


                        if(CodeStart >= 0 && !DataList.isEmpty())
                        {
                            QString NewCode = Script.mid(CodeStart, match.capturedStart() - CodeStart);
                            int SectionEndIndex = NewCode.lastIndexOf("section_end()!");
                            int Len = NewCode.length() - SectionEndIndex;
                            NewCode = NewCode.remove(SectionEndIndex,Len);
                            DataList.last().Code = NewCode;
                        }
                        DataList.append(Data);

                        CodeStart = match.capturedStart() + match.capturedLength() - 8;
                    }
                }
            }

        }

        if(CodeStart > 0)
        {
            int CodeEnd = Script.indexOf("_call(_on_start, null)!");
            if(CodeEnd >= 0 && !DataList.isEmpty())
            {
                QString NewCode = Script.mid(CodeStart, CodeEnd - CodeStart);
                int SectionEndIndex = NewCode.lastIndexOf("section_end()!");
                int Len = NewCode.length() - SectionEndIndex;
                NewCode = NewCode.remove(SectionEndIndex,Len);
                DataList.last().Code = NewCode;
            }

        }

        return DataList;
    }

    QStringList ModuleCreator::GetFunctionList()
    {
        QStringList Res;

        QList<FunctionData> Data = ParseCode();

        for(FunctionData f:Data)
        {
            Res.append(f.Name);
        }

        Res.removeAll("OnApplicationStart");

        Res.sort(Qt::CaseInsensitive);

        return Res;
    }
}
