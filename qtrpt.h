#ifndef QTRPT_H
#define QTRPT_H

#include <QObject>
#include <QWidget>
#include <QtGui>
#include <QDomDocument>
#include <QPrinter>
#include <QPrintPreviewWidget>
#include <QAction>


enum BandType {
    ReportTitle,
    PageHeader,
    MasterData,
    PageFooter,
    ReportSummary
};

class QtRPT : public QWidget
{
    Q_OBJECT
public:
    int recordCount;
    explicit QtRPT(QWidget *parent = 0);
    bool loadReport(QString fileName);
	void printExec(QPrinter * printer);
    //void setCallbackFunc(void (*func)(int &recNo, QString &paramName, QVariant &paramValue));

private:
    int m_recNo;
    float koefRes_h;
    float koefRes_w;
    int ph;
    int pw;
    int ml;
    int mr;
    int mt;
    int mb;
    QPrintPreviewWidget *pr;
    QList<QAction *> lst ;
    QDomNode reportTitle,pageHeader,masterData,pageFooter,reportSummary;
    QDomDocument *xmlDoc;
    QDomNode getBand(BandType type);
    void drawField(QDomNode n, int bandTop, QPainter *painter);
    QString sectionField(QString value);
    QString sectionValue(QString paramName);
    void newPage(QPrinter *printer, QPainter *painter, int &y);
    void proccessPHeader(QPainter *painter, int &y);
    void proccessPFooter(QPainter *painter, int &y);
    void proccessRSummary(QPrinter *printer, QPainter *painter, int &y);
    QColor colorFromString(QString value);
    //void (*callbackFunc)(int &recNo, QString &paramName, QVariant &paramValue);

protected:
    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void setValue(int &recNo, QString &paramName, QVariant &paramValue);
public slots:
    void printPreview(QPrinter *printer);
};

#endif // QTRPT_H
