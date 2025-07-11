#include "scrollarearepaint.h"
#include "devicescalemanager.h"
#include <QScrollBar>

ScrollAreaRepaint::ScrollAreaRepaint(QScrollArea *parent) : QObject(parent)
{
    ScrollArea = parent;

    BrowserAutomationStudioFramework::DeviceScaleManager ScaleManager;

    if(ScaleManager.GetScaleFactor() >= 1.0001)
    {
        connect(ScrollArea->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(OnScroll()));
        connect(ScrollArea->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(OnScroll()));
    }

}

void ScrollAreaRepaint::OnScroll()
{
    ScrollArea->widget()->repaint();
}
