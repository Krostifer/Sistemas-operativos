#ifndef Bomb_h_
#define Bomb_h_
#include "estructuras.h"

//Cabecera de las funciones utilizadas por los subprocesos

Particula *leerEntrada(char const *nombreArchivo, int *cantidadParticulas);
void energiaResultante(float *arregloMaterial, int cantCeldas, Particula particula, float minEnergia);
void simularImpacto(float *arregloMaterial, int cantCeldas, Particula *listaParticulas, int particulaInicioSimulacion, int particulaFinSimulacion);
int simulacionSubproceso(char *valorEntrada, int cantLineasEntrada, float *arregloMaterial, int cantCeldas, int numeroProcesos, int pipes[][2], int indiceProceso);
#endif