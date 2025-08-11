#ifndef GENERADOR_PDF_H
#define GENERADOR_PDF_H

#include <string>
#include <vector>

class GeneradorPDF {
public:
    static void generarPDFDesdeArchivoTeX(const std::string& rutaArchivoTeX, const std::string& rutaGuardado);
    static void deleteTempFiles(const std::vector<std::string> filePaths);

private:
    static void compileLatexToPdf(const std::string& latexFilePath, const std::string& rutaGuardado);
};

#endif // GENERADOR_PDF_H
