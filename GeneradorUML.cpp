#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
#include "GeneradorUML.h"

 using namespace std;

 std::string GeneradorUML::join(const std::vector<std::string>& vec, const std::string& sep) {
    std::string result;
    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i];
        if (i < vec.size() - 1) {
            result += sep;
        }
    }
    return result;
}


std::string GeneradorUML::generarDiagramaClase(const CppAnalizador::ClaseCPlusPlus clase, int posX, int posY) {
    std::stringstream ss;

    ss << "\\begin{class}[text width=8cm]{" << clase.nombre << "}{" << posX << "," << posY << "}\n";

    if (!clase.herencia.empty()) {
        ss << "\\inherit{" << clase.herencia << "}\n";
    }

    for (const auto& atributo : clase.atributos) {
        ss << "\\attribute{" << atributo.visibilidad << " " << atributo.nombre << " : " << atributo.tipo << "}\n";
    }

    for (const auto& metodo : clase.metodos) {
        ss << "\\operation{" << metodo.visibilidad << " " << metodo.nombre << " (";
        ss << join(metodo.argumentos, ", ");
        ss << ") : " << metodo.tipoRetorno << "}\n";
    }

    ss << "\\end{class}\n";
    return ss.str();
}


std::string GeneradorUML::generarDiagramaUML(const std::vector<CppAnalizador::ClaseCPlusPlus>& clases, std::vector<QPointF> posiciones) {

    QRectF tamanoPapel = calcularRectanguloDelimitador(posiciones);

    //de pixeles pasaremos a pt
    /*
     * 1 pulgada = 96 píxeles = 72 puntos
     * Por lo tanto:
     *  - 1 píxel ≈ 0.75 puntos (pt)
     *  - 1 punto (pt) ≈ 1.33 píxeles
     */
    double anchoPt = ((tamanoPapel.width() * 3) * 0.75);
    double altoPt = ((tamanoPapel.height() * 3) * 0.75);

    cout << "Ancho: " << anchoPt << "" << endl;
    cout << "Alto: " << altoPt << "" << endl;

    std::stringstream ss;
    ss << "\\documentclass{article}\n";
    ss << "\\usepackage[paperwidth=" << std::ceil(anchoPt) << "pt, paperheight=" << std::ceil(altoPt) << "pt, margin=200pt]{geometry}\n";
    ss << "\\usepackage{pgf-umlcd}\n";
    ss << "\\begin{document}\n";
    ss << "\\begin{tikzpicture}\n";

    int clasesPredefinidas = std::min((int)clases.size(), 6);
    int indicePosicion = 0;

    for (int i = 0; i < clasesPredefinidas; i++) {
        const CppAnalizador::ClaseCPlusPlus& clase = clases[i];
        QPointF posicion = posiciones[indicePosicion] / -25; //Recordemos rectificar a escala las posiciones de los rectangulos
        ss << generarDiagramaClase(clase, posicion.x() * -1, posicion.y());
        indicePosicion++;
    }

    int posX = 0;
    int posY = 20;
    for (std::size_t i = clasesPredefinidas; i < clases.size(); i++) {
        const CppAnalizador::ClaseCPlusPlus& clase = clases[i];
        ss << generarDiagramaClase(clase, posX, posY);
        posY += 10;
    }

    ss << "\\end{tikzpicture}\n";
    ss << "\\end{document}\n";

    return ss.str();
}

std::string GeneradorUML::generarTex(const std::string& codigoLaTeX, const std::string& nombreDiagrama) {
    std::string nombreArchivo = nombreDiagrama;
    if (nombreDiagrama.empty()) {
        nombreArchivo = "diagrama_" + std::to_string(std::time(nullptr));
    }

    std::string rutaArchivoTeX = "tex/" + nombreArchivo + ".tex";
    std::ofstream archivoTeX(rutaArchivoTeX);

    if (!archivoTeX.is_open()) {
        std::cerr << "No se pudo crear el archivo: " << rutaArchivoTeX << std::endl;
        return "";
    }

    archivoTeX << codigoLaTeX;
    archivoTeX.close();

    return nombreArchivo;
}

QRectF GeneradorUML::calcularRectanguloDelimitador(const vector<QPointF>& puntos) {
    if (puntos.empty()) {
        return QRectF();  // Devuelve un rectángulo vacío si la lista está vacía
    }

    // Inicializa los valores mínimo y máximo con el primer punto de la lista
    qreal minX = puntos[0].x();
    qreal maxX = puntos[0].x();
    qreal minY = puntos[0].y();
    qreal maxY = puntos[0].y();

    // Itera a través de los puntos para encontrar los extremos
    for (const QPointF& punto : puntos) {
        if (punto.x() < minX) {
            minX = punto.x();
        }
        if (punto.x() > maxX) {
            maxX = punto.x();
        }
        if (punto.y() < minY) {
            minY = punto.y();
        }
        if (punto.y() > maxY) {
            maxY = punto.y();
        }
    }

    // Calcula el tamaño del rectángulo
    qreal ancho = maxX - minX;
    qreal alto = maxY - minY;

    // Devuelve el rectángulo delimitador
    return QRectF(QPointF(minX, minY), QSizeF(ancho, alto));
}

