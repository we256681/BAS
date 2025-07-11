#ifndef MANUALACTIONS_H
#define MANUALACTIONS_H

#include <QObject>
#include <QMap>
#include "stream.h"
#include "apiscript.h"

struct ManualAction
{
    QString ActionId;
    QString ScriptId;
    QString Type;
    QString Data;
};

class ManualActions : public QObject
{
    Q_OBJECT
    QMap<QString, ManualAction> Data;
    Stream *_Stream;
    ApiScript *_ApiScript;
public:
    explicit ManualActions(QObject *parent = nullptr);
    void SetStream(Stream * _Stream);
    void SetApiScript(ApiScript * _ApiScript);
    void Add(
            const QString& ActionId,
            const QString& ScriptId,
            const QString& Type,
            const QString& Data
         );


    void ScriptFinished(const QString& ScriptId);
    QString Serialize();
public slots:
    void Answer(const QString& ActionId,const QString& Result, bool IsSuccess = true);
    void BrowserStop(const QString& ActionId);

signals:

public slots:
};

#endif // MANUALACTIONS_H
