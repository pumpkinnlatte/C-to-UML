#ifndef ITI_271221_EQ_06_U3_RECUADRO_H
#define ITI_271221_EQ_06_U3_RECUADRO_H

#include <QGraphicsPolygonItem>
#include <QPainter>
#include <QString>
#include <iostream>

using namespace std;

class Recuadro : public QGraphicsPolygonItem {
public:
    Recuadro(const QPolygon& polygon, const QString& nombreClase, QGraphicsItem* parent = nullptr)
            : QGraphicsPolygonItem(polygon, parent), nombreClase(nombreClase) {
        setFlag(QGraphicsItem::ItemIsMovable);
        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    }

    void setNombreClase(const QString& nombre) {
        nombreClase = nombre;
        update();
    }

    void setPadre(Recuadro *p) {
        padre = p;

        cout << "Se llega hasta setear el padre" << endl;

        if (padre != nullptr) {
            // Obtener las posiciones de los dos Recuadros
            QPointF pos1 = this->scenePos();
            QPointF pos2 = padre->scenePos();

            linea = new QGraphicsLineItem(QLineF(pos1, pos2));
            cout << "Linea creada\n" << endl;

        }
    }

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
        if (change == QGraphicsItem::ItemPositionChange) {
            // Actualizar la línea cuando el Recuadro se mueva
            if (padre != nullptr && linea != nullptr) {
                linea->setLine(QLineF(scenePos(), padre->scenePos()));
            }
            // Lógica adicional para manejar el cambio de posición
        }
        return QGraphicsPolygonItem::itemChange(change, value);
    }


    QString getNombreClase() {
        return nombreClase;
    }

    QGraphicsLineItem* getLinea(){
        return linea;
    }

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override {
        // Dibuja el polígono
        painter->setPen(Qt::black);
        painter->setBrush(Qt::yellow);  // Color de fondo
        painter->drawPolygon(polygon());

        // Dibuja el texto en el centro
        painter->setPen(Qt::black);
        painter->drawText(boundingRect(), Qt::AlignCenter, nombreClase);

    }

private:
    QString nombreClase;
    Recuadro *padre = nullptr;
    QGraphicsLineItem* linea;

};

#endif //ITI_271221_EQ_06_U3_RECUADRO_H
