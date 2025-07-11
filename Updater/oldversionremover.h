#ifndef OLDVERSIONREMOVER_H
#define OLDVERSIONREMOVER_H

#include <QObject>

class OldVersionRemover : public QObject
{
    Q_OBJECT

public:
    explicit OldVersionRemover(QObject *parent = nullptr);
    void Remove(const QString& Dir, int NumberOfFoldersToKeep = 5);
signals:

};

#endif // OLDVERSIONREMOVER_H
