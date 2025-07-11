#include "preprocessor.h"
#include <QRegExp>
#include <QMap>
#include <QRegularExpressionMatch>
#include "every_cpp.h"


namespace BrowserAutomationStudioFramework
{
    Preprocessor::Preprocessor(QObject *parent) :
        IPreprocessor(parent)
    {
        Encryptor = 0;
        IsRecord = false;
    }

    void Preprocessor::SetIsRecord(bool IsRecord)
    {
        this->IsRecord = IsRecord;
    }

    bool Preprocessor::GetIsRecord()
    {
        return IsRecord;
    }

    bool Preprocessor::IsInsideComment(const QString& str,int start)
    {
        int j = start - 1;
        QString start_str;

        while(j>=0 && str[j] != '\r' && str[j] != '\n')
        {
            start_str.insert(0,str[j]);
            --j;
        }


        start_str.replace(QRegExp("\\\\\\\""),"");
        start_str.replace(QRegExp("\\\\\\'"),"");

        QRegExp r1("\\\".*\\\"");
        r1.setMinimal(true);
        start_str.replace(r1,"");

        QRegExp r2("\\'.*\\'");
        r2.setMinimal(true);
        start_str.replace(r2,"");

        bool res = start_str.contains("\'") || start_str.contains("\"");
        return res;
    }

    Preprocessor::GotoLabelData Preprocessor::ParseSetGotoLabel(const QString& str,int start)
    {
        GotoLabelData res;
        int index = str.lastIndexOf("(",start);
        if(index<0)
        {
            res.IsGotoLabel = false;
            return res;
        }
        int IndexBracket = index;
        QString func_name = "_set_goto_label";
        index -= func_name.length();
        if(index<0)
        {
            res.IsGotoLabel = false;
            return res;
        }
        if(str.mid(index,func_name.length()) == func_name)
        {
            QString Label = str.mid(IndexBracket + 1,start - IndexBracket-1);
            if(Label.contains(")!"))
            {
                res.IsGotoLabel = false;
            }else
            {
                res.IsGotoLabel = true;
                res.Index = index;
                res.Label = str.mid(IndexBracket + 1,start - IndexBracket-1);
            }
            return res;
        }else
        {
            res.IsGotoLabel = false;
            return res;
        }

    }

    void Preprocessor::SetEncryptor(IEncryptor* Encryptor)
    {
        this->Encryptor = Encryptor;
    }

    IEncryptor* Preprocessor::GetEncryptor()
    {
        return this->Encryptor;
    }


    void Preprocessor::SetKey(const QByteArray& Key)
    {
        this->Key = Key;
    }

    QString Preprocessor::Encrypt(const QString& Script,int ParanoicLevel)
    {
        if(ParanoicLevel == 0 || Script.size() > 100000 || Script.size() < 100)
            return Script;

        if(!Script.contains("section("))
        {
            EncryptIterator++;
            if(EncryptIterator % ParanoicLevel == 0)
                //Encryption is done on server
                return QString("_BAS_DEC('") + Script.toUtf8().toBase64() + QString("')");
        }

        return Script;
    }

    QString Preprocessor::Encrypt(const QString& Script)
    {
        return QString("_BAS_DEC('") + Script.toUtf8().toBase64() + QString("')");
    }

    QString Preprocessor::Decrypt(const QString& Script)
    {
        return QString::fromUtf8(Encryptor->HybridDecrypt(QByteArray::fromBase64(Script.toUtf8()),Key));
    }

    void Preprocessor::PreprocessInternal(QString& Res,int ParanoicLevel,QMap<QString,QString>& GotoData)
    {
        int from = 0;
        int fast_search_last = -1;
        while(true)
        {
            int length = Res.length();
            int index_start = Res.lastIndexOf(")!", fast_search_last);
            //int index_start = Res.indexOf(")!", from);
            from = index_start + 2;
            bool is_one_argument = false;
            int index_search = index_start - 1;
            while(true)
            {
                if(index_search>=0)
                {
                    QChar c = Res.at(index_search);
                    if(c.isSpace())
                    {

                    }else if(c == '(')
                    {
                        is_one_argument = true;
                        break;
                    }
                    else
                    {
                        break;
                    }

                    index_search--;
                }else
                    break;
            }

            if(Res.indexOf(")!")<0)
            {
                break;
            }
            int index_end = index_start + 2;
            int open_bracket = 0;
            while(true)
            {
                if(index_end >= length - 1)
                {
                    QString all = Res.mid(index_start + 2);
                    if(all.indexOf(")!")>=0)
                        break;
                    GotoLabelData ParseData = ParseSetGotoLabel(Res,index_start);
                    if(ParseData.IsGotoLabel)
                    {
                        Res = Res.replace(ParseData.Index,index_end - ParseData.Index,"_fast_goto(" + ParseData.Label + ")!");
                        GotoData[ParseData.Label] = Encrypt(all,ParanoicLevel);
                    }else
                    {
                        Res = Res.replace(index_start + 2,all.length(),Encrypt(all,ParanoicLevel));
                        if(is_one_argument)
                            Res = Res.replace(index_start,2,"function(){");
                        else
                            Res = Res.replace(index_start,2,", function(){");

                        Res = Res.append("})");
                    }
                    from = 0;
                    fast_search_last = -1;

                    break;
                }else
                {
                    QChar c = Res.at(index_end);
                    if(c == '{')
                    {
                        open_bracket ++;
                    }else if(c == '}')
                    {
                        open_bracket --;
                        if(open_bracket<0)
                        {
                            QString all = Res.mid(index_start + 2, index_end - index_start - 2);
                            if(all.indexOf(")!")>=0)
                                break;


                            GotoLabelData ParseData = ParseSetGotoLabel(Res,index_start);
                            if(ParseData.IsGotoLabel)
                            {
                                QString ReplaceResultData = "_fast_goto(" + ParseData.Label + ")!";
                                int ReplaceResultLength = index_end - ParseData.Index;
                                Res = Res.replace(ParseData.Index,ReplaceResultLength,ReplaceResultData);
                                fast_search_last = ParseData.Index + ReplaceResultData.length();
                                GotoData[ParseData.Label] = Encrypt(all,ParanoicLevel);
                            }else
                            {
                                Res = Res.replace(index_end,1,"})}");
                                Res = Res.replace(index_start + 2,all.length(),Encrypt(all,ParanoicLevel));

                                QString replace_string = "function(){";
                                if(!is_one_argument)
                                    replace_string = "," + replace_string;

                                Res = Res.replace(index_start,2,replace_string);
                                fast_search_last = index_start;
                            }

                            from = 0;

                            break;
                        }
                    }

                    if(index_end == length - 1)
                    {
                        QString all = Res.mid(index_start + 2);
                        if(all.indexOf(")!")>=0)
                            break;

                        GotoLabelData ParseData = ParseSetGotoLabel(Res,index_start);
                        if(ParseData.IsGotoLabel)
                        {
                            Res = Res.replace(ParseData.Index,index_end - ParseData.Index,"_fast_goto(" + ParseData.Label + ")!");
                            GotoData[ParseData.Label] = Encrypt(all,ParanoicLevel);
                        }else
                        {
                            Res = Res.replace(index_start + 2,all.length(),Encrypt(all,ParanoicLevel));

                            Res.append("})");
                            QString replace_string = "function(){";
                            if(!is_one_argument)
                                replace_string = "," + replace_string;

                            Res = Res.replace(index_start,2,replace_string);
                        }
                        from = 0;
                        fast_search_last = -1;

                        break;
                    }
                    index_end++;
                }

            }

        }
    }

    void Preprocessor::PreprocessSections(QString& Res)
    {
        if(!IsRecord)
        {
            {
                QRegExp Regexp("section\\_start\\(\\s*\\\"[^\\\"]*\\\"\\s*\\,\\s*(\\-?\\d*)\\)\\!");
                int pos = 0;
                while ((pos = Regexp.indexIn(Res, pos)) != -1)
                {
                    QString IdString = Regexp.cap(1);
                    if(!IdString.isEmpty())
                    {
                        IdString = QString(";_sa(") + IdString + QString(");");
                    }
                    else
                    {
                        IdString = ";";
                    }
                    Res.replace(Regexp.pos(),Regexp.matchedLength(),IdString);
                }
            }

            {
                QRegExp Regexp("section\\_end\\(\\)\\!");
                /*int pos = 0;
                while ((pos = Regexp.indexIn(Res, pos)) != -1)
                {
                    Res.replace(Regexp.pos(),Regexp.matchedLength(),";");
                }*/
                Res.replace(Regexp,";");
            }

            {
                QRegExp Regexp("section\\_insert\\(\\)");
                /*int pos = 0;
                while ((pos = Regexp.indexIn(Res, pos)) != -1)
                {
                    Res.replace(Regexp.pos(),Regexp.matchedLength(),"");
                }*/
                Res.replace(Regexp,"");
            }

            {
                QRegExp Regexp("wait\\_code\\(\\)\\!");
                /*int pos = 0;
                while ((pos = Regexp.indexIn(Res, pos)) != -1)
                {
                    Res.replace(Regexp.pos(),Regexp.matchedLength(),"");
                }*/
                Res.replace(Regexp,"");
            }
        }
    }

    Preprocessor::ParsedMainScript Preprocessor::ParseMainScript(const QString& Script)
    {
        ParsedMainScript Result;

        QRegularExpression FunctionRegexp("section_start\\([^\\)]+\\\\u0022fd\\\\u0022\\:\\\\u0022[^\\)]+\\)!\\s+function\\s+[^\\(]+\\(");
        QRegularExpression MainRegexp("_call\\(_on_start,\\s*null\\)!");

        //Check start of main function
        int MainIndex = -1;
        {
            QRegularExpressionMatch MainMatch = MainRegexp.match(Script, 0);
            if(MainMatch.hasMatch())
            {
                MainIndex = MainMatch.capturedStart();
            }
        }
        if(MainIndex < 0)
        {
            //Main function not found, ignore per function preprocessing
            Result.MainScript = Script;
            return Result;
        }

        int Start = 0;
        int FirstFunctionIndex = -1;
        int LastFunctionIndex = -1;
        while(true)
        {
            //Search for next function
            int FunctionIndex = -1;
            QRegularExpressionMatch FunctionMatch = FunctionRegexp.match(Script, Start);
            if(FunctionMatch.hasMatch())
            {
                FunctionIndex = FunctionMatch.capturedStart();
                if(FirstFunctionIndex == -1)
                {
                    FirstFunctionIndex = FunctionIndex;
                }
            }

            if(FunctionIndex > MainIndex)
            {
                FunctionIndex = -1;
            }


            if(LastFunctionIndex >= 0)
            {
                if(FunctionIndex >= 0)
                {
                    //There is next function
                    QString FunctionBody = Script.mid(LastFunctionIndex,FunctionIndex - LastFunctionIndex);
                    Result.Functions.append(FunctionBody);
                    Start = FunctionIndex + FunctionMatch.capturedLength() + 1;
                }else
                {
                    //This is last function
                    QString FunctionBody = Script.mid(LastFunctionIndex,MainIndex - LastFunctionIndex);
                    Result.Functions.append(FunctionBody);
                    break;
                }

            }else
            {
                if(FunctionIndex >= 0)
                {
                    //There is first function, do nothing
                    Start = FunctionIndex + FunctionMatch.capturedLength() + 1;
                }else
                {
                    //No functions in this script, break
                    break;
                }

            }



            LastFunctionIndex = FunctionIndex;
        }

        if(Result.Functions.empty())
        {
            //Zero functions found
            Result.MainScript = Script;
            return Result;
        }else
        {
            //At least one function found
            Result.MainScript = Script;
            Result.MainScript.replace(FirstFunctionIndex, MainIndex - FirstFunctionIndex, "_BAS_FUNCTION_BODY_");
            return Result;
        }


        return Result;
    }

    

    QString Preprocessor::Preprocess(const QString& Script,int ParanoicLevel,bool IsMainScript)
    {
        QMap<QString,QString> GotoData;
        EncryptIterator = 0;
        QString Res = Script;

        {
            QRegExp Regexp("\\/\\*Dat\\:.*\\*\\/");
            Regexp.setMinimal(true);

            Res.replace(Regexp,"");
        }

        {
            Res.replace("\/*Browser*\/","");
        }

        {
            QRegExp Regexp("\\/\\*.*\\*\\/");
            Regexp.setMinimal(true);

            int pos = 0;
            while ((pos = Regexp.indexIn(Res, pos)) != -1)
            {
                bool Remove = !IsInsideComment(Res,Regexp.pos());
                if(Remove)
                {
                    Res.remove(Regexp.pos(),Regexp.matchedLength());
                }else
                {
                    pos += Regexp.matchedLength();
                }


            }
        }
        {
            QRegExp Regexp("\\/\\/([^\\r\\n]+)");
            int pos = 0;
            while ((pos = Regexp.indexIn(Res, pos)) != -1)
            {
                bool Remove = !IsInsideComment(Res,Regexp.pos());
                if(Remove)
                {
                    Res.replace(Regexp.pos(),Regexp.matchedLength(),"");
                }else
                {
                    pos += Regexp.matchedLength();
                }
            }
        }


        if(IsMainScript)
        {
            ParsedMainScript ParsedData = ParseMainScript(Res);
            int FunctionLength = ParsedData.Functions.length();

            PreprocessSections(ParsedData.MainScript);
            PreprocessInternal(ParsedData.MainScript,ParanoicLevel,GotoData);

            int FunctionsPosition = -1;

            if(FunctionLength > 0)
            {
                FunctionsPosition = ParsedData.MainScript.indexOf("_BAS_FUNCTION_BODY_");
                if(FunctionsPosition >= 0)
                    ParsedData.MainScript.remove(FunctionsPosition,19);
            }
            QString AllFunctions;

            for(int i = FunctionLength - 1;i >= 0;i--)
            {
                QString FunctionStringCopy = ParsedData.Functions[i];
                PreprocessSections(FunctionStringCopy);
                PreprocessInternal(FunctionStringCopy,ParanoicLevel,GotoData);
                AllFunctions.prepend(FunctionStringCopy);

            }

            ParsedData.MainScript.insert(FunctionsPosition,AllFunctions);

            Res = ParsedData.MainScript;

        }else
        {
            PreprocessSections(Res);

            PreprocessInternal(Res,ParanoicLevel,GotoData);
        }

        //Insert goto data
        if(!GotoData.empty())
        {
            QString payload = "if(typeof(_BAS_GOTO_DATA) == 'undefined'){_BAS_GOTO_DATA = {};};";
            //QString payload = "_BAS_GOTO_DATA = {};";

            QMapIterator<QString, QString> i(GotoData);
            while (i.hasNext())
            {
                i.next();
                payload.append("_BAS_GOTO_DATA[" + i.key() + "] = function(){" + i.value() + "};");
            }

            int index = Res.indexOf("section(");
            if(index >= 0)
            {
                index = Res.indexOf("function(){", index);
                if(index >= 0)
                {
                    Res = Res.insert(index + 11,payload);
                }
            }else
            {
                Res = Res.insert(0,payload);
            }
        }
        return Res;

    }



}
