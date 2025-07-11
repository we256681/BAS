#include "webinterfaceresourcecontroller.h"
#include "resourcemodeldatabase.h"
#include "resourcemodeldirectory.h"
#include "resourcemodelfile.h"
#include "resourcemodelfixedinteger.h"
#include "resourcemodelfixedstring.h"
#include "resourcemodelrandominteger.h"
#include "resourcemodelrandomstring.h"
#include "resourcemodelselect.h"
#include "resourcemodelurl.h"
#include "iresourcemodel.h"
#include "iresources.h"
#include "every_cpp.h"

WebInterfaceResourceController::WebInterfaceResourceController(QObject *parent) :
    IResourceController(parent)
{

}

QVariantMap WebInterfaceResourceController::GetData()
{
    return Data;
}
void WebInterfaceResourceController::SetData(const QVariantMap& Data)
{
    this->Data = Data;
}

void WebInterfaceResourceController::FromViewToModel(IResources * resources, bool Clear)
{
    QList<IResourceModel * > * List = resources->GetModelList();
    for(int i = 0;i<List->length();i++)
    {
        IResourceModel * Model = List->at(i);
        QString Name = Model->GetName();
        if(Data.contains(Name))
        {
            QString Value = Data[Name].toString();
            QString Type = Model->GetTypeId();
            if(Type == "FixedString")
            {
                qobject_cast<ResourceModelFixedString*>(Model)->SetValue(Value);
            }
            else if(Type == "FixedInteger")
            {
                qobject_cast<ResourceModelFixedInteger*>(Model)->SetValue(Value.toInt());
            }else if(Type == "Database")
            {
                qobject_cast<ResourceModelDatabase*>(Model)->SetGroupList(QStringList()<<Value);
            }else if(Type == "FilesFromDirectory")
            {
                qobject_cast<ResourceModelDirectory*>(Model)->SetDirectory(Value);
            }else if(Type == "LinesFromFile")
            {
                qobject_cast<ResourceModelFile*>(Model)->SetFilename(Value);
            }else if(Type == "LinesFromUrl")
            {
                qobject_cast<ResourceModelUrl*>(Model)->SetUrl(Value);
            }else if(Type == "RandomInteger")
            {
                ResourceModelRandomInteger* ModelConverted = qobject_cast<ResourceModelRandomInteger*>(Model);
                QStringList Split = Value.split(",");

                ModelConverted->SetMinUserValue(Split.first().toInt());
                ModelConverted->SetMaxUserValue(Split.last().toInt());
            }else if(Type == "RandomString")
            {
                qobject_cast<ResourceModelRandomString*>(Model)->SetValue(Value);
            }else if(Type == "Select")
            {
                ResourceModelSelect* ModelConverted = qobject_cast<ResourceModelSelect*>(Model);
                QList<int> Selected;
                QStringList SelectedValues = Value.split(",");
                QStringList Values = ModelConverted->GetValues();
                for(QString& SelectedValue:SelectedValues)
                {
                    int index = Values.indexOf(SelectedValue);
                    if(index>=0)
                        Selected.append(index);
                }

                ModelConverted->SetSelected(Selected);
            }
        }
    }
}

void WebInterfaceResourceController::FromModelToView(IResources * resources, bool Clear)
{
    if(Clear)
        Data.clear();
    QList<IResourceModel * > * List = resources->GetModelList();
    for(int i = 0;i<List->length();i++)
    {
        IResourceModel * Model = List->at(i);

        QString Value;
        QString Type = Model->GetTypeId();
        if(Type == "FixedString")
        {
            Value = qobject_cast<ResourceModelFixedString*>(Model)->GetValue();
        }
        else if(Type == "FixedInteger")
        {
            Value = QString::number(qobject_cast<ResourceModelFixedInteger*>(Model)->GetValue());
        }else if(Type == "Database")
        {
            QStringList list = qobject_cast<ResourceModelDatabase*>(Model)->GetGroupList();
            if(list.length() > 0)
                Value = list.first();
        }else if(Type == "FilesFromDirectory")
        {
            Value = qobject_cast<ResourceModelDirectory*>(Model)->GetDirectory();
        }else if(Type == "LinesFromFile")
        {
            Value = qobject_cast<ResourceModelFile*>(Model)->GetFilename();
        }else if(Type == "LinesFromUrl")
        {
            Value = qobject_cast<ResourceModelUrl*>(Model)->GetUrl();
        }else if(Type == "RandomInteger")
        {
            ResourceModelRandomInteger* ModelConverted = qobject_cast<ResourceModelRandomInteger*>(Model);
            Value = QString::number(ModelConverted->GetMinUserValue()) + "," + QString::number(ModelConverted->GetMaxUserValue());
        }else if(Type == "RandomString")
        {
            Value = qobject_cast<ResourceModelRandomString*>(Model)->GetValue();
        }else if(Type == "Select")
        {
            QStringList Values = qobject_cast<ResourceModelSelect*>(Model)->GetValues();
            QList<int> Selected = qobject_cast<ResourceModelSelect*>(Model)->GetSelected();
            QStringList SelectedValues;
            for(int i = 0;i<Selected.length();i++)
            {
                int index = Selected[i];
                if(index>=0 && index<Values.length())
                {
                    SelectedValues.append(Values.at(index));
                }
            }
            Value = SelectedValues.join(",");
        }
        Data[Model->GetName()] = Value;
    }
}
