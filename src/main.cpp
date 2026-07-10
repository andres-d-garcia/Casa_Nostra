#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

string trim(const string &value) {
    size_t start = value.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }

    size_t end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

// CLASE PERSONA
class Persona {
public:
    int id;
    string name;
    string last_name;
    char gender;
    int age;
    int id_boss;
    bool is_dead;
    bool in_jail;
    bool was_boss;
    bool is_boss;

    Persona()
        : id(0), name(""), last_name(""), gender('M'), age(0), id_boss(0),
          is_dead(false), in_jail(false), was_boss(false), is_boss(false) {}

    Persona(int pId, const string &pName, const string &pLastName, char pGender,
            int pAge, int pIdBoss, bool pIsDead, bool pInJail, bool pWasBoss,
            bool pIsBoss)
        : id(pId), name(pName), last_name(pLastName), gender(pGender), age(pAge),
          id_boss(pIdBoss), is_dead(pIsDead), in_jail(pInJail), was_boss(pWasBoss),
          is_boss(pIsBoss) {}
};


// CLASE NODO ARBOL
class NodoArbol {
public:
    Persona dato;
    NodoArbol *padre;
    NodoArbol *izquierdo;
    NodoArbol *derecho;

    explicit NodoArbol(const Persona &pDato)
        : dato(pDato), padre(nullptr), izquierdo(nullptr), derecho(nullptr) {}
};

// CLASE ARBOL FAMILIA
class ArbolFamilia {
private:
    NodoArbol *raiz;

    NodoArbol *buscarPorId(NodoArbol *actual, int id) {
        if (actual == nullptr) {
            return nullptr;
        }

        if (actual->dato.id == id) {
            return actual;
        }

        NodoArbol *izquierdoEncontrado = buscarPorId(actual->izquierdo, id);
        if (izquierdoEncontrado != nullptr) {
            return izquierdoEncontrado;
        }

        return buscarPorId(actual->derecho, id);
    }

    void destruir(NodoArbol *actual) {
        if (actual == nullptr) {
            return;
        }

        destruir(actual->izquierdo);
        destruir(actual->derecho);
        delete actual;
    }

public:
    ArbolFamilia() : raiz(nullptr) {}

    ~ArbolFamilia() {
        destruir(raiz);
    }

    void insertar(const Persona &persona) {
        NodoArbol *nuevo = new NodoArbol(persona);

        if (raiz == nullptr) {
            raiz = nuevo;
            return;
        }

        NodoArbol *padre = buscarPorId(raiz, persona.id_boss);
        if (padre == nullptr) {
            nuevo->padre = nullptr;
            if (raiz->izquierdo == nullptr) {
                raiz->izquierdo = nuevo;
            } else {
                raiz->derecho = nuevo;
            }
            return;
        }

        nuevo->padre = padre;
        if (padre->izquierdo == nullptr) {
            padre->izquierdo = nuevo;
        } else if (padre->derecho == nullptr) {
            padre->derecho = nuevo;
        } else {
            padre->derecho->derecho = nuevo;
        }
    }

    bool cargarDesdeCsv(const string &rutaArchivo) {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            return false;
        }

        string linea;
        getline(archivo, linea);

        while (getline(archivo, linea)) {
            if (trim(linea).empty()) {
                continue;
            }

            stringstream ss(linea);
            string campo;
            Persona persona;
            int index = 0;

            while (getline(ss, campo, ',')) {
                campo = trim(campo);

                switch (index) {
                case 0:
                    persona.id = stoi(campo);
                    break;
                case 1:
                    persona.name = campo;
                    break;
                case 2:
                    persona.last_name = campo;
                    break;
                case 3:
                    persona.gender = campo[0];
                    break;
                case 4:
                    persona.age = stoi(campo);
                    break;
                case 5:
                    persona.id_boss = stoi(campo);
                    break;
                case 6:
                    persona.is_dead = (campo == "1");
                    break;
                case 7:
                    persona.in_jail = (campo == "1");
                    break;
                case 8:
                    persona.was_boss = (campo == "1");
                    break;
                case 9:
                    persona.is_boss = (campo == "1");
                    break;
                }

                ++index;
            }

            insertar(persona);
        }

        archivo.close();
        return true;
    }

    void mostrarSucesionActual() const {
        cout << "Linea de sucesion actual:" << endl;
        mostrarEnOrden(raiz);
    }

    void mostrarEnOrden(NodoArbol *actual) const {
        if (actual == nullptr) {
            return;
        }

        mostrarEnOrden(actual->izquierdo);
        if (!actual->dato.is_dead && !actual->dato.in_jail) {
            cout << actual->dato.id << " - " << actual->dato.name << " "
                 << actual->dato.last_name << endl;
        }
        mostrarEnOrden(actual->derecho);
    }

    NodoArbol *encontrarPorId(int id) {
        return buscarPorId(raiz, id);
    }

    bool editarNodo(int id, const Persona &nuevaInformacion) {
        NodoArbol *nodo = encontrarPorId(id);
        if (nodo == nullptr) {
            return false;
        }

        nodo->dato.name = nuevaInformacion.name;
        nodo->dato.last_name = nuevaInformacion.last_name;
        nodo->dato.gender = nuevaInformacion.gender;
        nodo->dato.age = nuevaInformacion.age;
        nodo->dato.is_dead = nuevaInformacion.is_dead;
        nodo->dato.in_jail = nuevaInformacion.in_jail;
        nodo->dato.was_boss = nuevaInformacion.was_boss;
        nodo->dato.is_boss = nuevaInformacion.is_boss;
        nodo->dato.id_boss = nuevaInformacion.id_boss;
        return true;
    }
};





