#pragma once

#include <QMainWindow>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QPushButton>
#include <QWidget>
#include <QIcon>
#include <QString>
#include <vector>
#include <QPolygon>
#include <QPainter>
#include <iostream>
#include <QPointF>
#include "Canvas.h"
#include "CppAnalizador.h"
#include "GeneradorPDF.h"
#include "GeneradorUML.h"
#include "Recuadro.h"

using namespace std;

class VentanaPrevisualizacion : public QMainWindow {
    Q_OBJECT

public:
    explicit VentanaPrevisualizacion(QWidget *parent = nullptr);

    void setZoomIn();
    void setZoomOut();
    void setMover();
    void setSeleccion();

    Recuadro * buscarRecuadro(const std::string& nombreClase);

    std::vector<Recuadro*> rectangulos;  // Vector de punteros a Recuadro

    QPainter *painter;

    void createRectangulos(vector<CppAnalizador::ClaseCPlusPlus> *clases);
    vector<QPointF> obtenerPosiciones();
    void vaciarRectagulos();

    signals:
        void closed();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Canvas *canvas;
};
