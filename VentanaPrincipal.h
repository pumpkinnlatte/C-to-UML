#pragma once

#include <QWidget>
#include <iostream>
#include <fstream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMainWindow>
#include <QMenuBar>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QIcon>
#include <QMessageBox>
#include <VentanaPrevisualizacion.h>
#include "CppAnalizador.h"

using namespace std;

class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

private:
    void createMenus() {
        QMenuBar *bar = menuBar();
        QMenu *fileMenu = bar->addMenu(tr("&Archivo"));

        QAction *abrirA = new QAction("Abrir       Ctrl + O ", this);
        QAction *cerrarA = new QAction("Cerrar     Alt  + F4", this);

        fileMenu->addAction(abrirA);
        fileMenu->addAction(cerrarA);

        connect(abrirA, &QAction::triggered, this, &VentanaPrincipal::abrir);
        connect(cerrarA, &QAction::triggered, this, &VentanaPrincipal::cerrar);
    };

    void createSeleccionador() {
        seleccionador = new QFileDialog(this);
        seleccionador->setDirectory("/home/");

    };

public:
    QFile *archivoCargado = nullptr;
    QDir *dirSalida = nullptr;
    QDir *ultimoDirectorio = nullptr;
    QFileDialog *seleccionador = nullptr;

    QLineEdit *rutaArchivoField = nullptr;
    QLineEdit *rutaSalidaField = nullptr;

    vector<CppAnalizador::ClaseCPlusPlus> *clases = nullptr;  //Aqui guardaremos las clases

    string codigo;

    VentanaPrevisualizacion *previsualizacion;

    VentanaPrincipal(QWidget *parent = nullptr) : QMainWindow(parent) {
        createMenus();

        // Widget
        auto *widgetContenido = new QWidget();

        // Layouts
        auto *layoutVertical = new QVBoxLayout();
        auto *layoutGrid = new QGridLayout();
        layoutGrid->setAlignment(Qt::AlignTop);

        // Contenido layoutGrid
        auto *labelArch = new QLabel("Archivo:", this);
        labelArch->setAlignment(Qt::AlignCenter | Qt::AlignRight);

        auto *labelRutaGuardado = new QLabel("Ruta Guardado:", this);
        labelRutaGuardado->setAlignment(Qt::AlignBottom | Qt::AlignRight);

        auto *abrirBoton = new QPushButton(this);
        abrirBoton->setIcon(QIcon("resources/folder.png"));

        auto *rutaBoton = new QPushButton(this);
        rutaBoton->setIcon(QIcon("resources/folder.png"));

        auto *previsualizarB = new QPushButton("Previsualizar");
        auto *convertirB = new QPushButton("Convertir");

        // Inicializar QLineEdit
        rutaArchivoField = new QLineEdit(this);
        rutaSalidaField = new QLineEdit("diagramas/", this);

        layoutGrid->addWidget(labelArch, 0, 0);      // texto
        layoutGrid->addWidget(rutaArchivoField, 0, 1);  // input
        layoutGrid->addWidget(abrirBoton, 0, 2); // boton

        layoutGrid->addWidget(labelRutaGuardado, 1, 0);      // texto
        layoutGrid->addWidget(rutaSalidaField, 1, 1);  // input
        layoutGrid->addWidget(rutaBoton, 1, 2);  // boton


        layoutGrid->addWidget(previsualizarB, 2, 1);
        layoutGrid->addWidget(convertirB, 2, 2);

        layoutVertical->addLayout(layoutGrid);

        // Conexiones de los eventos
        connect(abrirBoton, &QPushButton::clicked, this, &VentanaPrincipal::abrir);
        connect(rutaBoton, &QPushButton::clicked, this, &VentanaPrincipal::escogerDirectorio);

        connect(previsualizarB, &QPushButton::clicked, this, &VentanaPrincipal::abrirPrevisualizacion);
        connect(convertirB, &QPushButton::clicked, this, &VentanaPrincipal::convertir);

        //VentanaPrevisualizacion;
        previsualizacion = new VentanaPrevisualizacion(this);
        previsualizacion->hide();

        connect(previsualizacion, &VentanaPrevisualizacion::closed, this, &VentanaPrincipal::habilitarVentana);

        // Establecer el layout
        widgetContenido->setLayout(layoutVertical);
        setCentralWidget(widgetContenido);
    }

    void leerArchivo(const std::string& rutaArchivo, std::string& contenido) {
        std::ifstream archivo(rutaArchivo);
        if (archivo.is_open()) {
            std::stringstream buffer;
            buffer << archivo.rdbuf();
            contenido = buffer.str();
        } else {
            throw std::runtime_error("No se pudo abrir el archivo " + rutaArchivo);
        }
    }

    QString mostrarDialogoEntrada(QWidget *parent) {
        bool ok;
        QString nombreArchivo = QInputDialog::getText(parent, "Nombre del Archivo",
                                                      "Ingrese el nombre del archivo:",
                                                      QLineEdit::Normal, "", &ok);

        if (ok && !nombreArchivo.isEmpty()) {
            // Procesar el nombre del archivo
            qDebug() << "Nombre del archivo ingresado:" << nombreArchivo;
            return nombreArchivo;
        } else {
            qDebug() << "No se ingresó un nombre de archivo.";
            return "";
        }
    }

public slots:
    void cerrar() {
        cout << "Cerrar" << endl;
    };

    void abrir() {
        escogerArchivo();

        if (archivoCargado != nullptr) {
            cout << "Si esta cargado" << endl;
            rutaArchivoField->setText(archivoCargado->symLinkTarget());
        }
    }

    void escogerDirectorio() {

        createSeleccionador();

        if (ultimoDirectorio != nullptr) {  // Ingresar en el ultimo directorio seleccionado
            cout << "Ultimo direcotrio: " << ultimoDirectorio->absolutePath().toStdString() << endl;
            seleccionador->setDirectory(ultimoDirectorio->absolutePath());
        }

        seleccionador->setDirectory("diagramas/");
        seleccionador->setFileMode(QFileDialog::Directory);

        if (seleccionador->exec()) {
            QStringList directorio = seleccionador->selectedFiles();

            if (!directorio.isEmpty()) {
                cout << "Directorio Seleccionado: " << directorio[0].toStdString() << endl;

                ultimoDirectorio = new QDir(seleccionador->directory().absolutePath());  // Recordar ultimo Dir
                rutaSalidaField->setText(directorio[0]);

            } else {
                cout << "No se selecciono ningun directorio" << endl;
            }
        }
    }

    void escogerArchivo() {

        if(clases != nullptr){
            if(!clases->empty()) {
                clases->clear();
                previsualizacion->vaciarRectagulos();
            }
        }

        createSeleccionador();

        if (ultimoDirectorio != nullptr) {  // Ingresar en el ultimo directorio seleccionado
            seleccionador->setDirectory(ultimoDirectorio->absolutePath());
            cout << "Ultimo direcotrio: " << ultimoDirectorio->absolutePath().toStdString() << endl;
        }

        seleccionador->setNameFilter(tr("Todo Archivo C++ (*.cpp *.cc *.C *.cxx *.c++)"));
        seleccionador->setFileMode(QFileDialog::ExistingFile);

        if (seleccionador->exec()) {
            QStringList nombres = seleccionador->selectedFiles();

            if (!nombres.isEmpty()) {
                cout << "Archivo seleccionado: " << nombres[0].toStdString() << endl;

                ultimoDirectorio = new QDir(seleccionador->directory().absolutePath());  // Recordar ultimo Dir
                rutaArchivoField->setText(nombres[0]);

            } else {
                cout << "No se selecciono ningun archivo" << endl;
            }
        }
    };

    void abrirPrevisualizacion(){
        cout << "Abriendo modo Previsualización" << endl;

        if(clases == nullptr || clases->empty()){

            if(rutaArchivoField->text().toStdString().empty()){
                cout << "\nDireccion de archivo no especificada\n" << endl;
            } else {
                leerArchivo(rutaArchivoField->text().toStdString(), codigo);

                string prePros = CppAnalizador::preprocesarCodigoCpp(codigo);

                clases = new vector<CppAnalizador::ClaseCPlusPlus>(CppAnalizador::analizarCodigoCpp(prePros));

                previsualizacion->createRectangulos(clases);

            }

        }

        // Deshabilitar la ventana principal
        this->setEnabled(false);

        // Mostrar la nueva ventana
        previsualizacion->setEnabled(true);
        previsualizacion->show();

    }

    void convertir(){
        cout << "Convirtiendo Archivo" << endl;

        if(clases == nullptr || clases->empty()){ //Mostrar advertencia
            QMessageBox mensaje;
            mensaje.setIcon(QMessageBox::Warning);  // Establece el ícono como una advertencia
            mensaje.setWindowTitle("Archivo no seleccionado");
            mensaje.setText("No se ha seleccionado ningún archivo.");
            mensaje.setStandardButtons(QMessageBox::Ok);  // Solo muestra el botón "Aceptar"
            mensaje.setDefaultButton(QMessageBox::Ok);  // Establece "Aceptar" como el botón predeterminado
            mensaje.exec();  // Muestra el diálogo
        } else {

            string rutaGuardado = rutaSalidaField->text().toStdString(); //Obtener ruta guardado

            vector<QPointF> posicionesActuales(previsualizacion->obtenerPosiciones());

            string diagramaUML = GeneradorUML::generarDiagramaUML(*clases, posicionesActuales);

            QString nombre = mostrarDialogoEntrada(this);

            string nombreDiagrama = GeneradorUML::generarTex(diagramaUML, nombre.toStdString());

            GeneradorPDF::generarPDFDesdeArchivoTeX(nombreDiagrama, rutaGuardado); // Generar PDF

            string log = rutaGuardado + nombreDiagrama + ".log";
            string aux = rutaGuardado + nombreDiagrama + ".aux";

            vector<string> aEliminar = {log, aux};

            GeneradorPDF::deleteTempFiles(aEliminar);

            cout << "Diagrama UML generado y exportado como PDF.\n";

        }

    }

    void habilitarVentana(){
        this->setEnabled(true);
    }



};
