# Casa Nostra

Sistema de gestión de la línea de sucesión para una familia de la mafia italiana. El programa carga los datos de los miembros desde un archivo CSV y los organiza en un árbol binario para determinar la línea de sucesión automáticamente.

## Requisitos

- Compilador con soporte para C++11 o superior
- Sistema operativo: Windows, Linux o macOS

## Estructura del proyecto

```
Casa_Nostra/
├── bin/
│   ├── datos_familia.csv   # Archivo de datos de la familia
│   └── main.exe             # Ejecutable del programa
├── src/
│   └── main.cpp             # Código fuente del programa
├── checklist.md             # Lista de verificación del proyecto
├── README.md                # Documentación del proyecto
└── requerimientos.md        # Enunciado original del proyecto
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

| Columna     | Descripción                              | Valores         |
|-------------|------------------------------------------|-----------------|
| id          | Identificador único del miembro          | Entero positivo  |
| name        | Nombre del miembro                       | Texto            |
| last_name   | Apellido del miembro                     | Texto            |
| gender      | Género                                   | H o M           |
| age         | Edad                                     | Entero           |
| id_boss     | ID del jefe directo (0 si es la raíz)    | Entero           |
| is_dead     | Indica si el miembro está muerto          | 0 o 1           |
| in_jail     | Indica si el miembro está en prisión      | 0 o 1           |
| was_boss    | Indica si el miembro fue jefe antes       | 0 o 1           |
| is_boss     | Indica si el miembro es el jefe actual    | 0 o 1           |

## Funcionalidades

### 1. Mostrar línea de sucesión
Muestra los miembros vivos ordenados jerárquicamente, indicando quién es el jefe actual.

### 2. Modificar miembro
Permite editar los datos de cualquier miembro: nombre, apellido, género, edad, estado (vivo/muerto) y situación (libre/prisión). No se permite modificar el ID ni el ID del jefe.

### 3. Resolver sucesión
Asigna un nuevo jefe automáticamente cuando el actual muere, va a prisión o supera los 70 años. La búsqueda del sucesor sigue este orden:

1. **Sucesión directa**: Busca en el subárbol del primer hijo (izquierdo), luego en el del segundo (derecho).
2. **Sucesión colateral**: Si no hay sucesor directo, busca en la rama del hermano del jefe, y si no, sube al padre y repite.
3. **Incluir presos**: Si no se encuentra ningún sucesor libre, repite la búsqueda incluyendo miembros encarcelados.

## Modelo de datos

El programa utiliza tres clases principales:

- **Persona**: Almacena los datos de cada miembro (id, nombre, apellido, género, edad, etc.).
- **NodoArbol**: Nodo del árbol binario con punteros al padre, hijo izquierdo y derecho.
- **ArbolFamilia**: Administra el árbol completo con métodos para insertar, buscar, mostrar y resolver la sucesión.

## Datos de prueba

El archivo `bin/datos_familia.csv` incluye datos de ejemplo de la familia Maranzano para probar todas las funcionalidades del programa.
