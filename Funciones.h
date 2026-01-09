#ifndef FUNCIONES_H
#define FUNCIONES_H

#define MAX 50

typedef struct {
    char id[MAX];
    char marca[MAX];
    char tipo[MAX];
    int anio;
    float precio;
    int disponible;
} Vehiculo;

typedef struct {
    char id[MAX];
    char nombre[MAX];
    int edad;
    float presupuesto;
} Cliente;

typedef struct {
    char idVenta[MAX];
    char idCliente[MAX];
    char idVehiculo[MAX];
} Venta;

void menu();

void registrarVehiculo();
void listarVehiculos();
void buscarVehiculosPorPresupuesto();

void registrarCliente();

void registrarVenta(char idCliente[], char idVehiculo[]);

int validarAlfanumerico(char cad[]);
int validarSoloLetras(char cad[]);
int leerEntero();
float leerFloat();

#endif
