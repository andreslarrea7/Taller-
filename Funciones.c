#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funciones.h"

/* ---------- VALIDACIONES ---------- */
int validarAlfanumerico(char cad[]) {
    int letra = 0, numero = 0;
    for (int i = 0; cad[i]; i++) {
        if (isalpha(cad[i])) letra = 1;
        else if (isdigit(cad[i])) numero = 1;
        else return 0;
    }
    return letra && numero;
}

int validarSoloLetras(char cad[]) {
    for (int i = 0; cad[i]; i++) {
        if (!isalpha(cad[i])) return 0;
    }
    return 1;
}

int leerEntero() {
    int n;
    while (scanf("%d", &n) != 1) {
        while (getchar() != '\n');
        printf("Entrada invalida, ingrese nuevamente\n");
    }
    return n;
}

float leerFloat() {
    float n;
    while (scanf("%f", &n) != 1) {
        while (getchar() != '\n');
        printf("Entrada invalida, ingrese nuevamente\n");
    }
    return n;
}

/* ---------- MENU ---------- */
void menu() {
    int op;
    do {
        printf("\n--- CONCESIONARIA RUEDAS DE ORO ---\n");
        printf("1. Registrar vehiculo\n");
        printf("2. Listar vehiculos\n");
        printf("3. Registrar cliente\n");
        printf("4. Buscar vehiculos por cliente\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        op = leerEntero();

        switch (op) {
            case 1: registrarVehiculo(); break;
            case 2: listarVehiculos(); break;
            case 3: registrarCliente(); break;
            case 4: buscarVehiculosPorPresupuesto(); break;
        }
    } while (op != 0);
}

/* ---------- VEHICULOS ---------- */
void registrarVehiculo() {
    FILE *f = fopen("vehiculos.dat", "ab");
    Vehiculo v;

    do {
        printf("ID Vehiculo: ");
        scanf("%s", v.id);
        if (!validarAlfanumerico(v.id))
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (!validarAlfanumerico(v.id));

    do {
        printf("Marca: ");
        scanf("%s", v.marca);
        if (!validarSoloLetras(v.marca))
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (!validarSoloLetras(v.marca));

    do {
        printf("Tipo de vehiculo: ");
        scanf("%s", v.tipo);
        if (!validarSoloLetras(v.tipo))
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (!validarSoloLetras(v.tipo));

    do {
        printf("Anio: ");
        v.anio = leerEntero();
        if (v.anio < 1900 || v.anio > 2100)
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (v.anio < 1900 || v.anio > 2100);

    do {
        printf("Precio: ");
        v.precio = leerFloat();
        if (v.precio <= 0)
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (v.precio <= 0);

    v.disponible = 1;
    fwrite(&v, sizeof(Vehiculo), 1, f);
    fclose(f);
}

void listarVehiculos() {
    FILE *f = fopen("vehiculos.dat", "rb");
    Vehiculo v;

    printf("\n%-15s %-15s %-15s %-10s\n",
           "ID", "MARCA", "TIPO", "PRECIO");
    printf("----------------------------------------------------------\n");

    while (fread(&v, sizeof(Vehiculo), 1, f)) {
        if (v.disponible) {
            printf("%-15s %-15s %-15s $%8.2f\n",
                   v.id, v.marca, v.tipo, v.precio);
        }
    }
    fclose(f);
}

/* ---------- CLIENTES ---------- */
void registrarCliente() {
    FILE *f = fopen("clientes.dat", "ab");
    Cliente c;

    do {
        printf("ID Cliente: ");
        scanf("%s", c.id);
        if (!validarAlfanumerico(c.id))
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (!validarAlfanumerico(c.id));

    do {
        printf("Nombre: ");
        scanf("%s", c.nombre);
        if (!validarSoloLetras(c.nombre))
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (!validarSoloLetras(c.nombre));

    do {
        printf("Edad: ");
        c.edad = leerEntero();
        if (c.edad <= 0 || c.edad > 120)
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (c.edad <= 0 || c.edad > 120);

    do {
        printf("Presupuesto: ");
        c.presupuesto = leerFloat();
        if (c.presupuesto <= 0)
            printf("Entrada invalida, ingrese nuevamente\n");
    } while (c.presupuesto <= 0);

    fwrite(&c, sizeof(Cliente), 1, f);
    fclose(f);
}

/* ---------- BUSQUEDA Y VENTA ---------- */
void buscarVehiculosPorPresupuesto() {
    FILE *fv = fopen("vehiculos.dat", "rb+");
    FILE *fc = fopen("clientes.dat", "rb");
    Vehiculo v;
    Cliente c;
    char idCliente[MAX], idVehiculo[MAX];
    int encontrado = 0;

    printf("Ingrese ID del cliente: ");
    scanf("%s", idCliente);

    while (fread(&c, sizeof(Cliente), 1, fc)) {
        if (strcmp(c.id, idCliente) == 0) {
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Entrada invalida, ingrese nuevamente\n");
        fclose(fv);
        fclose(fc);
        return;
    }

    printf("\nVehiculos dentro del presupuesto %.2f\n", c.presupuesto);
    printf("%-15s %-15s %-15s %-10s\n",
           "ID", "MARCA", "TIPO", "PRECIO");
    printf("----------------------------------------------------------\n");

    rewind(fv);
    while (fread(&v, sizeof(Vehiculo), 1, fv)) {
        if (v.disponible && v.precio <= c.presupuesto) {
            printf("%-15s %-15s %-15s $%8.2f\n",
                   v.id, v.marca, v.tipo, v.precio);
        }
    }

    printf("Seleccione ID del vehiculo: ");
    scanf("%s", idVehiculo);

    rewind(fv);
    while (fread(&v, sizeof(Vehiculo), 1, fv)) {
        if (strcmp(v.id, idVehiculo) == 0 && v.disponible) {
            v.disponible = 0;
            fseek(fv, -sizeof(Vehiculo), SEEK_CUR);
            fwrite(&v, sizeof(Vehiculo), 1, fv);
            registrarVenta(idCliente, idVehiculo);
            break;
        }
    }

    fclose(fv);
    fclose(fc);
}

/* ---------- VENTAS ---------- */
void registrarVenta(char idCliente[], char idVehiculo[]) {
    FILE *f = fopen("ventas.dat", "ab");
    Venta v;

    sprintf(v.idVenta, "V%04d", rand() % 10000);
    strcpy(v.idCliente, idCliente);
    strcpy(v.idVehiculo, idVehiculo);

    fwrite(&v, sizeof(Venta), 1, f);
    fclose(f);

    printf("VENTA REGISTRADA\n");
}

