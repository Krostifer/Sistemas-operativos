#include "functions.h"
#include "bomb.h"
#include "estructuras.h"

int main(int argc, char **argv){

    int cantCeldas = 0;
    int numeroProcesos = 0;
    int cantLineasEntrada = 0;
    char *valorEntrada = NULL;
    char *valorSalida = NULL;
    int banderaSalida = 0;
    int x;
    opterr = 0;

    //Ciclo para obtener la informacion entregada por linea de comando
    while((x = getopt(argc, argv, "N:p:c:i:o:D")) != -1){
        switch(x){
            case 'N':
                sscanf(optarg, "%d", &cantCeldas);
                break;
                
            case 'p':
                sscanf(optarg, "%d", &numeroProcesos);
                break;
            case 'c':
                sscanf(optarg, "%d", &cantLineasEntrada);
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

    //Comprobacion del parametro de entrada N
    if(cantCeldas == 0){
        printf("El material no posee celdas.\n");
        return 1;
    }

    //Se comprueba que la cantidad de lineas del archivo de entrada indicadas, a pesar de que no se utilice este dato
    else if(cantLineasEntrada <= 1){
        printf("Error con la cantidad de lineas indicadas para el archivo de entrada.\n");
        return 2;
    }
    
    //Se inicializa el arreglo que representa las celdas del material
    float *arregloMaterial = inicializarArreglo(cantCeldas);
    //Arreglo con los pids de los subprocesos
    int pids[numeroProcesos];
    //Arreglo con los descriptores de los  pipes
    int pipes[numeroProcesos][2];

    //Ciclo para la creacion de los subprocesos
    for(int i = 0; i < numeroProcesos; i++){
        //Creacion de los pipes y verificacion
        if(pipe(pipes[i]) == -1){
            printf("Fallo la creacion del pipe.\n");
            return 3;
        }

        //Se crea un subproceso y se almacena su pid en el arreglo
        pids[i] = fork();
        //Se comprueba que el subproceso se haya creado correctamente
        if(pids[i] == -1){
            printf("Fallo la creacion del hijo.\n");
            return 4;
        }

        //Ejecucion para los subprocesos
        if(pids[i] == 0){
            int resultado = simulacionSubproceso(valorEntrada, cantLineasEntrada, arregloMaterial, cantCeldas, numeroProcesos, pipes, i);

            //Se comprueba si hubo un error en la ejecucion del subproceso
            if(resultado == 1){
                printf("El proceso %d termino debido a un error en la ejecucion.\n", i);
                return 5;
            }
            
            else{
                return 0;
            }
        }

        //Se cierra el extremo de escritura del pipe correspondiente al subproceso creado
        close(pipes[i][1]);
    }
    
    //Se lee la informacion enviada por los subprocesos
    leerInfoHijos(arregloMaterial, cantCeldas, numeroProcesos, pipes);
    
    //Ciclo para esperar el termino de la ejecucion de cada hijo
    for(int i = i; i < numeroProcesos; i++){
        wait(NULL);
    }

   //Se obtiene el indice de la celda con mayor energia
    int indiceMayor = indiceMayorEnergia(arregloMaterial, cantCeldas);
    //Se genera el archivo de salida
    generarSalida(valorSalida, arregloMaterial, cantCeldas, indiceMayor);

    //Se verifica si se requiere entregar una salida por consola
    if(banderaSalida == 1){
        niceprint(cantCeldas, arregloMaterial, arregloMaterial[indiceMayor]);
    }

    //Se libera la memoria utilizada por el arreglo del material
    free(arregloMaterial);


    return 0;
}