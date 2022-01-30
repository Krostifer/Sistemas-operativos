#ifndef Functions_h_
#define Functions_h_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "estructuras.h"

//Cabecera de las funciones utilizadas

extern void niceprint(int N, float *Energy, float Maximum);
Particula *leerEntrada(char const *nombreArchivo, int *cantidadParticulas);
float *inicializarArreglo(int largo);
void energiaResultante(float *arregloMaterial, int cantCeldas, Particula particula, float minEnergia);
void simularImpacto(float *arregloMaterial, int cantCeldas, Particula *listaParticulas, int cantParticulas);
int indiceMayorEnergia(float *arregloMaterial, int cantCeldas);
void generarSalida(char const *nombreArchivo, float *arregloMaterial, int cantCeldas, int mayorIndice);


#endif