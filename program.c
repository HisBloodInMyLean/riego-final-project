/* ========================================================
 * Tema: Sistema de Riego
 * Autores: Iván Maldonado Rodríguez y Daniel Palma Valdés
 * Docentes: Hugo Araya Carrasco && Luis Ponce Rosales
 * Curso: INF-123 Programación I
 * Institución: Universidad Católica del Maule
 * Fecha: 17-12-2025
 * ========================================================
 */
#include <stdio.h>
#define MAX_NOMBRE_PLANTA   (31)

typedef struct sistema_riego {
    float caudal;
    char cultivo[MAX_NOMBRE_PLANTA];
    int num_plantas;
    float litros;
} SistemaRiego;

/* ## Prototipos */
void limpiar_buffer();
void leer_cadena(char *, int);
void mostrar_menu(SistemaRiego);
void interactuar_menu(int *, SistemaRiego *);
void calcular_tiempo(SistemaRiego, float *);
void exportar_riego(SistemaRiego);

/* ## Main */
int main() {
    /* Inicialización de variables del sistema */
    SistemaRiego riego = {0.0, "No definido", 0, 0.0};
    int selector = 0;

    /* Bucle del menú del sistema*/
    do {
        mostrar_menu(riego);
        printf("Seleccione una de las opciones: ");
        scanf("%d", &selector);

        interactuar_menu(&selector, &riego);

    } while (selector != 5);
    printf("::: [Gracias por hacer uso del sistema] :::\n");
    return 0;
}

/* ## Funciones */
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

/* Lee una cadena de entrada evitando errores de buffer */
void leer_cadena(char * dest, int size) {
    int i = 0, c;
    while (i < size) {
        c = getchar();
        /* Termina de leer si se presiona Enter*/
        if (c == EOF || c == '\n') break;
        dest[i] = (char)c;
        i++;
    }
    dest[i] = '\0';
    /* En caso de no haberse presionado Enter, se limpia el bucle */
    if (c != EOF && c != '\n') {
        limpiar_buffer();
    }
}

/* Muestra el menú del sistema en pantalla con información ingresada por usuario */
void mostrar_menu(SistemaRiego riego) {
    printf("::::::::::::::::::::::::::::::  Menu  ::::::::::::::::::::::::::::::\n");
    printf("1.- Caudal maximo en litros por minuto (%.1f L)\n", riego.caudal);
    printf("2.- Tipo de planta o cultivo ('%s')\n", riego.cultivo);
    printf("3.- Numero de plantas (x%d)\n", riego.num_plantas);
    printf("4.- Litros requeridos por planta (%.2f)\n", riego.litros);
    printf("5.- Salir/Exportar\n");
    printf("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");   
}

/* Manejo de respuesta al seleccionar una opción del menú */
void interactuar_menu(int * selector, SistemaRiego * riego) {
    switch (*selector) {
        /* 1.- Caudal máximo en litros por minuto*/
        case 1: {
            printf("::: Ingrese el caudal maximo\n>> ");
            scanf("%f", &riego->caudal);
            break;
        }
        /* 2.- Tipo de planta o cultivo */
        case 2: {
            printf("::: Ingrese el tipo de planta o cultivo (max %d caracteres)\n>> ", MAX_NOMBRE_PLANTA - 1);
            leer_cadena(riego->cultivo, MAX_NOMBRE_PLANTA);
            break;
        }
        /* 3.- Número de plantas */
        case 3: {
            printf("::: Ingrese el numero de plantas\n>> ");
            scanf("%d", &riego->num_plantas);
            break;
        }
        /* 4.- Litros requeridos por planta */
        case 4: {
            printf("::: Ingrese los litros requeridos por planta\n>> ");
            scanf("%f", &riego->litros);
            break;
        }
        /* 5.- Salir/Exportar */
        case 5: {
            if (riego->caudal <= 0) {
                printf("[!] Error: El caudal debe ser mayor a 0.\n\n");
                /* Se modifica selector a 0 para evitar que finalice el programa */
                *selector = 0;
            } else {
                exportar_riego(*riego);
            }
            break;
        }
        /* Casos no válidos */
        default: {
            printf("[!] Error: Seleccione una opcion valida (1 - 5)\n\n");
            break;
        }
    }
}

/* Calcula el tiempo de riego en base a formula con datos ingresados por usuarios */
void calcular_tiempo(SistemaRiego riego, float * minutos) {
    /* Fórmula: tiempo_minutos = (cantidad_plantas × litros_por_planta) / caudal_litros_min */
    *minutos = (riego.num_plantas * riego.litros) / riego.caudal;
}

/* Exporta información de riego del cultivo ingresado */
void exportar_riego(SistemaRiego riego) {
    FILE * f;
    f = fopen("riego.txt", "w");
    if (f == NULL) {
        printf("[!] Error: no se logró exportar el archivo.\n\n");
        return;
    }

    float minutos;
    /* Obtiene el tiempo de riego */
    calcular_tiempo(riego, &minutos);
    
    fprintf(f, "/* ------------------------------------------------------- */\n");
    fprintf(f, "Sistema de riego por goteo\n");
    fprintf(f, "Plantas : %d\n", riego.num_plantas);
    fprintf(f, "Litros por planta : %.2f\n", riego.litros);
    fprintf(f, "Caudal válvula L/min : %.2f\n", riego.caudal);
    fprintf(f, "------------------------------------------------------------\n");
    fprintf(f, "Tiempo de apertura : %.2f min\n", minutos);
    fprintf(f, "Tiempo en segundos : %.0f s\n", minutos*60);
    fprintf(f, "Litros totales : %.0f L\n", ((riego.litros) * (riego.num_plantas)));
    fprintf(f, "/* ------------------------------------------------------- */\n");
    fclose(f);
    printf("Se ha exportado el archivo 'riego.txt' correctamente.\n");
}