/*
 * ProduControl - Sistema de Optimizacion de Produccion
 * Fabrica de Componentes Electronicos
 * ISWZ1102 - Programacion 1
 *
 * Restricciones: sin struct, sin archivos, sin funciones predefinidas
 * Integrantes: Emily Obando, Scarlett Alcivar, Karolayns Calva, Jairo Subia y
 * Vannesa Vallejo
 */

#include <stdio.h>

#define MAX_PRODUCTOS 5
#define MAX_NOMBRE 50

/* ---------- Prototipos ---------- */
/* Lectura de datos */
void limpiarBuffer(void);
int leerEnteroPositivo(const char *mensaje);
float leerRealPositivo(const char *mensaje);
void leerNombre(const char *mensaje, char *destino);

/* Manejo de texto propio (sin string.h) */
void copiarTexto(char *destino, const char *origen);

/* Menu y operaciones */
void mostrarMenu(void);
void ingresarProductos(char nombres[][MAX_NOMBRE], int cantidades[],
                       float tiempos[], float recursos[], int *total);
void mostrarProductos(char nombres[][MAX_NOMBRE], int cantidades[],
                      float tiempos[], float recursos[], int total);

/* Busqueda, editar y eliminar (parte P2) */
int buscarProducto(char nombres[][MAX_NOMBRE], int total, const char *buscado);
void editarProducto(char nombres[][MAX_NOMBRE], int cantidades[],
                    float tiempos[], float recursos[], int total);
void eliminarProducto(char nombres[][MAX_NOMBRE], int cantidades[],
                      float tiempos[], float recursos[], int *total);

/* Calculos */
float calcularTiempoTotal(int cantidades[], float tiempos[], int total);
float calcularRecursosTotales(int cantidades[], float recursos[], int total);
void evaluarFactibilidad(int cantidades[], float tiempos[],
                         float recursos[], int total);

/* ---------- Programa principal ---------- */
int main(void)
{
    char nombres[MAX_PRODUCTOS][MAX_NOMBRE];
    int cantidades[MAX_PRODUCTOS];
    float tiempos[MAX_PRODUCTOS];
    float recursos[MAX_PRODUCTOS];
    int total = 0;
    int opcion;

    do {
        mostrarMenu();
        opcion = leerEnteroPositivo("Seleccione una opcion: ");

        switch (opcion) {
            case 1:
                ingresarProductos(nombres, cantidades, tiempos, recursos, &total);
                break;
            case 2:
                mostrarProductos(nombres, cantidades, tiempos, recursos, total);
                break;
            case 3:
                editarProducto(nombres, cantidades, tiempos, recursos, total);
                break;
            case 4:
                eliminarProducto(nombres, cantidades, tiempos, recursos, &total);
                break;
            case 5:
                evaluarFactibilidad(cantidades, tiempos, recursos, total);
                break;
            case 0:
                printf("\nGracias por usar ProduControl. Hasta pronto.\n");
                break;
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 0);

    return 0;
}

/* ---------- Lectura de datos ---------- */

/* Descarta lo que quede escrito en la entrada hasta el Enter */
void limpiarBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

/* Pide un entero mayor o igual a 0 hasta que el usuario lo ingrese bien */
int leerEnteroPositivo(const char *mensaje)
{
    int valor;
    int leido;

    do {
        printf("%s", mensaje);
        leido = scanf("%d", &valor);
        limpiarBuffer();
        if (leido != 1 || valor < 0) {
            printf("  Valor invalido. Ingrese un numero entero mayor o igual a 0.\n");
        }
    } while (leido != 1 || valor < 0);

    return valor;
}

/* Pide un numero real mayor que 0 hasta que el usuario lo ingrese bien */
float leerRealPositivo(const char *mensaje)
{
    float valor;
    int leido;

    do {
        printf("%s", mensaje);
        leido = scanf("%f", &valor);
        limpiarBuffer();
        if (leido != 1 || valor <= 0) {
            printf("  Valor invalido. Ingrese un numero mayor que 0.\n");
        }
    } while (leido != 1 || valor <= 0);

    return valor;
}

/* Lee un texto con espacios (maximo 49 caracteres) */
void leerNombre(const char *mensaje, char *destino)
{
    printf("%s", mensaje);
    scanf(" %49[^\n]", destino);
    limpiarBuffer();
}

/* ---------- Manejo de texto propio ---------- */

/* Copia un texto caracter por caracter usando punteros */
void copiarTexto(char *destino, const char *origen)
{
    while (*origen != '\0') {
        *destino = *origen;
        destino++;
        origen++;
    }
    *destino = '\0';
}

/* ---------- Menu ---------- */
void mostrarMenu(void)
{
    printf("\n==============================================\n");
    printf("     PRODUCONTROL - FABRICA ELECTRONICA\n");
    printf("==============================================\n");
    printf(" 1. Ingresar productos\n");
    printf(" 2. Mostrar productos\n");
    printf(" 3. Editar producto\n");
    printf(" 4. Eliminar producto\n");
    printf(" 5. Calcular tiempo, recursos y factibilidad\n");
    printf(" 0. Salir\n");
    printf("----------------------------------------------\n");
}

/* ---------- Ingreso de productos ---------- */
void ingresarProductos(char nombres[][MAX_NOMBRE], int cantidades[],
                       float tiempos[], float recursos[], int *total)
{
    int espacio = MAX_PRODUCTOS - *total;
    int n;
    int i;

    if (espacio == 0) {
        printf("\nYa hay %d productos registrados (maximo permitido).\n", MAX_PRODUCTOS);
        return;
    }

    printf("\nPuede ingresar hasta %d producto(s).\n", espacio);
    do {
        n = leerEnteroPositivo("Cuantos productos desea ingresar?: ");
        if (n < 1 || n > espacio) {
            printf("  Debe ingresar entre 1 y %d.\n", espacio);
        }
    } while (n < 1 || n > espacio);

    for (i = 0; i < n; i++) {
        int pos = *total;

        printf("\n--- Producto %d ---\n", pos + 1);

        /* No se permiten nombres repetidos */
        do {
            leerNombre("Nombre: ", nombres[pos]);
            if (buscarProducto(nombres, *total, nombres[pos]) != -1) {
                printf("  Ese producto ya existe. Ingrese otro nombre.\n");
            }
        } while (buscarProducto(nombres, *total, nombres[pos]) != -1);

        cantidades[pos] = leerEnteroPositivo("Cantidad demandada (unidades): ");
        tiempos[pos] = leerRealPositivo("Tiempo de fabricacion por unidad (horas): ");
        recursos[pos] = leerRealPositivo("Recursos por unidad: ");

        (*total)++;
    }

    printf("\nProductos registrados correctamente. Total: %d\n", *total);
}

/* ---------- Mostrar productos ---------- */
void mostrarProductos(char nombres[][MAX_NOMBRE], int cantidades[],
                      float tiempos[], float recursos[], int total)
{
    int i;

    if (total == 0) {
        printf("\nNo hay productos registrados.\n");
        return;
    }

    printf("\n%-4s %-20s %10s %12s %12s\n",
           "N.", "Producto", "Cantidad", "Tiempo/u", "Recursos/u");
    printf("------------------------------------------------------------\n");
    for (i = 0; i < total; i++) {
        printf("%-4d %-20s %10d %12.2f %12.2f\n",
               i + 1, nombres[i], cantidades[i], tiempos[i], recursos[i]);
    }
}

/* =====================================================
   PARTE DE P2 (busqueda, editar y eliminar)
   ===================================================== */

/* Busqueda secuencial: recorre los nombres uno por uno y compara
   letra por letra. Devuelve la posicion, o -1 si no lo encuentra. */
int buscarProducto(char nombres[][MAX_NOMBRE], int total, const char *buscado)
{
    int i;
    int j;

    for (i = 0; i < total; i++) {
        j = 0;
        while (nombres[i][j] == buscado[j] && buscado[j] != '\0') {
            j++;
        }
        if (nombres[i][j] == buscado[j]) {
            return i;
        }
    }
    return -1;
}

/* Editar: busca el producto y pide de nuevo todos sus datos */
void editarProducto(char nombres[][MAX_NOMBRE], int cantidades[],
                    float tiempos[], float recursos[], int total)
{
    char buscado[MAX_NOMBRE];
    char nuevoNombre[MAX_NOMBRE];
    int pos;
    int otro;

    if (total == 0) {
        printf("\nNo hay productos registrados para editar.\n");
        return;
    }

    leerNombre("\nNombre del producto a editar: ", buscado);

    pos = buscarProducto(nombres, total, buscado);
    if (pos == -1) {
        printf("  Producto no encontrado.\n");
        return;
    }

    printf("\n--- Editando producto %d ---\n", pos + 1);

    /* El nuevo nombre no puede ser igual al de otro producto
       (puede quedar igual al que ya tenia) */
    do {
        leerNombre("Nuevo nombre: ", nuevoNombre);
        otro = buscarProducto(nombres, total, nuevoNombre);
        if (otro != -1 && otro != pos) {
            printf("  Ya existe otro producto con ese nombre.\n");
        }
    } while (otro != -1 && otro != pos);
    copiarTexto(nombres[pos], nuevoNombre);

    cantidades[pos] = leerEnteroPositivo("Nueva cantidad demandada: ");
    tiempos[pos] = leerRealPositivo("Nuevo tiempo por unidad (horas): ");
    recursos[pos] = leerRealPositivo("Nuevos recursos por unidad: ");

    printf("\nProducto editado correctamente.\n");
}

/* Eliminar: busca el producto y mueve los siguientes una posicion
   hacia atras para tapar el hueco */
void eliminarProducto(char nombres[][MAX_NOMBRE], int cantidades[],
                      float tiempos[], float recursos[], int *total)
{
    char buscado[MAX_NOMBRE];
    int pos;
    int i;
    int j;

    if (*total == 0) {
        printf("\nNo hay productos registrados para eliminar.\n");
        return;
    }

    leerNombre("\nNombre del producto a eliminar: ", buscado);

    pos = buscarProducto(nombres, *total, buscado);
    if (pos == -1) {
        printf("  Producto no encontrado.\n");
        return;
    }

    for (i = pos; i < *total - 1; i++) {
        for (j = 0; j < MAX_NOMBRE; j++) {
            nombres[i][j] = nombres[i + 1][j];
        }
        cantidades[i] = cantidades[i + 1];
        tiempos[i] = tiempos[i + 1];
        recursos[i] = recursos[i + 1];
    }

    (*total)--;
    printf("\nProducto eliminado correctamente. Quedan %d producto(s).\n", *total);
}

/* ---------- Calculos ---------- */

/* Tiempo total = suma de (cantidad x tiempo por unidad) */
float calcularTiempoTotal(int cantidades[], float tiempos[], int total)
{
    float suma = 0;
    int i;

    for (i = 0; i < total; i++) {
        suma += cantidades[i] * tiempos[i];
    }
    return suma;
}

/* Recursos totales = suma de (cantidad x recursos por unidad) */
float calcularRecursosTotales(int cantidades[], float recursos[], int total)
{
    float suma = 0;
    int i;

    for (i = 0; i < total; i++) {
        suma += cantidades[i] * recursos[i];
    }
    return suma;
}

/* Compara lo necesario con lo disponible y dice si se cumple la demanda */
void evaluarFactibilidad(int cantidades[], float tiempos[],
                         float recursos[], int total)
{
    float tiempoNecesario;
    float recursosNecesarios;
    float tiempoDisponible;
    float recursosDisponibles;

    if (total == 0) {
        printf("\nPrimero debe ingresar productos.\n");
        return;
    }

    tiempoNecesario = calcularTiempoTotal(cantidades, tiempos, total);
    recursosNecesarios = calcularRecursosTotales(cantidades, recursos, total);

    printf("\n========== RESULTADOS ==========\n");
    printf("Tiempo total requerido:   %.2f horas\n", tiempoNecesario);
    printf("Recursos totales:         %.2f unidades\n", recursosNecesarios);

    printf("\n--- Capacidad de la fabrica ---\n");
    tiempoDisponible = leerRealPositivo("Tiempo de produccion disponible (horas): ");
    recursosDisponibles = leerRealPositivo("Recursos disponibles: ");

    printf("\n--- Evaluacion ---\n");
    if (tiempoNecesario <= tiempoDisponible) {
        printf("Tiempo:   SUFICIENTE (sobran %.2f horas)\n",
               tiempoDisponible - tiempoNecesario);
    } else {
        printf("Tiempo:   INSUFICIENTE (faltan %.2f horas)\n",
               tiempoNecesario - tiempoDisponible);
    }

    if (recursosNecesarios <= recursosDisponibles) {
        printf("Recursos: SUFICIENTES (sobran %.2f)\n",
               recursosDisponibles - recursosNecesarios);
    } else {
        printf("Recursos: INSUFICIENTES (faltan %.2f)\n",
               recursosNecesarios - recursosDisponibles);
    }

    if (tiempoNecesario <= tiempoDisponible &&
        recursosNecesarios <= recursosDisponibles) {
        printf("\n>> La fabrica SI puede cumplir con la demanda.\n");
    } else {
        printf("\n>> La fabrica NO puede cumplir con la demanda.\n");
    }
}
