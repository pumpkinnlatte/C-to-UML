#include <QApplication>
#include <VentanaPrincipal.h>

int main(int argc, char* argv[]){

	QApplication app(argc, argv);

	VentanaPrincipal window;

	window.setWindowTitle("Generador de Diagramas");
	window.resize(500, 250);
	window.show();

	return app.exec();

}
