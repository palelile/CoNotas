#include "qtrpt.h"
#include "qprintpreviewwidget.h"

QtRPT::QtRPT(QWidget *parent) : QWidget(parent) {
    xmlDoc = new QDomDocument("Report");
}

bool QtRPT::loadReport(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!xmlDoc->setContent(&file)) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

QDomNode QtRPT::getBand(BandType type) {
    QString s_type;
    if (type == ReportTitle)    s_type = "ReportTitle";
    if (type == PageHeader)     s_type = "PageHeader";
    if (type == MasterData)     s_type = "MasterData";
    if (type == PageFooter)     s_type = "PageFooter";
    if (type == ReportSummary)  s_type = "ReportSummary";
    QDomElement docElem = xmlDoc->documentElement();  //get root element
    //гуляем во всем элементам родителем которых является корневой
    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if ((!e.isNull()) && (e.tagName() == "ReportBand")) {
            if (e.attribute("type") == s_type)
                return n;
        }
        n = n.nextSibling();
    }
    return n;
}

QColor QtRPT::colorFromString(QString value) {
    QColor color;
    int start; int end;
    start = value.indexOf("(",0,Qt::CaseInsensitive);
    end =   value.indexOf(")",start+1,Qt::CaseInsensitive);
    QString tmp = value.mid(start+1,end-start-1);
    int v = tmp.section(",",0,0).toInt();
    color.setRed(v);
    v = tmp.section(",",1,1).toInt();
    color.setGreen(v);
    v = tmp.section(",",2,2).toInt();
    color.setBlue(v);
    v = tmp.section(",",3,3).toInt();
    color.setAlpha(v);
    return color;
}

void QtRPT::drawField(QDomNode n, int bandTop, QPainter *painter) {
    //В качестве параметра подается нод бэнда
    QDomNode c = n.firstChild();
    while(!c.isNull()) {
        QDomElement e = c.toElement(); // try to convert the node to an element.
        if ((!e.isNull()) && (e.tagName() == "TContainerField")) {         
            QFont font(e.attribute("fontFamily"),e.attribute("fontSize").toInt());
            font.setBold(e.attribute("fontBold").toInt());
            font.setItalic(e.attribute("fontItalic").toInt());
            font.setUnderline(e.attribute("fontUnderline").toInt());

            painter->setFont(font);
            painter->setPen(Qt::black);

            int left_ = e.attribute("left").toInt()*koefRes_w;
            int top_ = (bandTop+e.attribute("top").toInt())*koefRes_h;
            int width_ = (e.attribute("width").toInt()-1)*koefRes_w;;
            int height_ = e.attribute("height").toInt()*koefRes_h;

            int cor = QFontMetrics(font).height() * koefRes_h;
            QRect textRect(left_, top_-height_, width_, height_);
            textRect.translate(0, cor );

            QPen pen = painter->pen();

            Qt::Alignment al;
            Qt::Alignment alH, alV;
            if (e.attribute("aligmentH") == "hRight")   alH = Qt::AlignRight;
            if (e.attribute("aligmentH") == "hLeft")    alH = Qt::AlignLeft;
            if (e.attribute("aligmentH") == "hCenter")  alH = Qt::AlignHCenter;
            if (e.attribute("aligmentH") == "hJustify") alH = Qt::AlignJustify;
            if (e.attribute("aligmentV") == "vTop")     alV = Qt::AlignTop;
            if (e.attribute("aligmentV") == "vBottom")  alV = Qt::AlignBottom;
            if (e.attribute("aligmentV") == "vCenter")  alV = Qt::AlignVCenter;
            al = alH | alV;  //colorFromString(e.attribute("borderRight"))
            painter->fillRect(left_+1,top_+1,width_-2,height_-2,colorFromString(e.attribute("backgroundColor")));
            QString txt = sectionField(e.attribute("value"));
            pen.setColor(colorFromString(e.attribute("fontColor")));
            painter->setPen(pen);
            painter->drawText(left_+10,top_,width_-10,height_, al, txt);

            pen.setWidth(1);
            pen.setColor(colorFromString(e.attribute("borderTop")));
            painter->setPen(pen);
            painter->drawLine(left_, top_, left_ + width_, top_);
            pen.setColor(colorFromString(e.attribute("borderBottom")));
            painter->setPen(pen);
            painter->drawLine(left_, top_ + height_, left_ + width_, top_ + height_);
            pen.setColor(colorFromString(e.attribute("borderLeft")));
            painter->setPen(pen);
            painter->drawLine(left_, top_, left_, top_ + height_);
            pen.setColor(colorFromString(e.attribute("borderRight")));
            painter->setPen(pen);
            painter->drawLine(left_ + width_, top_, left_ + width_, top_ + height_);

            if (e.attribute("picture","text") != "text") {
                QByteArray byteArray;
                byteArray = QByteArray::fromBase64(e.attribute("picture","text").toUtf8());
                QPixmap pixmap = QPixmap::fromImage(QImage::fromData(byteArray, "PNG"));
                painter->drawPixmap(left_,top_,width_,height_,pixmap);
            }
        }
        c = c.nextSibling();
    }
}

QString QtRPT::sectionField(QString value) {
    QString tmpStr;
    QStringList res;
    for (int i = 0; i < value.size(); ++i) {
        if (value.at(i) != '[' && value.at(i) != ']')
            tmpStr += value.at(i);
        else {
            if (value.at(i) == ']') {
                tmpStr += value.at(i);
                res << tmpStr;
                tmpStr.clear();
            }
            if (value.at(i) == '[') {
                if (!tmpStr.isEmpty())
                    res << tmpStr;
                tmpStr.clear();
                tmpStr += value.at(i);
            }
        }
    }
    if (!tmpStr.isEmpty()) res << tmpStr;

    tmpStr.clear();
    for (int i = 0; i < res.size(); ++i) {
        if (res.at(i).contains("[") && res.at(i).contains("]"))
        tmpStr += sectionValue(res.at(i));
        else tmpStr += res.at(i);
    }
    return tmpStr;
}

QString QtRPT::sectionValue(QString paramName) {
    QVariant paramValue;
    paramName.replace("[","");
    paramName.replace("]","");
    int recNo = m_recNo;
    //callbackFunc(recNo, paramName, paramValue);
    emit setValue(recNo, paramName, paramValue);
    return paramValue.toString();
}

/*void QtRPT::setCallbackFunc( void (*func)(int &recNo, QString &paramName, QVariant &paramValue) ) {
    callbackFunc=func;
}*/

void QtRPT::printExec(QPrinter *printer) {
#ifndef QT_NO_PRINTER
    QDomElement docElem = xmlDoc->documentElement();
    ph = docElem.attribute("pageHeight").toInt();
    pw = docElem.attribute("pageWidth").toInt();
    ml = docElem.attribute("marginsLeft").toInt();
    mr = docElem.attribute("marginsRight").toInt();
    mt = docElem.attribute("marginsTop").toInt();
    mb = docElem.attribute("marginsBottom").toInt();
	printer->setPageMargins(ml/4+0.01, mt/4+0.01, mr/4+0.01, mb/4+0.01, QPrinter::Millimeter);
	printPreview(printer);
#endif
}

void QtRPT::printExec() {
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QDomElement docElem = xmlDoc->documentElement();
    ph = docElem.attribute("pageHeight").toInt();
    pw = docElem.attribute("pageWidth").toInt();
    ml = docElem.attribute("marginsLeft").toInt();
    mr = docElem.attribute("marginsRight").toInt();
    mt = docElem.attribute("marginsTop").toInt();
    mb = docElem.attribute("marginsBottom").toInt();
    printer.setPageMargins(ml/4+0.01, mt/4+0.01, mr/4+0.01, mb/4+0.01, QPrinter::Millimeter);
    QPrintPreviewDialog preview(&printer, this, Qt::Window);
    preview.setWindowState(Qt::WindowMaximized);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    pr = preview.findChild<QPrintPreviewWidget *>();
    lst = preview.findChildren<QAction *>();
    pr->installEventFilter(this);
    preview.exec();
#endif
}

void QtRPT::printPreview(QPrinter *printer) {
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    QPainter painter;
    painter.begin(printer);
    QRect r = printer->pageRect();
    //painter.drawRect(0,0,r.width(),r.height());   Рамка вокруг страницы

    int y = 0;
//    int yS = 0;
    int yF = 0;

    //QDomElement docElem = xmlDoc->documentElement();  //get root element

    koefRes_h = static_cast<double>(r.height()) / (ph - mt - mb);
    koefRes_w = static_cast<double>(r.width())  / (pw - ml - mr);
    reportTitle = getBand(ReportTitle);
    pageHeader = getBand(PageHeader);
    pageFooter = getBand(PageFooter);
    reportSummary = getBand(ReportSummary);

    if (!reportSummary.isNull()) {
        QDomElement e = reportSummary.toElement();
//        yS = e.attribute("height").toInt();
    }
    if (!pageFooter.isNull()) {
        QDomElement e = pageFooter.toElement();
        yF = e.attribute("height").toInt();
    }

    if (!reportTitle.isNull()) {
        QDomElement e = reportTitle.toElement();
        drawField(reportTitle,y,&painter);
        y += e.attribute("height").toInt();
        //painter.drawLine(0,y*koefRes_h,r.width(),y*koefRes_h);
    }
    proccessPHeader(&painter,y);

    if (recordCount > 0) {
        masterData = getBand(MasterData);
        if (!masterData.isNull()) {
            for (int i = 0; i < recordCount; i++) {
                m_recNo = i;
                QDomElement e = masterData.toElement();
                if (y + e.attribute("height").toInt() > ph-mb-mt-yF) {
                    proccessPFooter(&painter,y);
                    newPage(printer, &painter, y);
                }

                drawField(masterData,y,&painter);
                y += e.attribute("height").toInt();
                //painter.drawLine(0,y*koefRes_h,r.width(),y*koefRes_h);
            }
        }
    }

    proccessPFooter(&painter,y);
    proccessRSummary(printer, &painter,y);
    painter.end();
#endif
}

bool QtRPT::eventFilter(QObject *obj, QEvent *e) {
    if (obj == pr && e->type()==QEvent::Show)  {
        for (int i = 0; i < lst.size(); i++) {
            if (lst.at(i)->text().contains("previous", Qt::CaseInsensitive))
                lst.at(i)->trigger();
        }

        pr->setCurrentPage(1);
        return true;
    }
    return QWidget::eventFilter(obj,e);
}

void QtRPT::newPage(QPrinter *printer, QPainter *painter, int &y) {
    printer->newPage();
    y = 0;
    proccessPHeader(painter,y);
}

void QtRPT::proccessRSummary(QPrinter *printer, QPainter *painter, int &y) {
    if (reportSummary.isNull()) return;
    QDomElement e = reportSummary.toElement();
    if (y + e.attribute("height").toInt() > ph-mb-mt-e.attribute("height").toInt())
        newPage(printer, painter, y);
    drawField(reportSummary,y,painter);
    y += e.attribute("height").toInt();
    //painter->drawLine(0,y*koefRes_h,pw*koefRes_h,y*koefRes_h);
}

void QtRPT::proccessPFooter(QPainter *painter, int &y) {
    if (pageFooter.isNull()) return;
    QDomElement e = pageFooter.toElement();
    drawField(pageFooter,y,painter);
    y += e.attribute("height").toInt();
    //painter->drawLine(0,y*koefRes_h,pw*koefRes_h,y*koefRes_h);
}

void QtRPT::proccessPHeader(QPainter *painter, int &y) {
    if (pageHeader.isNull()) return;
    QDomElement e = pageHeader.toElement();
    drawField(pageHeader,y,painter);
    y += e.attribute("height").toInt();
    //painter->drawLine(0,y*koefRes_h,pw*koefRes_h,y*koefRes_h);
}
