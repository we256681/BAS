#include "plaincodeeditor.h"
#include "every_cpp.h"

namespace BrowserAutomationStudioFramework
{
    PlainCodeEditor::PlainCodeEditor(QObject *parent) :
        ICodeEditor(parent)

    {
        Edit = new QPlainTextEdit();
        IsVisible = false;


    }

    void PlainCodeEditor::Show()
    {
        IsVisible = true;
        Edit->setPlainText(Data);
        Data.clear();
        connect(Edit,SIGNAL(textChanged()),this,SIGNAL(Changed()));
    }

    QWidget* PlainCodeEditor::GetWidget()
    {
        return Edit;
    }

    void PlainCodeEditor::SetText(const QString& Text)
    {
        if(!IsVisible)
        {
            Data = Text;
            emit Changed();
        }else
        {
            Edit->setPlainText(Text);
        }

    }

    QString PlainCodeEditor::GetText()
    {
        if(!IsVisible)
        {
            return Data;
        }else
        {
            return Edit->toPlainText();
        }

    }

    void PlainCodeEditor::Clear()
    {
        Data.clear();
        Edit->clear();
    }
}
