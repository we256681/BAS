#ifndef WEBDRIVERBROWSERVIEWER_H
#define WEBDRIVERBROWSERVIEWER_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class WebDriverBrowserViewer;
}

class WebDriverBrowserViewer : public QWidget
{
    Q_OBJECT

    QTimer * ScreenshotTimer;
    QImage Screenshot;
    int CursorX = 0;
    int CursorY = 0;

public:
    explicit WebDriverBrowserViewer(QWidget *parent = 0);
    ~WebDriverBrowserViewer();
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *e);


signals:
    void NeedScreenshot();
    void Closed();

public slots:
    void ViewerData(QImage Image, int CursorX, int CursorY);

private:
    Ui::WebDriverBrowserViewer *ui;
};

#endif // WEBDRIVERBROWSERVIEWER_H
