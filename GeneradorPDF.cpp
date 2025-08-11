#include "GeneradorPDF.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>

void GeneradorPDF::generarPDFDesdeArchivoTeX(const std::string& rutaArchivoTeX, const std::string& rutaGuardado) {
    // Verificar que el archivo .tex no está vacío
    std::string rutaCompletaTeX = "tex/" + rutaArchivoTeX + ".tex";
    std::ifstream archivoTeX(rutaCompletaTeX, std::ios::binary | std::ios::ate);

    if (!archivoTeX) {
        std::cerr << "Error: No se pudo abrir el archivo .tex: " << rutaCompletaTeX << std::endl;
        return;
    }

    std::streamsize fileSize = archivoTeX.tellg();
    archivoTeX.close();

    if (fileSize == 0) {
        std::cerr << "Error: El archivo .tex está vacío." << std::endl;
        return;
    }

    // Compilar el archivo .tex para generar el archivo PDF
    compileLatexToPdf(rutaCompletaTeX, rutaGuardado);
}

void GeneradorPDF::compileLatexToPdf(const std::string& latexFilePath, const std::string& rutaGuardado) {
    std::string comando = "pdflatex -interaction=nonstopmode -output-directory=" + rutaGuardado + " " + latexFilePath;

    int resultado = std::system(comando.c_str());
    if (resultado != 0) {
        std::cerr << "pdflatex terminó con errores. Código de salida: " << resultado << std::endl;
    } else {
        std::cout << "Compilación de pdflatex exitosa." << std::endl;
    }
}

void GeneradorPDF::deleteTempFiles(const std::vector<std::string> filePaths) {
    for (const std::string& filePath : filePaths) {
        if (std::remove(filePath.c_str()) != 0) {
            std::cerr << "Error al eliminar el archivo: " << filePath << std::endl;
        }
    }
}
