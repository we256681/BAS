#ifndef DESIGNCHOOSERRESOURCEWIDGET_H
#define DESIGNCHOOSERRESOURCEWIDGET_H
#include "engine_global.h"
#include "dragsection.h"
#include <QWidget>
#include "idatabasestate.h"
namespace Ui {
class DesignChooserResourceWidget;
}
using namespace BrowserAutomationStudioFramework;
class ENGINESHARED_EXPORT DesignChooserResourceWidget : public QWidget
{
    Q_OBJECT
    DragSection *DatabaseDrag;
    IDatabaseState *DatabaseState;
    QList<DatabaseColumn> ColumnsData;
    int OriginalDatabaseTableId;
    QList<int> OriginalDatabaseColumnsIds;
public:
    explicit DesignChooserResourceWidget(QWidget *parent = 0);
    ~DesignChooserResourceWidget();
    QWidget* GetTemplateWidgetByIndex(int index);
    void SetDatabaseState(IDatabaseState *DatabaseState);
    QList<int> GetDatabaseColumnsIds();
    QList<int> GetOriginalDatabaseColumnsIds();
    void SetDatabaseColumnsIds(const QList<int>& ColumnIds);
    int GetOriginalDatabaseTableId();
    void SetOriginalDatabaseTableId(int OriginalDatabaseTableId);

private:
    Ui::DesignChooserResourceWidget *ui;
    void changeEvent(QEvent *e);
public slots:
    void setCurrentIndex(int index);

private slots:
    void GroupsChanged();
    void on_TableCombobox_currentIndexChanged(int index);
    void TableIdChanged();
signals:
    void TableIdChanged(int TableId);
};

#endif // DESIGNCHOOSERRESOURCEWIDGET_H
