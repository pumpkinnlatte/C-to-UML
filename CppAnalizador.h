#ifndef CPP_ANALIZADOR_H
#define CPP_ANALIZADOR_H

#include <string>
#include <vector>
#include <regex>

class CppAnalizador {
public:
    static std::string parseVisibilidad(const std::string& v);
    static std::string leerArchivo(const std::string& filePath);
    static std::string preprocesarCodigoCpp(const std::string& codigoCpp);

    class ClaseCPlusPlus {
    public:
        class Atributo {
        public:
            std::string visibilidad;
            std::string tipo;
            std::string nombre;

            Atributo(std::string visibilidad, std::string tipo, std::string nombre) : visibilidad(visibilidad), tipo(tipo), nombre(nombre){}
        };

        class Metodo {
        public:
            std::string visibilidad;
            std::string tipoRetorno;
            std::string nombre;
            std::vector<std::string> argumentos;

            Metodo(const std::string& visibilidad, const std::string& tipoRetorno, const std::string& nombre, const std::vector<std::string>& argumentos)
                    : visibilidad(visibilidad), tipoRetorno(tipoRetorno), nombre(nombre), argumentos(argumentos) {}
        };


        std::string nombre;
        std::string herencia;
        std::vector<Atributo> atributos;
        std::vector<Metodo> metodos;

        ClaseCPlusPlus(std::string nombre, std::string herencia, std::vector<Atributo> atributos, std::vector<Metodo> metodos) : nombre(nombre), herencia(herencia), atributos(atributos), metodos(metodos){}
    };

    static std::vector<ClaseCPlusPlus> analizarCodigoCpp(const std::string& codigoCpp);
};

#endif // CPP_ANALIZADOR_H
