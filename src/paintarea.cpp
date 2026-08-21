#include "paintarea.h"
#include <QMouseEvent>
#include <QPainter>
#include <QRect>

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    setAttribute(Qt::WA_OpaquePaintEvent);

    theImage.fill(qRgb(255, 255, 255));
}

void PaintArea::setImage(const QImage &image)
{
    theImage = image.convertToFormat(QImage::Format_RGB32);
    update();
    updateGeometry();
}

bool PaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    return theImage.save(fileName, fileFormat);
}

void PaintArea::setBrushColor(const QColor &color)
{
    this->color = color;
}

void PaintArea::setBrushWidth(int width)
{
    thickness = width;
}

void PaintArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), theImage);
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!pendingPath.isEmpty()) {
            QPainter painter(&theImage);
            setupPainter(painter);

            const QRectF boundingRect = pendingPath.boundingRect();
            QLinearGradient gradient(boundingRect.topRight(),
                                     boundingRect.bottomLeft());
            gradient.setColorAt(0.0, QColor(color.red(), color.green(),
                                            color.blue(), 63));
            gradient.setColorAt(1.0, QColor(color.red(), color.green(),
                                            color.blue(), 191));
            painter.setBrush(gradient);
            painter.translate(event->pos() - boundingRect.center());
            painter.drawPath(pendingPath);

            pendingPath = QPainterPath();
            #ifndef QT_NO_CURSOR
            unsetCursor();
            #endif
            update();
        }
        else
        {
            QPainter painter(&theImage);
            setupPainter(painter);
            const QRect rect = mousePress(painter,event->pos());
            update(rect);
            lastPos = event->pos();
        }
    }
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && lastPos != QPoint(-1, -1)) {
            QPainter painter(&theImage);
            setupPainter(painter);
            const QRect rect = mouseMove(painter, lastPos, event->pos());
            update(rect);
            lastPos = event->pos();
    }
}


void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && lastPos != QPoint(-1, -1)) {
            QPainter painter(&theImage);
            setupPainter(painter);
            QRect rect = mouseRelease(painter,event->pos());
            update(rect);
            lastPos = QPoint(-1, -1);
    }
}

void PaintArea::setupPainter(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(color, thickness, Qt::SolidLine, Qt::RoundCap,
                   Qt::RoundJoin));
}

QRect PaintArea::mousePress(QPainter &painter,const QPoint &pos)
{
    return mouseMove(painter, pos, pos);
}

QRect PaintArea::mouseMove(QPainter &painter,const QPoint &oldPos, const QPoint &newPos)
{
    painter.save();

    int rad = painter.pen().width() / 2;
    QRect boundingRect = QRect(oldPos, newPos).normalized()
                                              .adjusted(-rad, -rad, +rad, +rad);
    QColor color = painter.pen().color();
    QColor transparentColor(color.red(), color.green(), color.blue(), 0);

        painter.drawLine(oldPos, newPos);

    painter.restore();
    return boundingRect;
}

QRect PaintArea::mouseRelease(QPainter &,const QPoint &)
{
    return QRect(0, 0, 0, 0);
}
