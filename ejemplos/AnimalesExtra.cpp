class Animal {
private:
    string tipo;
    int edad;

public:
    Animal(string tipo, int edad) : tipo(tipo), edad(edad) {}

    string getTipo() {
        return tipo;
    }

    int getEdad() {
        return edad;
    }

    void hacerSonido() {
        if (tipo == "Perro") {
            cout << "Guau" << endl;
        } else if (tipo == "Gato") {
            cout << "Miau" << endl;
        } else {
            cout << "No se reconoce el tipo de animal" << endl;
        }
    }
};

class Perro : public Animal {
private:
    string raza;

public:
    Perro(string tipo, int edad, string raza) : Animal(tipo, edad), raza(raza) {}

    string getRaza() {
        return raza;
    }

    void correr() {
        cout << "El perro está corriendo" << endl;
    }
};

class Gato : public Animal {
private:
    bool esPeludo;

public:
    Gato(string tipo, int edad, bool esPeludo) : Animal(tipo, edad), esPeludo(esPeludo) {}

    bool esPeludo()              {
        return esPeludo;
    }

    void maullar(){
        cout << "El gato está maullando" << endl;
    }
};

class GatoShrek : public Gato {
private:
    bool esPeludo;

public:
    GatoShrek(string tipo, int edad, bool esPeludo) : Gato(tipo, edad), esPeludo(esPeludo) {}

    bool esPeludo()              {
        return esPeludo;
    }

    void maullar(){
        cout << "El gato está maullando" << endl;
    }
};

class GatoSiames : public Gato {
private:
    bool esPeludo;

public:
    GatoSiames(string tipo, int edad, bool esPeludo) : Gato(tipo, edad), esPeludo(esPeludo) {}

    bool esPeludo()              {
        return esPeludo;
    }

    void maullar(){
        cout << "El gato está maullando" << endl;
    }
};


class GatoAngora : public Gato {
private:
    bool esPeludo;

public:
    GatoAngora(string tipo, int edad, bool esPeludo) : Gato(tipo, edad), esPeludo(esPeludo) {}

    bool esPeludo()              {
        return esPeludo;
    }

    void maullar(){
        cout << "El gato está maullando" << endl;
    }
};
