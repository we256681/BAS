#include "webdriverbrowserviewer.h"
#include "ui_webdriverbrowserviewer.h"
#include <QDebug>
#include <QCloseEvent>
#include <QPainter>

WebDriverBrowserViewer::WebDriverBrowserViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WebDriverBrowserViewer)
{
    ui->setupUi(this);
    ScreenshotTimer = new QTimer(this);
    ScreenshotTimer->setSingleShot(true);
    ScreenshotTimer->setInterval(500);
    connect(ScreenshotTimer,SIGNAL(timeout()),this,SIGNAL(NeedScreenshot()));
    ScreenshotTimer->start();
}

void WebDriverBrowserViewer::ViewerData(QImage Image, int CursorX, int CursorY)
{
    this->CursorX = CursorX;
    this->CursorY = CursorY;
    Screenshot = Image;
    int w = Screenshot.width();
    int h = Screenshot.height();
    if(w == 0 && h == 0)
    {
        close();
        return;
    }
    ScreenshotTimer->start();
    this->repaint();
}


void WebDriverBrowserViewer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    int w = Screenshot.width();
    int h = Screenshot.height();

    int W = width();
    int H = height();
    float z1 = 1.0f;
    float z2 = 1.0f;
    if(w > W)
    {
        z1 = (float)W / (float)w;
    }
    if(h > H)
    {
        z2 = (float)H / (float)h;
    }
    if(z2 < z1)
        z1 = z2;
    int width = z1 * w;
    int height = z1 * h;
    int dx = (W - width)/2;
    int dy = (H - height)/2;
    QRect rect(dx,dy,width,height);
    painter.drawImage(rect, Screenshot);
    if(w > 0 && CursorX >= 0 && CursorX < w && CursorY >= 0 && CursorY < h)
    {
        QPixmap c(":/engine/images/cursor.png");
        painter.drawPixmap(dx + CursorX * z1, dy + CursorY * z1, c.width(), c.height(), c);
    }

}


void WebDriverBrowserViewer::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit Closed();
}


WebDriverBrowserViewer::~WebDriverBrowserViewer()
{
    delete ui;
}
