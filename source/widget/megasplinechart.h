#ifndef MEGASPLINECHART_H
#define MEGASPLINECHART_H

#include <QTimer>
#include <QtCharts/QChart>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    virtual ~Chart();

    void beginChangeData();
    void dataAppend(double v1, double v2);
    void endChangeData();

    void snapDataSet( QList<QPointF> **m_pData1,
                      QList<QPointF> **m_pData2 );

    QSplineSeries *series1() const;
    QSplineSeries *series2() const;

    QValueAxis *axis() const;

private:
    QSplineSeries *m_series1;
    QSplineSeries *m_series2;
    QValueAxis *m_axis;

    double m_x;
    double m_y1;
    double m_y2;
    double m_step;

    const int _capacity = 2048;
    QList<QPointF> mData1, mData2;
};


#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTranslator>
#include <QtCharts/QChartView>
#include <QApplication>

class MegaSplineChart : public QWidget
{
    Q_OBJECT
public:
    explicit MegaSplineChart(const QString &tittle = "", QWidget *parent = nullptr);
    MegaSplineChart(QWidget *parent = nullptr);

    ~MegaSplineChart();

    Chart* chart();
    QChartView* chartView();

    void dataAppend(double v1, double v2);

public:
    void changeLanguage(QString qmFile);

private:
    QString m_tittle;

    QChartView *m_chartView;
    QGridLayout *m_layout;
    Chart *m_chart;

    QTranslator m_translator;
};

#endif // MEGASPLINECHART_H
