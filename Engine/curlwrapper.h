#ifndef CURLWRAPPER_H
#define CURLWRAPPER_H

#include <QObject>
#include "QtCUrl.h"
#include "engine_global.h"

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT CurlWrapper : public QObject
    {
        Q_OBJECT

        QString FileName;
        QtCUrl::Options Options;
        QString Filter;
        QString TraceFilter;
        QtCUrl * cUrl;
        bool AlreadyDeleted;
        bool DoTrace;

    public:
        explicit CurlWrapper(QObject *parent = 0);
        void SetFilter(const QString& Filter);
        void SetTraceFilter(const QString& TraceFilter);
        void SetFileName(const QString& FileName);
        void SetDoTrace();
        void SetOptions(const QtCUrl::Options& Options);
        ~CurlWrapper();
    signals:
        void Done(bool Result,const QByteArray& Data, const QByteArray& Log, const QByteArray& Trace);
    public slots:
        void Start();
        void Stop();
    };
}

#endif // CURLWRAPPER_H
