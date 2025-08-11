#include "VentanaPrevisualizacion.h"

VentanaPrevisualizacion::VentanaPrevisualizacion(QWidget *parent) : QMainWindow(parent) {

    QDockWidget *opcionesLaterales = new QDockWidget(tr("Herramientas"), this);
    opcionesLaterales->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    opcionesLaterales->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);


    auto *widget = new QWidget();
    auto *contLateral = new QVBoxLayout();

    widget->setLayout(contLateral);

    // Crear botones
    auto *zoomIn = new QPushButton("Zoom In", this);
    zoomIn->setIcon(QIcon("resources/zoomIn.png"));

    auto *zoomOut = new QPushButton("Zoom Out", this);
    zoomOut->setIcon(QIcon("resources/zoomOut.png"));

    //auto *mover = new QPushButton("Mover", this);
    //mover->setIcon(QIcon("resources/mover.png"));

    auto *seleccionar = new QPushButton("Seleccionar", this);
    seleccionar->setIcon(QIcon("resources/cursor.png"));

    // Añadir botones

    contLateral->setDirection(QBoxLayout::BottomToTop);
    contLateral->setSpacing(15);
    contLateral->setContentsMargins(5, 20, 5, 5);
    contLateral->addStretch(0);


    //contLateral->addWidget(mover);
    contLateral->addWidget(zoomIn);
    contLateral->addWidget(zoomOut);
    contLateral->addWidget(seleccionar);

    opcionesLaterales->setEnabled(true);

    // Añadir el widget al QDockWidget
    opcionesLaterales->setWidget(widget);

    // Añadir el QDockWidget a la ventana principal
    addDockWidget(Qt::LeftDockWidgetArea, opcionesLaterales);

    canvas = new Canvas();
    canvas->setScene(new QGraphicsScene(this));  // Asegúrate de inicializar la escena

    setCentralWidget(canvas);

    resize(1000, 800);

    //connect(mover, &QPushButton::clicked, this, &VentanaPrevisualizacion::setMover);
    connect(zoomIn, &QPushButton::clicked, this, &VentanaPrevisualizacion::setZoomIn);
    connect(zoomOut, &QPushButton::clicked, this, &VentanaPrevisualizacion::setZoomOut);
    connect(seleccionar, &QPushButton::clicked, this, &VentanaPrevisualizacion::setSeleccion);

}

void VentanaPrevisualizacion::closeEvent(QCloseEvent *event) {
    emit closed();
    QMainWindow::closeEvent(event);
}

void VentanaPrevisualizacion::setZoomIn() {
    if(canvas->modo != nullptr){
        delete canvas->modo;
    }
    canvas->modo = new QString("ZoomIn");
};

void  VentanaPrevisualizacion::setZoomOut() {
        if(canvas->modo != nullptr){
            delete canvas->modo;
        }
        canvas->modo = new QString("ZoomOut");

};

/*void  VentanaPrevisualizacion::setMover(){
    if(canvas->modo != nullptr){
        delete canvas->modo;
    }
    canvas->modo = new QString("Mover");
};*/

void VentanaPrevisualizacion::setSeleccion(){
    if(canvas->modo != nullptr){
        delete canvas->modo;
    }
    canvas->modo = new QString("Seleccion");
}

void VentanaPrevisualizacion::createRectangulos(vector<CppAnalizador::ClaseCPlusPlus> *clases) {
    vector<QPointF> posiciones = {QPointF(0,0), QPointF(-5,-5), QPointF(5,-5), QPointF(-10,-10), QPointF(10,-10), QPointF(20,-20)};

    int i = 0;
    for (CppAnalizador::ClaseCPlusPlus& clase : *clases) {

        QPolygon polygon;
        polygon << QPoint(10, 20) << QPoint(200, 20) << QPoint(200, 100) << QPoint(10, 100);

        Recuadro* nuevoRect = new Recuadro(polygon, QString::fromStdString(clase.nombre));
        posiciones[i] = posiciones[i] * -25;
        nuevoRect->setPos(posiciones[i]);

        rectangulos.push_back(nuevoRect);

        canvas->scene()->addItem(nuevoRect);

        i++;
    }

    i = 0;
    for (CppAnalizador::ClaseCPlusPlus& clase : *clases) {
        if (!clase.herencia.empty()) {
            Recuadro* padre = buscarRecuadro(clase.herencia);

            if (padre != nullptr) {
                rectangulos.at(i)->setPadre(padre);  // Usar puntero

                //canvas->scene()->addItem(rectangulos.at(i)->getLinea());
            }
        }
        i++;
    }
}



Recuadro* VentanaPrevisualizacion::buscarRecuadro(const std::string& nombreClase) {
    for(Recuadro *recuadro : rectangulos) {
        cout << recuadro->getNombreClase().toStdString() << " == " << nombreClase << endl;
       if(recuadro && recuadro->getNombreClase().toStdString() == nombreClase) {
            cout << "\nse encontro una clase padre: " << nombreClase << "\n" << endl;
            return recuadro;
        }
    }
    return nullptr; // Maneja el caso en el que no se encuentra la clase
}

void VentanaPrevisualizacion::vaciarRectagulos() {
    for(Recuadro *recuadro : rectangulos) {
        delete recuadro;
    }

    rectangulos.clear();
}

vector<QPointF> VentanaPrevisualizacion::obtenerPosiciones(){
    vector<QPointF> posiciones;

    for(Recuadro *recuadro : rectangulos){
        posiciones.push_back(recuadro->pos());
    }

    return posiciones;
}





