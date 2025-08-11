#include "CppAnalizador.h"
#include "stringF.h"
#include <fstream>
#include <iostream>

std::string CppAnalizador::parseVisibilidad(const std::string& v) {
    if (v == "public") return "+";
    if (v == "private") return "-";
    if (v == "protected") return "#";
    return "";
}

std::string CppAnalizador::leerArchivo(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string CppAnalizador::preprocesarCodigoCpp(const std::string& codigoCpp) {
    std::string codigo = codigoCpp;

    // Expresión regular para encontrar y eliminar el contenido de los métodos
    std::regex metodos(R"(\b\w+\s+\w+\s*\([^)]*\)\s*\{(?:[^{}]*|\{(?:[^{}]*|\{[^{}]*\})*\})*\})");
    std::string codigoConMetodosModificados;
    auto method_begin = std::sregex_iterator(codigo.begin(), codigo.end(), metodos);
    auto method_end = std::sregex_iterator();
    size_t lastPos = 0;

    for (std::sregex_iterator i = method_begin; i != method_end; ++i) {
        std::smatch match = *i;
        std::string metodo = match.str();
        std::regex contenidoMetodo(R"(\{(?:[^{}]*|\{(?:[^{}]*|\{[^{}]*\})*\})*\})");
        std::string metodoModificado = std::regex_replace(metodo, contenidoMetodo, "{}");

        // Añadir el texto anterior al metodo modificado
        codigoConMetodosModificados += codigo.substr(lastPos, match.position() - lastPos);
        // Añadir el metodo modificado
        codigoConMetodosModificados += metodoModificado;
        lastPos = match.position() + match.length();
    }

    // Añadir el texto restante después del último metodo
    codigoConMetodosModificados += codigo.substr(lastPos);

    // Expresión regular para encontrar y eliminar las listas de inicialización
    std::regex constructores(R"((\b\w+\s*\([^)]*\)\s*):\s*([^{}]*))");
    std::string codigoSinListas = std::regex_replace(codigoConMetodosModificados, constructores, "$1");

    // Eliminar contenido de los métodos y reemplazarlos por un punto y coma
    std::string codigoSinLlavesVacias = std::regex_replace(codigoSinListas, std::regex(R"(\{\s*\})"), ";");

    // Eliminar espacios entre los paréntesis y el punto y coma al final de los métodos
    std::string codigoFinal = std::regex_replace(codigoSinLlavesVacias, std::regex(R"(\)\s*;)"), ");");

    std::cout << "Código Pre-procesado\n";
    std::cout << codigoFinal << std::endl;

    return codigoFinal;
}



std::vector<CppAnalizador::ClaseCPlusPlus> CppAnalizador::analizarCodigoCpp(const std::string& codigoCpp) {
    std::vector<ClaseCPlusPlus> clases;
    std::regex patronClase(R"(\b(class|struct)\s+(\w+)(?:\s*:\s*([\w\s,:]+))?\s*\{([\s\S]*?)\};)");
    std::smatch matcherClase;
    std::string codigo = codigoCpp;

    while (std::regex_search(codigo, matcherClase, patronClase)) {
        std::string nombreClase = matcherClase[2].str();
        std::string herencia = matcherClase[3].str();
        std::string contenidoClase = matcherClase[4].str();

        // Regex para encontrar "public", "private", o "protected" seguidos de un espacio
        std::regex regexVisibilidad(R"(\b(public|private|protected)\s+)");

        // Reemplaza la coincidencia con una cadena vacía
        herencia = std::regex_replace(herencia, regexVisibilidad, "");

        stringF::trim(herencia); //eliminar espacios

        std::vector<ClaseCPlusPlus::Atributo> atributos;
        std::vector<ClaseCPlusPlus::Metodo> metodos;

        std::regex patronBloqueVisibilidad(R"((public|private|protected):([\s\S]*?)(?=(public|private|protected):|$))");
        std::smatch matcherBloqueVisibilidad;

        while (std::regex_search(contenidoClase, matcherBloqueVisibilidad, patronBloqueVisibilidad)) {
            std::string visibilidad = parseVisibilidad(matcherBloqueVisibilidad[1].str());
            std::string contenidoBloque = matcherBloqueVisibilidad[2].str();

            // Procesar atributos
            std::regex patronAtributo(R"(\s*(?:const\s+)?(\w[\w<>:]*[*&\s]*)\s+(\w+)(?:\s*=\s*(.*?))?\s*;)");
            std::smatch matcherAtributo;

            while (std::regex_search(contenidoBloque, matcherAtributo, patronAtributo)) {
                std::string tipo = matcherAtributo[1].str();
                std::string nombre = matcherAtributo[2].str();
                std::string valorDefecto = matcherAtributo[3];

                if (!valorDefecto.empty()) {
                    tipo += " = " + valorDefecto;
                }

                atributos.emplace_back(visibilidad, tipo, nombre);
                contenidoBloque = matcherAtributo.suffix().str();
            }

            // Procesar métodos
            std::regex patronMetodo(R"(\s*(?:virtual\s+)?(\w[\w<>:]*[*&\s]*)\s+(\w+)\s*\(([^)]*)\)\s*(?:const)?\s*;)");
            std::smatch matcherMetodo;

            while (std::regex_search(contenidoBloque, matcherMetodo, patronMetodo)) {
                std::string tipoRetorno = matcherMetodo[1].str();
                std::string nombre = matcherMetodo[2].str();
                std::string argumentosStr = matcherMetodo[3].str();

                std::vector<std::string> listaArgumentos;
                std::stringstream ss(argumentosStr);
                std::string argumento;

                while (std::getline(ss, argumento, ',')) {
                    stringF::trim(argumento);
                    listaArgumentos.push_back(argumento);
                }

                if (nombre == nombreClase) {
                    tipoRetorno = "";
                }

                metodos.emplace_back(visibilidad, tipoRetorno, nombre, listaArgumentos);
                contenidoBloque = matcherMetodo.suffix().str();
            }

            contenidoClase = matcherBloqueVisibilidad.suffix().str();
        }

        ClaseCPlusPlus nuevaClase(nombreClase, herencia, atributos, metodos);
        clases.push_back(nuevaClase);

        // Mostrar el resultado deseado
        std::cout << "Contenido de la clase " << nombreClase << ":" << std::endl;
        std::cout << "Atributos:" << std::endl;
        for (const auto& atributo : nuevaClase.atributos) {
            std::cout << atributo.visibilidad << " " << atributo.tipo << " " << atributo.nombre << ";" << std::endl;
        }

        std::cout << "Metodos:" << std::endl;
        for (const auto& metodo : nuevaClase.metodos) {
            std::cout << metodo.visibilidad << " " << metodo.tipoRetorno << " " << metodo.nombre << "(";
            for (size_t i = 0; i < metodo.argumentos.size(); ++i) {
                if (i > 0) std::cout << ", ";
                std::cout << metodo.argumentos[i];
            }
            std::cout << ");" << std::endl;
        }

        // Actualiza `codigo` para procesar el siguiente bloque de clase
        codigo = matcherClase.suffix().str();
    }

    return clases;
}