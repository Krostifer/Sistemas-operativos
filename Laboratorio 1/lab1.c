#include "functions.h"
#include "estructuras.h"

int main(int argc, char **argv){

    int cantCeldas = 0;
    char *valorEntrada = NULL;
    char *valorSalida = NULL;
    int banderaSalida = 0;
    int c;
    opterr = 0;

    //Ciclo para obtener la informacion entregada por linea de comando
    while((c = getopt(argc, argv, "N:i:o:D")) != -1){
        switch(c){
            case 'N':
                sscanf(optarg, "%d", &cantCeldas);
                break;
            case 'i':
                valorEntrada = optarg;
                break;
            
            case 'o':
                valorSalida = optarg;
                break;
            
            case 'D':
                banderaSalida = 1;
                break;
            
            case '?':
                if(optopt == 'o'){
                    fprintf(stderr, "Es necesario indicar un argumento en el parametro -%c.\n", optopt);
                }

                else if(isprint(optopt)){
                    fprintf(stderr, "No se puede reconocer la opcion ingresada en el parametro -%c.\n", optopt);
                }

                else{
                    fprintf(stderr, "Se ha ingresado un caracter desconocido '\\x%x'.\n", optopt);
                }
            
            default:
                abort();
        }
    }

    //Se definen las variables para tomar el tiempo de ejecucion
    clock_t tiempoInicio, tiempoFinal;
    //Se toma el tiempo en que se inicia la ejecucion
    tiempoInicio = clock();

    //Se crea la variable para almacenar la cantidad de particulas
    int cantParticulas = 0;
    //Se lee el archivo de entrada
    Particula *listaParticulas = leerEntrada(valorEntrada, &cantParticulas);

    //Se comprueba que el archivo se haya abierto sin problemas
    if(cantParticulas == -1){
        printf("El archivo de entrada indicado no existe.\n");
        return 0;
    }
    
    //Se inicializa el arreglo que representa las celdas del material
    float *arregloMaterial = inicializarArreglo(cantCeldas);

    //Se simula el impacto de las particulas contra el material
    simularImpacto(arregloMaterial, cantCeldas, listaParticulas, cantParticulas);

    //Se obtiene el indice de la celda con mayor energia
    int indiceMayor = indiceMayorEnergia(arregloMaterial, cantCeldas);
    //Se genera el archivo de salida
    generarSalida(valorSalida, arregloMaterial, cantCeldas, indiceMayor);

    //Se verifica si se requiere entregar una salida por consola
    if(banderaSalida == 1){
        niceprint(cantCeldas, arregloMaterial, arregloMaterial[indiceMayor]);
    }

    //Se toma el tiempo al termino de la ejecucion
    tiempoFinal = clock();
    //Se calcula la cantidad de segundos
    double tiempoEje = (tiempoFinal - tiempoInicio) / CLOCKS_PER_SEC;
    //Se muestra por pantalla el tiempo de ejecucion
    printf("El tiempo de ejecucion fue de: %f segundos.\n", tiempoEje);

    //Se libera la memoria utilizada por los arreglos
    free(listaParticulas);
    free(arregloMaterial);

    return 0;
}