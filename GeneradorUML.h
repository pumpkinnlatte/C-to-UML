#ifndef GENERADOR_UML_H
#define GENERADOR_UML_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <QRectF>
#include "CppAnalizador.h"

class GeneradorUML {
public:
    static std::string generarDiagramaUML(const std::vector<CppAnalizador::ClaseCPlusPlus>& clases, std::vector<QPointF> posiciones);

    static std::string generarDiagramaClase(const CppAnalizador::ClaseCPlusPlus clase, int posX, int posY);

    static std::string generarTex(const std::string& codigoLaTeX, const std::string& nombreDiagrama);

    static std::string join(const std::vector<std::string>& vec, const std::string& sep);

    static QRectF calcularRectanguloDelimitador(const std::vector<QPointF>& puntos);

};

#endif // GENERADOR_UML_H
