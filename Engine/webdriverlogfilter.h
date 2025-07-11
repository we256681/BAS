#ifndef WebDriverLogFilter_H
#define WebDriverLogFilter_H

#include <QDialog>
#include "ilogger.h"
#include "engine_global.h"

namespace Ui {
class WebDriverLogFilter;
}

namespace BrowserAutomationStudioFramework
{
    class ENGINESHARED_EXPORT WebDriverLogFilter : public QDialog
    {
        Q_OBJECT
        QList<LogLevel> AvailableLevels;
        void GenerateFilterChangedEvents();
        int PrevThreadNumber;
        bool IsActive;
    public:
        explicit WebDriverLogFilter(QWidget *parent = 0);
        ~WebDriverLogFilter();
    public slots:
        void SetThreadsNumber(int Number);
        void Clear();
        void SetAvailableLevels(QList<LogLevel> AvailableLevels);
    signals:
        void FilterChanged(QList<LogLevel> Level, int ThreadNumber, const QString& Filter, bool IsActive);

    private slots:
        void on_ClearLog_clicked();

        void ClearInternal();

        void on_EnableFilter_clicked();

        void on_DisableFilter_clicked();

        void on_ClearFilter_clicked();

    private:
        Ui::WebDriverLogFilter *ui;
    };
}

#endif // WebDriverLogFilter_H
