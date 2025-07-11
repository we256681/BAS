#include "classcomboboxfillpictures.h"
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include "every_cpp.h"


ClassComboBoxFillPictures::ClassComboBoxFillPictures()
{


}

QIcon ClassComboBoxFillPictures::GetIconFor(int number)
{
    int w = 22 * 3,h = 16 * 3;

    QPen Violet((QColor(142,45,197)),1);
    QRect rect(0,0,w,h);
    QFont f;
    f.setPixelSize(10 * 3);

    switch(number)
    {
    case 4:
    {

        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));

        QPainter painter(&pixmap);

        painter.setRenderHint(QPainter::HighQualityAntialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);

        painter.setBrush(QBrush(QColor(142,45,197)));


        painter.setPen(Violet);
        painter.drawRect(9*3,h/4,w - 4 * 3 - 9 * 3,2);
        painter.drawEllipse(4*3,h/4,4,4);

        painter.drawRect(9*3,h/2,w - 4 * 3 - 9 * 3,2);
        painter.drawEllipse(4*3,h/2,4,4);

        painter.drawRect(9*3,3*h/4,w - 4 * 3 - 9 * 3,2);
        painter.drawEllipse(4*3,3*h/4,4,4);

        //painter.drawRect(4*3,h/4,5,2);
        /*painter.fillRect(4*3,h/4,5,2,QColor(142,45,197));
        painter.fill(4*3,h/4,5,2,QColor(142,45,197));*/

        //painter.drawLine(4*3,h/4,4,h/4);
        //painter.drawLine(4,h/4,4,h/4);
        /*painter.drawLine(9,h/2,w-4,h/2);
        painter.drawLine(4,h/2,4,h/2);
        painter.drawLine(9,3*h/4,w-4,3*h/4);
        painter.drawLine(4,3*h/4,4,3*h/4);*/

        QIcon icon(pixmap);


        return icon;
    }
    case 0:
    {
        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));
        QPainter painter(&pixmap);
        painter.setFont(f);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);


        painter.setPen(Violet);
        painter.drawText(rect,Qt::AlignCenter,"abc");
        QIcon icon(pixmap);

        return icon;
    }


    case 1:
    {
        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));

        QPainter painter(&pixmap);

        painter.setFont(f);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);


        painter.setPen(Violet);
        painter.drawText(rect,Qt::AlignCenter,"123");
        QIcon icon(pixmap);

        return icon;
    }
    case 2:
    {
        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));

        QPainter painter(&pixmap);

        painter.setFont(f);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);


        painter.setPen(Violet);
        painter.drawText(rect,Qt::AlignCenter,"a-Z");
        QIcon icon(pixmap);

        return icon;
    }
    case 3:
    {
        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));

        QPainter painter(&pixmap);

        painter.setFont(f);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);


        painter.setPen(Violet);
        painter.drawText(rect,Qt::AlignCenter,"0-9");
        QIcon icon(pixmap);

        return icon;
    }
    case 5:
    {
        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));

        QPainter painter(&pixmap);

        painter.setFont(f);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);


        painter.setPen(Violet);
        painter.drawText(rect,Qt::AlignCenter,".txt");
        QIcon icon(pixmap);

        return icon;
    }

    case 6:
    {
        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));

        QPainter painter(&pixmap);
        f.setPixelSize(11 * 3);
        painter.setFont(f);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);


        painter.setPen(Violet);
        painter.drawText(rect,Qt::AlignCenter,"@");
        QIcon icon(pixmap);

        return icon;
    }

    case 7:
    {
        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));

        QPainter painter(&pixmap);
        f.setPixelSize(11 * 3);
        painter.setFont(f);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);


        painter.setPen(Violet);
        painter.drawText(rect,Qt::AlignCenter,"c:/");
        QIcon icon(pixmap);

        return icon;
    }
    case 8:
    {
        QPixmap pixmap(w,h);
        pixmap.fill(QColor(0,0,0,0));

        QPainter painter(&pixmap);

        painter.setRenderHint(QPainter::HighQualityAntialiasing);

        painter.setPen(QColor(0,0,0,0));
        painter.setBrush(QBrush(QColor(235,235,235)));
        painter.drawRoundedRect(rect,6,6);

        QSvgRenderer svg(QString(":/engine/images/database.svg"));
        svg.render(&painter,QRectF(4 * 3,1 * 3,14 * 3,14 * 3));

        QIcon icon(pixmap);

        return icon;
    }
    }
    return QIcon();
}

void ClassComboBoxFillPictures::AddIconsToCombobox(QComboBox *Combox)
{
    int w = 22,h = 16;

    Combox->setIconSize(QSize(w,h));
    for(int i = 0;i<9;i++)
        Combox->setItemIcon(i,GetIconFor(i));


}
