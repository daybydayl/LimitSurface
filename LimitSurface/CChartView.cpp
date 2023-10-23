#include "CChartView.h"

CChartView::CChartView(QChart* chart, QWidget* parent):QChartView(chart, parent)
{
    // 创建QGraphicsSimpleTextItem来显示坐标
    m_coordX = new QGraphicsSimpleTextItem(chart);
    m_coordY = new QGraphicsSimpleTextItem(chart);

    // 启用橡皮筋选择
    setRubberBand(QChartView::RectangleRubberBand);
}

CChartView::~CChartView()
{

}

void CChartView::mouseMoveEvent(QMouseEvent* event)
{
    QPointF point = chart()->mapToValue(event->pos());
    m_coordX->setText(QString("X: %1").arg(point.x()));
    m_coordY->setText(QString("Y: %1").arg(point.y()));

    // 设置坐标显示的位置
    m_coordX->setPos(event->pos() + QPoint(25, 0));
    m_coordY->setPos(event->pos() + QPoint(25, 25));  // 将Y坐标显示在X坐标下方
}

void CChartView::wheelEvent(QWheelEvent* event)
{
    // 使用滚轮放大和缩小
    if (event->delta() > 0) {
        chart()->zoomIn();
    }
    else {
        chart()->zoomOut();
    }
}

void CChartView::mouseReleaseEvent(QMouseEvent* event)
{
    // 在选定区域显示数据
    if (rubberBand() != QChartView::NoRubberBand) {
        QRectF rect = rubberBandRect();
        //qDebug() << "Selected area:" << rect;

        // 在这里添加你的代码来处理选定区域的数据

        setRubberBand(QChartView::NoRubberBand);
    }

    QChartView::mouseReleaseEvent(event);
}

