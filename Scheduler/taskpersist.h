#ifndef TASKPERSIST_H
#define TASKPERSIST_H

#include <QObject>
#include <QMap>
#include "stream.h"
#include "task.h"
#include "manualactions.h"

class TaskPersist : public QObject
{
    Q_OBJECT
    QMap<QString,Task> Tasks;
    Stream* _Stream;
    ManualActions* _ManualActions;
public:
    explicit TaskPersist(QObject *parent = nullptr);
    void SetStream(Stream* _Stream);
    void SetManualActions(ManualActions *_ManualActions);

    void SaveAll();
    void LoadAll();

    Task& Find(const QString& Id);
    QList<QString> FindAllIds();
    bool Contains(const QString& Id);
    void Insert(Task T);
    void Remove(const QString& Id);
    //void Update(const QString& Id, Task T);
    void ClearAll();
    void ClearArchived();
signals:

public slots:
};

#endif // TASKPERSIST_H
