#include "compiler.h"
#include <QDir>
#include <QRegExp>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QDirIterator>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QBuffer>
#include <windows.h>
#include "every_cpp.h"

Compiler::Compiler(QObject *parent) :
    QObject(parent)
{
    HideBrowsers = false;
    IntegrateScheduler = false;
    SingleInstance = false;
    IsEnginesInAppData = false;
    IsCopySilent = false;
}

void Compiler::UpdateExeIcon(const QString& ExePath)
{
    QByteArray DataOriginal = QByteArray::fromBase64(GetIcon().toUtf8());
    QImage ImageOriginal;
    ImageOriginal.loadFromData(DataOriginal);

    QList<QImage> Images;
    QList<QByteArray> Data;
    QList<int> Dims;
    Dims.append(16);
    Dims.append(32);
    Dims.append(48);
    Dims.append(64);
    Dims.append(128);


    for(int dim: Dims)
    {
        if(dim <= ImageOriginal.width())
        {
            QByteArray data;
            QBuffer buffer(&data);
            buffer.open(QIODevice::WriteOnly);

            ImageOriginal.scaled(dim,dim,Qt::IgnoreAspectRatio,Qt::SmoothTransformation).save(&buffer, "ICO",100);
            buffer.close();

            QImage image;
            image.loadFromData(data,"ICO");
            Images.append(image);
            Data.append(data);
        }else
            break;
    }


    HANDLE hWhere = BeginUpdateResource(ExePath.toStdWString().data(), FALSE);

    for(int i = 1;i<=6;i++)
        UpdateResource(
             hWhere,
             RT_ICON,
             MAKEINTRESOURCE(i),
             MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
             0,
             0
            );

    UpdateResource(
         hWhere,
         RT_GROUP_ICON,
         L"IDI_ICON1",
         MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
         0,
         0
        );

    QByteArray grData;
    QDataStream out(&grData, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out << (qint16)0;
    out << (qint16)1;
    out << (qint16)Data.size();

    int i = 1;
    for(QByteArray& data:Data)
    {
      QImage& Image = Images[i-1];
        if(i == Data.length())
        {
            out << (qint8)0;
            out << (qint8)0;
        }else
        {
            out << (qint8)Image.width();
            out << (qint8)Image.height();
        }
        out << (qint8)Image.colorCount();
        out << (qint8)0;
        out << (qint16)1;
        out << (qint16)Image.depth();
        out << (qint32)(data.size()-22);
        out << (qint16)(i);

    UpdateResource(
         hWhere,
         RT_ICON,
         MAKEINTRESOURCE(i++),
         MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
         (data.data()+22),
         data.size()-22
        );


    }


    UpdateResource(
        hWhere,
        RT_GROUP_ICON,
        L"MAINICON",
        MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        grData.data(),
        grData.size()
    );


    EndUpdateResource(hWhere, FALSE);

}

void Compiler::Reset()
{
    HideBrowsers = false;
    IntegrateScheduler = false;
    SingleInstance = false;
    IsEnginesInAppData = false;
    IsCopySilent = false;
    Username.clear();
    Password.clear();
    Version = "1.0.0";
    Name = "ScriptName";
    Icon.clear();
    IsCustomIcon = false;
    ProtectionStrength = 4;
}

void Compiler::SetHideBrowsers(bool HideBrowsers)
{
    this->HideBrowsers = HideBrowsers;
}

bool Compiler::GetHideBrowsers()
{
    return HideBrowsers;
}

void Compiler::SetIntegrateScheduler(bool IntegrateScheduler)
{
    this->IntegrateScheduler = IntegrateScheduler;
}

bool Compiler::GetIntegrateScheduler()
{
    return IntegrateScheduler;
}


void Compiler::SetSingleInstance(bool SingleInstance)
{
    this->SingleInstance = SingleInstance;
}

bool Compiler::GetSingleInstance()
{
    return SingleInstance;
}

void Compiler::SetCopySilent(bool CopySilent)
{
    this->IsCopySilent = CopySilent;
}

bool Compiler::GetCopySilent()
{
    return IsCopySilent;
}

bool Compiler::GetIsEnginesInAppData()
{
    return IsEnginesInAppData;
}

bool Compiler::GetIsCustomIcon()
{
    return IsCustomIcon;
}

void Compiler::SetIsEnginesInAppData(bool IsEnginesInAppData)
{
    this->IsEnginesInAppData = IsEnginesInAppData;
}



void Compiler::SetName(const QString & Name)
{
    this->Name = Name;
}

void Compiler::SetProtectionStrength(int ProtectionStrength)
{
    this->ProtectionStrength = ProtectionStrength;
}

void Compiler::SetIcon(const QString & Icon)
{
    this->Icon = Icon;
}
void Compiler::SetIsCustomIcon(bool IsCustomIcon)
{
    this->IsCustomIcon = IsCustomIcon;
}
void Compiler::SetVersion(const QString & Version)
{
    this->Version = Version;
}

void Compiler::SetUsername(const QString & Username)
{
    this->Username = Username;
}

void Compiler::SetPassword(const QString & Password)
{
    this->Password = Password;
}

Compiler::CompileType Compiler::GetType()
{
    return Type;
}

void Compiler::SetType(const CompileType & Type)
{
    this->Type = Type;
}

void Compiler::SetTypeString(const QString& TypeString)
{
    if(TypeString == "NoProtection")
        Type = NoProtection;
    if(TypeString == "PrivateScriptEnterPassForUser")
        Type = PrivateScriptEnterPassForUser;
    if(TypeString == "PrivateScriptUserEnterPass")
        Type = PrivateScriptUserEnterPass;
}


QString Compiler::GetTypeString()
{
    if(Type == NoProtection)
        return "NoProtection";
    if(Type == PrivateScriptEnterPassForUser)
        return "PrivateScriptEnterPassForUser";
    if(Type == PrivateScriptUserEnterPass)
        return "PrivateScriptUserEnterPass";
    return QString();
}

QString Compiler::GetName()
{
    return Name;
}
int Compiler::GetProtectionStrength()
{
    return ProtectionStrength;
}


QString Compiler::GetIcon()
{
    if(Icon.isEmpty())
    {
        QFile f(":/studio/images/LogoMedium.png");
        if(f.open(QFile::ReadOnly))
        {
            Icon = QString::fromUtf8(f.readAll().toBase64());
        }
        f.close();
    }
    return Icon;
}
QString Compiler::GetVersion()
{
    return Version;
}
void Compiler::SetOs(const QString & Os)
{
    this->Os = Os;
}

QString Compiler::GetOs()
{
    return Os;
}

void Compiler::SetReleaseFolder(const QString& ReleaseFolder)
{
    this->ReleaseFolder = ReleaseFolder;
}

QString Compiler::GetEngineFolder()
{
    return EngineFolder;
}

QString Compiler::GetCompiledFolder()
{
    return QDir(CompiledFolder).absolutePath();
}
void Compiler::SetBuildFolder(const QString& BuildFolder)
{
    this->BuildFolder = BuildFolder;
}

void Compiler::CalculateCompiledFolder()
{
    QDir FolderRelease(ReleaseFolder);
    QString nameCopy = Name;
    QString NameEscapedOriginal = nameCopy.replace(QRegExp("\\s"),".").replace(QRegExp("[^a-zA-Z0-9]"),"");

    QStringList Exists = FolderRelease.entryList(QStringList()<<"*",QDir::Dirs | QDir::NoDotAndDotDot);

    if(NameEscapedOriginal.isEmpty())
        NameEscapedOriginal = "OnlyEnglishNames";
    int iterator = 0;

    CompiledFolder = NameEscapedOriginal;
    while(Exists.contains(CompiledFolder))
    {
        CompiledFolder = NameEscapedOriginal + "." + QString::number(++iterator);
    }
    CompiledFolder = FolderRelease.absoluteFilePath(CompiledFolder);
    /*QDir FolderCompiled(CompiledFolder);
    EngineFolder = FolderCompiled.absoluteFilePath("engine/engine");*/
    EngineFolder = CompiledFolder;
}


static void recurseCopyAddDir(QDir d, QDir t, const QStringList& e)
{
    QStringList qsl = d.entryList(QStringList()<<"*",QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    foreach (QString file, qsl)
    {
        QFileInfo finfo(d.absoluteFilePath(file));

        if (finfo.isSymLink())
            return;

        if (finfo.isDir())
        {
            t.mkdir(file);
            QDir sd(finfo.filePath());
            recurseCopyAddDir(sd, QDir(t.absoluteFilePath(file)),e);
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
                QFile::copy(finfo.absoluteFilePath(),t.absoluteFilePath(file));
        }
    }
}


bool Compiler::Compile()
{
    QDir FolderRelease(ReleaseFolder);
    FolderRelease.mkpath(".");
    CalculateCompiledFolder();

    QDir FolderCompiled(CompiledFolder);
    FolderCompiled.mkpath(".");
    FolderCompiled.mkpath("data");

    QDir FolderRemoteExecuteScript("RemoteExecuteScript");

    QStringList ExcludeList;
    if(!IsCopySilent)
    {
        ExcludeList.append("RemoteExecuteScriptSilent.exe");
    }
    recurseCopyAddDir(FolderRemoteExecuteScript,FolderCompiled,ExcludeList);

    QString ExeFilePath;
    QDirIterator it(CompiledFolder, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QString filename = it.next();
        if(QFileInfo(filename).completeSuffix() == "exe")
        {
            ExeFilePath = filename;
            break;
        }
    }
    if(!ExeFilePath.isEmpty())
    {
        if(Type != NoProtection)
            UpdateExeIcon(ExeFilePath);
        FolderCompiled.rename(QFileInfo(ExeFilePath).fileName(),Name + QString(".exe"));
    }


    QFile RemoteSettings(CompiledFolder + QString("/data/remote_settings.ini"));
    if(RemoteSettings.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&RemoteSettings);
        if(IsEnginesInAppData)
        {
            stream << "IsEnginesInAppData=true" << endl;
        }
        stream << "KeepVersionNumber=5" << endl;
    }
    RemoteSettings.close();


    if(Type == NoProtection)
    {
        return true;
    }else if(Type == PrivateScriptEnterPassForUser)
    {
        QFile FileProjectXml(CompiledFolder + "/data/project.xml");
        if(FileProjectXml.open(QIODevice::WriteOnly))
        {
            QXmlStreamWriter xmlWriter(&FileProjectXml);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();

            xmlWriter.writeStartElement("Remote");
            xmlWriter.writeAttribute("ScriptName", Name);
            xmlWriter.writeAttribute("Version", "last");
            xmlWriter.writeAttribute("Username", Username);
            xmlWriter.writeAttribute("Password", Password);
            xmlWriter.writeAttribute("Mode", "1");

            xmlWriter.writeEndElement();
        }

        FileProjectXml.close();
        return false;
    }else if(Type == PrivateScriptUserEnterPass)
    {
        QFile FileProjectXml(CompiledFolder + "/data/project.xml");
        if(FileProjectXml.open(QIODevice::WriteOnly))
        {
            QXmlStreamWriter xmlWriter(&FileProjectXml);
            xmlWriter.setAutoFormatting(true);
            xmlWriter.writeStartDocument();

            xmlWriter.writeStartElement("Remote");
            xmlWriter.writeAttribute("ScriptName", Name);
            xmlWriter.writeAttribute("Version", "last");
            xmlWriter.writeAttribute("Mode", "1");

            xmlWriter.writeEndElement();
        }

        FileProjectXml.close();
        return false;
    }
    return false;
}

