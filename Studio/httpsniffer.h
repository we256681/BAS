#ifndef HTTPSNIFFER_H
#define HTTPSNIFFER_H

#include <QWidget>
#include <QList>
#include "ihttpclient.h"

namespace Ui {
class HttpSniffer;
}

using namespace BrowserAutomationStudioFramework;

class HttpSniffer : public QWidget
{
    Q_OBJECT
    QList<SnifferReport> Data;
public:
    explicit HttpSniffer(QWidget *parent = 0);
    ~HttpSniffer();

public slots:
    void NewSnifferReport(SnifferReport);
    void Show();
signals:
    void StateChanged(bool);
private slots:
    void on_SaveLogs_clicked();
    void ItemChanged(int row,int column);
    void ItemUpdate();
    void on_Close_clicked();

    void on_ClearLogs_clicked();

    void on_Preview_clicked();

private:
    Ui::HttpSniffer *ui;
};

#endif // HTTPSNIFFER_H
