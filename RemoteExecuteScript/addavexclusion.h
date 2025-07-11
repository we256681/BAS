#ifndef ADDAVEXCLUSION_H
#define ADDAVEXCLUSION_H

#include <QObject>


class AddAVExclusion : public QObject
{
    Q_OBJECT
    QString DirPath;
public:
    explicit AddAVExclusion(QObject *parent = 0);
    void SetDirPath(const QString& DirPath);
    void Run();
signals:
    void Done();
};


#endif // ADDAVEXCLUSION_H
