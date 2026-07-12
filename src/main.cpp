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
    NodoArbol *jefeActual;

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

    void _mostrarSucesoresVivos(NodoArbol *nodo, int nivel) const {
        if (nodo == nullptr) {
            return;
        }

        
        if (nodo->dato.is_dead == 0) {
            
            for (int i = 0; i < nivel; ++i) {
                cout << "  ";
            }

            cout << "- " << nodo->dato.name << " " << nodo->dato.last_name
                 << " (Edad: " << nodo->dato.age << ")";

            if (nodo->dato.is_boss == 1) {
                cout << " <-- JEFE ACTUAL";
            }
            cout << endl;
        }

        
        _mostrarSucesoresVivos(nodo->izquierdo, nivel + 1);
        _mostrarSucesoresVivos(nodo->derecho, nivel + 1);
    }

    NodoArbol *_buscarJefe(NodoArbol *nodo) {
        if (nodo == nullptr) {
            return nullptr;
        }
        if (nodo->dato.is_boss) {
            return nodo;
        }
        NodoArbol *encontrado = _buscarJefe(nodo->izquierdo);
        if (encontrado != nullptr) {
            return encontrado;
        }
        return _buscarJefe(nodo->derecho);
    }

    NodoArbol *_buscarSucesorEnSubarbol(NodoArbol *subRaiz, bool incluirPresos) {
        if (subRaiz == nullptr) {
            return nullptr;
        }

        bool esValido = !subRaiz->dato.is_dead && (incluirPresos || !subRaiz->dato.in_jail);

        if (esValido) {
            return subRaiz;
        }

        NodoArbol *sucesor = _buscarSucesorEnSubarbol(subRaiz->izquierdo, incluirPresos);
        if (sucesor != nullptr) {
            return sucesor;
        }

        return _buscarSucesorEnSubarbol(subRaiz->derecho, incluirPresos);
    }

    void asignarNuevoJefe(NodoArbol *nuevoJefe, NodoArbol *antiguoJefe) {
        if (antiguoJefe != nullptr) {
            antiguoJefe->dato.is_boss = false;
            antiguoJefe->dato.was_boss = true;
            cout << "El antiguo jefe, " << antiguoJefe->dato.name << " "
                 << antiguoJefe->dato.last_name << ", ha dejado el puesto." << endl;
        }

        if (nuevoJefe != nullptr) {
            nuevoJefe->dato.is_boss = true;
            jefeActual = nuevoJefe;

            cout << "\n¡NUEVO JEFE ASIGNADO!" << endl;
            cout << "El nuevo jefe de la familia es: " << nuevoJefe->dato.name << " "
                 << nuevoJefe->dato.last_name << endl;
        } else {
            cout << "\nCRISIS DE SUCESIÓN: No se pudo encontrar un nuevo jefe para la "
                    "familia."
                 << endl;
            if (antiguoJefe != nullptr) {
                jefeActual = nullptr;
            }
        }
    }


public:
    ArbolFamilia() : raiz(nullptr), jefeActual(nullptr) {}

    ~ArbolFamilia() {
        destruir(raiz);
    }

    void insertar(const Persona &persona) {
        NodoArbol *nuevo = new NodoArbol(persona);

        if (raiz == nullptr) {
            if (persona.id_boss == 0) {
                raiz = nuevo;
            } else {
                cerr << "Error: El árbol está vacío, pero el primer miembro (ID: " << persona.id << ") no es el jefe principal (id_boss != 0)." << endl;
                delete nuevo;
            }
            return;
        }

        if (persona.id_boss == 0) {
            cerr << "Error: Se encontró un segundo miembro (ID: " << persona.id << ") con id_boss=0. Solo puede haber una raíz." << endl;
            delete nuevo;
            return;
        }

        NodoArbol *padre = buscarPorId(raiz, persona.id_boss);
        if (padre == nullptr) {
            cerr << "Error de inserción: No se encontró el padre con ID " << persona.id_boss << " para el miembro " << persona.name << " (ID: " << persona.id << ")." << endl;
            cerr << "Asegúrese de que el archivo CSV esté ordenado jerárquicamente (padres antes que hijos)." << endl;
            delete nuevo;
            return;
        }

        nuevo->padre = padre;
        if (padre->izquierdo == nullptr) {
            padre->izquierdo = nuevo;
        } else if (padre->derecho == nullptr) {
            padre->derecho = nuevo;
        } else {
            cerr << "Error de estructura: El padre ID " << padre->dato.id 
                 << " (" << padre->dato.name << ") ya tiene dos sucesores directos." << endl;
            cerr << "No se puede insertar a " << persona.name << " (ID: " << persona.id 
                 << ") como tercer sucesor directo. El modelo actual es un árbol binario." << endl;
            delete nuevo;
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
        jefeActual = _buscarJefe(raiz);
        return true;
    }

    void mostrarLineaDeSucesion() const {
        if (jefeActual == nullptr) {
            cout << "No se ha asignado un jefe actual en la familia." << endl;
            return;
        }

        cout << "\n--- Línea de Sucesión Actual (solo miembros vivos) ---\n"
             << endl;
        _mostrarSucesoresVivos(jefeActual, 0);
        cout << "\n-----------------------------------------------------\n"
             << endl;
    }

    void resolverSucesion() {
        if (jefeActual == nullptr) {
            cout << "No hay un jefe actual para resolver la sucesión." << endl;
            return;
        }

        bool necesitaSucesor = jefeActual->dato.is_dead ||
                               jefeActual->dato.in_jail ||
                               jefeActual->dato.age > 70;

        if (!necesitaSucesor) {
            cout << "El jefe actual (" << jefeActual->dato.name
                 << ") sigue en condiciones de liderar." << endl;
            return;
        }

        cout << "\nEl jefe actual " << jefeActual->dato.name << " "
             << jefeActual->dato.last_name
             << " ya no puede liderar. Buscando un sucesor..." << endl;

        NodoArbol *antiguoJefe = jefeActual;
        NodoArbol *nuevoJefe = nullptr;

        for (int i = 0; i < 2 && nuevoJefe == nullptr; ++i) {
            bool incluirPresos = (i == 1);
            if (incluirPresos) {
                cout << "No se encontraron sucesores libres. Ampliando la búsqueda para incluir miembros encarcelados..." << endl;
            }

            nuevoJefe = _buscarSucesorEnSubarbol(antiguoJefe->izquierdo, incluirPresos);
            if (nuevoJefe == nullptr) {
                nuevoJefe = _buscarSucesorEnSubarbol(antiguoJefe->derecho, incluirPresos);
            }

            if (nuevoJefe == nullptr) {
                NodoArbol *ancestro = antiguoJefe;
                while (ancestro->padre != nullptr && nuevoJefe == nullptr) {
                    NodoArbol *padreAncestro = ancestro->padre;
                    NodoArbol *tio = (padreAncestro->izquierdo == ancestro)
                                         ? padreAncestro->derecho
                                         : padreAncestro->izquierdo;

                    if (tio != nullptr) {
                        nuevoJefe = _buscarSucesorEnSubarbol(tio, incluirPresos);
                    }
                    ancestro = padreAncestro;
                }
            }
        }

        asignarNuevoJefe(nuevoJefe, antiguoJefe);
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
        // El id y el id_boss no se pueden modificar según los requisitos.
        // Los estados is_boss y was_boss son manejados por la lógica de sucesión.
        return true;
    }
};

void mostrarDetallesPersona(const Persona& p) {
    cout << "\n--- Detalles del Miembro ---" << endl;
    cout << "ID: " << p.id << endl;
    cout << "Nombre: " << p.name << " " << p.last_name << endl;
    cout << "Género: " << p.gender << endl;
    cout << "Edad: " << p.age << endl;
    cout << "ID Jefe: " << p.id_boss << endl;
    cout << "Estado: " << (p.is_dead ? "Muerto" : "Vivo") << endl;
    cout << "Situación: " << (p.in_jail ? "En la cárcel" : "Libre") << endl;
    cout << "Jefe actual: " << (p.is_boss ? "Sí" : "No") << endl;
    cout << "Fue jefe: " << (p.was_boss ? "Sí" : "No") << endl;
    cout << "---------------------------\n";
}

int main(int argc, char *argv[]) {
    ArbolFamilia familia;
    string ruta_csv;

    if (argc > 1) {
        ruta_csv = argv[1];
        cout << "Intentando cargar desde la ruta proporcionada: " << ruta_csv << endl;
    } else {
        ruta_csv = "bin/datos_familia.csv";
    }

    if (!familia.cargarDesdeCsv(ruta_csv)) {
        cerr << "Error: No se pudo cargar el archivo '" << ruta_csv << "'.\n"
             << "Asegúrese de que la ruta sea correcta o ejecute el programa desde la carpeta raíz del proyecto."
             << endl;
        return 1;
    }

    cout << "Árbol familiar cargado exitosamente." << endl;

    string input;
    while (true) {
        cout << "\n--- Menú Principal ---" << endl;
        cout << "1. Mostrar línea de sucesión actual" << endl;
        cout << "2. Modificar miembro de la familia" << endl;
        cout << "3. Resolver sucesión (si es necesario)" << endl;
        cout << "4. Salir" << endl;
        cout << "Seleccione una opción: ";
        getline(cin, input);

        if (input == "1") {
            familia.mostrarLineaDeSucesion();
        } else if (input == "2") {
            cout << "Introduzca el ID del miembro a modificar: ";
            getline(cin, input);
            try {
                int id_to_edit = stoi(input);
                NodoArbol *nodo = familia.encontrarPorId(id_to_edit);
                if (nodo) {
                    Persona p = nodo->dato;
                    mostrarDetallesPersona(p);

                    cout << "--- Menú de Edición ---" << endl;
                    cout << "1. Editar nombre" << endl;
                    cout << "2. Editar apellido" << endl;
                    cout << "3. Editar género" << endl;
                    cout << "4. Editar edad" << endl;
                    cout << "5. Cambiar estado (muerto/vivo)" << endl;
                    cout << "6. Cambiar situación (cárcel/libre)" << endl;
                    cout << "7. Cancelar" << endl;
                    cout << "Seleccione el campo a modificar: ";
                    getline(cin, input);

                    string newValue;
                    bool cambioRealizado = false;

                    if (input == "1") {
                        cout << "Nuevo nombre: ";
                        getline(cin, p.name);
                        cambioRealizado = true;
                    } else if (input == "2") {
                        cout << "Nuevo apellido: ";
                        getline(cin, p.last_name);
                        cambioRealizado = true;
                    } else if (input == "3") {
                        cout << "Nuevo género (H/M): ";
                        getline(cin, newValue);
                        if (!newValue.empty() && (toupper(newValue[0]) == 'H' || toupper(newValue[0]) == 'M')) {
                            p.gender = toupper(newValue[0]);
                            cambioRealizado = true;
                        } else {
                            cout << "Género inválido. No se realizaron cambios." << endl;
                        }
                    } else if (input == "4") {
                        cout << "Nueva edad: ";
                        getline(cin, newValue);
                        try {
                            p.age = stoi(newValue);
                            cambioRealizado = true;
                        } catch (const std::invalid_argument &ia) {
                            cout << "Edad inválida. No se realizaron cambios." << endl;
                        }
                    } else if (input == "5") {
                        cout << "¿Está muerto? (1=si, 0=no): ";
                        getline(cin, newValue);
                        if (newValue == "1" || newValue == "0") {
                            p.is_dead = (newValue == "1");
                            cambioRealizado = true;
                        } else {
                            cout << "Opción inválida. No se realizaron cambios." << endl;
                        }
                    } else if (input == "6") {
                        cout << "¿Está en la cárcel? (1=si, 0=no): ";
                        getline(cin, newValue);
                        if (newValue == "1" || newValue == "0") {
                            p.in_jail = (newValue == "1");
                            cambioRealizado = true;
                        } else {
                            cout << "Opción inválida. No se realizaron cambios." << endl;
                        }
                    }

                    if (cambioRealizado) {
                        familia.editarNodo(id_to_edit, p);
                        cout << "Miembro actualizado exitosamente." << endl;
                    }
                } else {
                    cout << "ID no encontrado." << endl;
                }
            } catch (const std::invalid_argument &ia) {
                cerr << "Entrada inválida. Por favor ingrese un número para el ID." << endl;
            }
        } else if (input == "3") {
            familia.resolverSucesion();
        } else if (input == "4") {
            cout << "Saliendo del programa." << endl;
            break;
        } else {
            cout << "Opción no válida. Intente de nuevo." << endl;
        }
    }

    return 0;
}
