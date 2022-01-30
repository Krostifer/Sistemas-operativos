#ifndef Functions_h_
#define Functions_h_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include "estructuras.h"

//Cabecera de las funciones utilizadas por el proceso padre

extern void niceprint(int N, float *Energy, float Maximum);
float *inicializarArreglo(int largo);
int indiceMayorEnergia(float *arregloMaterial, int cantCeldas);
void generarSalida(char const *nombreArchivo, float *arregloMaterial, int cantCeldas, int mayorIndice);
void leerInfoHijos(float *arregloMaterial, int cantCeldas, int numeroProcesos, int pipes[][2]);

#endif