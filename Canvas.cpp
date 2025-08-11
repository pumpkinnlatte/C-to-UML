#include "Canvas.h"
#include <iostream>
#include <QMouseEvent>
#include <cmath>

using namespace std;

Canvas::Canvas(QWidget *parent) : QGraphicsView(parent), zoomLevel(1.0), minZoomLevel(0.5), maxZoomLevel(2.0)  {
    setMouseTracking(true);  // Habilitar seguimiento de mouse
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);  // Permite arrastrar la escena

    modo = new QString("Seleccion");

    // Inicializar pixmaps
    zoomICursor = new QPixmap("resources/zoomIn.png");
    zoomOCursor = new QPixmap("resources/zoomOut.png");
    moveCursor = new QPixmap("resources/mover.png");

    // Crear cursores
    cursorIn = new QCursor(*zoomICursor);
    cursorOut = new QCursor(*zoomOCursor);
    cursorMove = new QCursor(*moveCursor);

}


void Canvas::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);  // Llama al comportamiento predeterminado
}


void Canvas::mousePressEvent(QMouseEvent *event) {
    if (modo->toStdString() == "Seleccion"){
        cout << "Se presionó Seleccion\n" << endl;
        QGraphicsView::mousePressEvent(event);

    } else if (modo->toStdString() == "Mover") {
        setMoveCursor();
    } else if (modo->toStdString() == "ZoomIn"){
        ZoomIn();
        cout << "Se presionó ZoomIn\n" << endl;
    } else if (modo->toStdString() == "ZoomOut"){
        ZoomOut();
        cout << "Se presionó ZoomOut\n" << endl;
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {

    QGraphicsView::mouseReleaseEvent(event);

    if (modo->toStdString() == "Mover") {
        setMoveCursor();
    } else if (modo->toStdString() == "ZoomIn"){
        setZoomInCursor();
        cout << "Se presionó ZoomIn\n" << endl;
    } else if (modo->toStdString() == "ZoomOut"){
        setZoomOutCursor();
        cout << "Se presionó ZoomOut\n" << endl;
    }

}


void Canvas::enterEvent(QEnterEvent *event){

    cout << "Modo seleccionado: " << modo->toStdString() << endl;

    if (modo->toStdString() == "Seleccion"){
        setSeleccionCursor();
    } else if (modo->toStdString() == "Mover") {
        // Implementar la lógica de mover
        setMoveCursor();
    } else if (modo->toStdString() == "ZoomIn") {
        setZoomInCursor();
    } else if (modo->toStdString() == "ZoomOut") {
        setZoomOutCursor();
    }
}

void Canvas::leaveEvent(QEvent *event) {

    setCursor(Qt::ArrowCursor);
    cout << "Evento abandonado" << endl;

}

void Canvas::ZoomIn() {
    if (zoomLevel < maxZoomLevel) {
        scale(1.1, 1.1);  // Escalar 10% más grande
        zoomLevel *= 1.1;
    }
}

void Canvas::ZoomOut() {
    if (zoomLevel > minZoomLevel) {
        scale(0.9, 0.9);  // Escalar 10% más pequeño
        zoomLevel *= 0.9;
    }
}
void Canvas::setZoomInCursor() {
    viewport()->setCursor(*cursorIn);
}

void Canvas::setZoomOutCursor() {
    viewport()->setCursor(*cursorOut);
}

void Canvas::setMoveCursor() {
    viewport()->setCursor(*cursorMove);
}

void Canvas::setSeleccionCursor() {
    viewport()->setCursor(Qt::OpenHandCursor);
}



