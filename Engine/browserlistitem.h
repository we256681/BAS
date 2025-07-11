#ifndef BROWSERLISTITEM_H
#define BROWSERLISTITEM_H

#include <QWidget>

namespace Ui {
class BrowserListItem;
}

class BrowserListItem : public QWidget
{
    Q_OBJECT
    bool IsVisible;
    bool IsManual;
    int Number;
public:
    explicit BrowserListItem(QWidget *parent = 0);
    ~BrowserListItem();
    void SetNumber(int Number);
    int GetNumber();
    void SetVisibility(bool IsVisible);
    bool GetIsVisible();
    void StartManualControl(const QString& Message);
    void StopManualControl();
    bool GetIsManual();
signals:
    void Changed(bool);

private:
    void SetVisibilityInternal(bool IsVisible);

private slots:
    virtual void mousePressEvent( QMouseEvent * event );

private:

    Ui::BrowserListItem *ui;
};

#endif // BROWSERLISTITEM_H
