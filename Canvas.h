#pragma once

#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QTransform>
#include <QCursor>
#include <QGraphicsView>
#include <iostream>
#include <QPixmap>

using namespace std;

class Canvas : public QGraphicsView {
    Q_OBJECT
public:
    Canvas(QWidget *parent = nullptr);
    QString *modo = nullptr;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;


    void ZoomIn();
    void ZoomOut();

    void setZoomOutCursor();
    void setZoomInCursor();
    void setMoveCursor();
    void setSeleccionCursor();

    QPixmap *zoomICursor;
    QPixmap *zoomOCursor;
    QPixmap *moveCursor;


private:
    QPoint lastMousePos;
    QPoint panStartPos;


    QCursor *cursorOut;
    QCursor *cursorIn;
    QCursor *cursorMove;


    double zoomLevel;
    double minZoomLevel;
    double maxZoomLevel;

};