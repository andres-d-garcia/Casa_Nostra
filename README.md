# Casa Nostra

Sistema de gestión de la línea de sucesión para una familia de la mafia italiana. El programa carga los datos de los miembros desde un archivo CSV y los organiza en un árbol binario para determinar la línea de sucesión.

## Requisitos

- Compilador con soporte para C++11 o superior
- Sistema operativo: Windows, Linux o macOS

## Estructura del proyecto

```
Casa_Nostra/
├── bin/
│   └── datos_familia.csv   # Archivo de datos de la familia
├── src/
│   └── main.cpp             # Código fuente del programa
├── README.md
└── requerimientos.md
```

## Compilación

Desde la raíz del proyecto:

```bash
g++ src/main.cpp -o bin/main
```

En Windows con MinGW:

```bash
g++ src/main.cpp -o bin/main.exe
```

## Ejecución

Desde la raíz del proyecto:

```bash
bin/main
```

Opcionalmente se puede pasar la ruta al archivo CSV como argumento:

```bash
bin/main ruta/al/archivo.csv
```

## Formato del CSV

El archivo CSV debe contener las siguientes columnas en orden:

```
id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss
```

- `gender`: H (hombre) o M (mujer)
- `is_dead`, `in_jail`, `was_boss`, `is_boss`: 1 (sí) o 0 (no)
- `id_boss`: ID del jefe directo (0 si es la raíz)

## Funcionalidades

1. **Mostrar línea de sucesión**: Muestra los miembros vivos ordenados jerárquicamente
2. **Modificar miembro**: Permite editar los datos de cualquier miembro (excepto ID e ID del jefe)
3. **Resolver sucesión**: Asigna un nuevo jefe automáticamente cuando el actual muere, va a prisión o supera los 70 años
