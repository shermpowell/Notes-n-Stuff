#ifndef PaintArea_H
#define PaintArea_H

#include <QString>
#include <QPainter>
#include <QMouseEvent>
#include <QColor>
#include <QImage>
#include <QPainterPath>
#include <QWidget>
#include <QRect>
class PaintArea : public QWidget
{
    class BrushInterface;
    Q_OBJECT

    public:
        PaintArea(QWidget *parent = nullptr);

        bool saveImage(const QString &fileName, const char *fileFormat);
        void setImage(const QImage &image);
        void setBrushColor(const QColor &color);
        void setBrushWidth(int width);
        //void setBrush(BrushInterface *brushInterface, const QString &brush);

        QImage image() const { return theImage; }
        QColor brushColor() const { return color; }
        int brushWidth() const { return thickness; }
        //QSize sizeHint() const override;

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

        QRect mousePress(QPainter &painter,const QPoint &pos);
        QRect mouseMove(QPainter &painter,const QPoint &oldPos, const QPoint &newPos);
        QRect mouseRelease(QPainter &painter,const QPoint &pos);

    private:
        void setupPainter(QPainter &painter);

        QImage theImage = {4000, 2000, QImage::Format_RGB32};
        QColor color= (85, 170, 255, 255);
        int thickness = 3;
        QPoint lastPos = {-1, -1};

        QPainterPath pendingPath;
    };

    #endif
