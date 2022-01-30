#include "functions.h"
#include "estructuras.h"


//Entrada: Recibe un "string" que representa el nombre del archivo de entrada y la direccion de memoria de una entero 
//Funcionamiento: Se lee el archivo de entrada, se almacena la informacion de los impactos en un arreglo y se asigna la cantidad de particulas mediante la direccion de memoria
//Salida: Arreglo de Particula que contiene la informacion de los impactos
Particula *leerEntrada(char const *nombreArchivo, int *cantidadParticulas){
    FILE *pf;
    pf = fopen(nombreArchivo, "r");

    //Se comprueba que exista el archivo de entrada indicado
    if(pf == NULL){
        Particula *listaParticulas = (Particula*)malloc(sizeof(Particula) * 0);
        *cantidadParticulas = -1;
        return listaParticulas;
    }

    //Se copia la cantidad de particulas 
    fscanf(pf, "%d", cantidadParticulas);
    //Se crea el arreglo para almacenar las particulas
    Particula *listaParticulas = (Particula*)malloc(sizeof(Particula) * (*cantidadParticulas));

    //Ciclo para leer y almacenar las particulas del archivo
    for(int i = 0; i < *cantidadParticulas; i++){
        Particula nueva;
        fscanf(pf, "%d", &nueva.posicion);
        fscanf(pf, "%f", &nueva.energia);

        //Se agrega la nueva particula al arreglo
        listaParticulas[i] = nueva;
    }

    //Se cierra el archivo
    fclose(pf);

    return listaParticulas;
}

//Entrada: Entero que representa la cantidad de elementos que poseera el arreglo de flotantes
//Funcionamiento: Se asigna memoria para un arreglo de flotantes del largo indicado y se asigna el valor 0 en cada posicion del arreglo
//Salida: Arreglo de flotantes con un 0 en cada posicion
float *inicializarArreglo(int largo){
    //Se asigna memoria
    float *arreglo = (float*)malloc(sizeof(float) * largo);
    //Ciclo para recorrer las posiciones del arreglo
    for(int i = 0; i < largo; i++){
        arreglo[i] = 0;
    }

    return arreglo;
}

//Entrada: Arreglo de flotantes que representa el material, entero con la catidad de celdas del material, la Particula que impacta a analizar, flotante con el umbral minimo de energia
//Funcionamiento: Se calcula la energia resultante para cada celda del material producida por el impacto de la particula indicada y se registra en caso de superar el umbral minimo
//Salida: No se retorna nada, solo se actualizan los valores del arreglo
void energiaResultante(float *arregloMaterial, int cantCeldas, Particula particula, float minEnergia){
    //Ciclo para conseguir la energia resultante en cada celda del material
    for(int i = 0; i < cantCeldas; i++){
        //Se calcula la energia resultante en la celda
        float nuevaEnergia = arregloMaterial[i] + ((1000 * particula.energia) / (cantCeldas * sqrt(abs(particula.posicion - i) + 1)));

        //Se comprueba que la energia resultante sea mayor al umbral
        if(nuevaEnergia >= minEnergia){
            arregloMaterial[i] = nuevaEnergia;
        }
    }

    return;
}

//Entrada: Arreglo de flotantes que representa el material, entero con la catidad de celdas del material, arreglo de Particulas que impactan el material, entero con la cantidad de particulas
//Funcionamiento: Se calcula el umbral minimo de energia a registrar y luego se simula el impacto de las particulas sobre el material
//Salida: No se retorna nada, solo se actualizan los valores del arreglo
void simularImpacto(float *arregloMaterial, int cantCeldas, Particula *listaParticulas, int cantParticulas){
    //Se calcula el umbral minimo de energia
    float minEnergia = 0.001/cantCeldas;

    //Ciclo para simular el impacto de cada una de las particulas
    for(int i = 0; i < cantParticulas; i++){
        energiaResultante(arregloMaterial, cantCeldas, listaParticulas[i], minEnergia);
    }

    return;
}

//Entrada: Arreglo de flotantes que representa el material y un entero con la cantidad de celdas del material
//Funcionamiento: Busca el indice dentro del arreglo de la celda del material que posee mayor cantidad de energia almacenada
//Salida: Flotante con el indice dentro del arreglo de la celda con mayor energia 
int indiceMayorEnergia(float *arregloMaterial, int cantCeldas){
    //Se utiliza el primer valor del arreglo como pivote para comparar
    int indiceMayor = 0;
    float valorMayor = arregloMaterial[0];

    //Se compara con el resto de las energias
    for(int i = 1; i < cantCeldas; i++){
        //Se comprueba si la energia de la celda es mayor a la actual mayor energia
        if(arregloMaterial[i] > valorMayor){
            indiceMayor = i;
            valorMayor = arregloMaterial[i];
        }
    }

    return indiceMayor;
}

//Entrada: Arreglo de caracteres con el nombre del archivo de salida, arreglo de flotantes que representa el material, entero con la cantidad de celdas del material, entero con el indice de la celda con mayor energia almacenada
//Funcionamiento: Crea un archivo de texto con el nombre indicado y escribe la informacion de cada una de las celdas del material, partiendo con la celda de mayor energia
//Salida: Archivo de texto plano con la informacion del material
void generarSalida(char const *nombreArchivo, float *arregloMaterial, int cantCeldas, int mayorIndice){
    FILE *salida;
    //Se abre el archivo en modo escritura
    salida = fopen(nombreArchivo, "w");

    //Se escribe la informacion de la celda con mayor energia al inicio
    fprintf(salida, "%d %f\n", mayorIndice, arregloMaterial[mayorIndice]);

    //Ciclo para escribir el resto de las celdas
    for(int i = 0; i < cantCeldas; i++){
        fprintf(salida, "%d %f\n", i, arregloMaterial[i]);
    }

    //Se cierra el archivo de salida
    fclose(salida);

    return;
}