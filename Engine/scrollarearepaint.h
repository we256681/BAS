#ifndef SCROLLAREAREPAINT_H
#define SCROLLAREAREPAINT_H

#include <QObject>
#include <QScrollArea>

class ScrollAreaRepaint : public QObject
{
    Q_OBJECT
    QScrollArea* ScrollArea = nullptr;
public:
    explicit ScrollAreaRepaint(QScrollArea *parent = nullptr);

private slots:
    void OnScroll();

signals:

};

#endif // SCROLLAREAREPAINT_H
