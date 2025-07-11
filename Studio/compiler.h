#ifndef COMPILEROPTIONS_H
#define COMPILEROPTIONS_H

#include <QObject>

class Compiler : public QObject
{
    Q_OBJECT
    QString Name;
    int ProtectionStrength = 4;
    QString Version;
    QString Username;
    QString Password;
    QString Icon;
    bool IsCustomIcon;

    QString Os;
    QString EngineFolder;
    QString CompiledFolder;
    QString ReleaseFolder;
    QString BuildFolder;
    bool HideBrowsers;
    bool IntegrateScheduler;
    bool SingleInstance;
    bool IsEnginesInAppData;
    bool IsCopySilent;
    void CalculateCompiledFolder();
public:
    enum CompileType{NoProtection,PrivateScriptEnterPassForUser,PrivateScriptUserEnterPass};
private:
    CompileType Type;
    void UpdateExeIcon(const QString& ExePath);
public:

    void SetName(const QString & Name);
    void SetProtectionStrength(int ProtectionStrength);
    void SetIcon(const QString & Icon);
    void SetIsCustomIcon(bool IsCustomIcon);
    void SetHideBrowsers(bool HideBrowsers);
    void SetIntegrateScheduler(bool IntegrateScheduler);
    void SetSingleInstance(bool SingleInstance);
    void SetCopySilent(bool CopySilent);
    void SetIsEnginesInAppData(bool IsEnginesInAppData);

    void SetOs(const QString & Os);
    void SetVersion(const QString & Version);

    void SetUsername(const QString & Username);
    void SetPassword(const QString & Password);
    void SetType(const CompileType & Type);
    void SetTypeString(const QString& TypeString);

    CompileType GetType();
    QString GetTypeString();

    bool GetHideBrowsers();
    bool GetIntegrateScheduler();
    bool GetSingleInstance();
    bool GetCopySilent();
    bool GetIsEnginesInAppData();
    bool GetIsCustomIcon();

    QString GetName();
    int GetProtectionStrength();
    QString GetIcon();
    QString GetOs();
    QString GetVersion();
    explicit Compiler(QObject *parent = 0);

    //Returns true if need to copy project.xml
    bool Compile();
    QString GetEngineFolder();
    QString GetCompiledFolder();
    void SetReleaseFolder(const QString& ReleaseFolder);
    void SetBuildFolder(const QString& BuildFolder);

    void Reset();
signals:

public slots:

};

#endif // COMPILEROPTIONS_H
