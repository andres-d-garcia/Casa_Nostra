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




