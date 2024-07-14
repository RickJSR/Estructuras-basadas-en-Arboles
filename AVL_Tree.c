//AVL Tree

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define YES 1
#define NO 0

//Enum para definir el tipo de rotación
enum rotationTyoe {LL, LR, RR, RL};

#define LL 0
#define LR 1
#define RR 2
#define RL 3

struct avl_node {
    int elem;                       //Elemento (entero)
    int height;                     //Altura (de abajo hacia arriba) ->la altura de un nodo que no existe es -1
    struct avl_node *right;         //Hijo derecho
    struct avl_node *left;          //Hijo izquierdo
};
typedef struct avl_node avl_tree;

//Función para determinar la altura de un nodo
int nodeHeight(avl_tree *n){
    //Primero se evalúa si n es nulo
    if(n==NULL)
        return -1;
    //Si no es nulo, simplemente se entregará su altura
    return n->height;
}

//Función para una rotación simple 
avl_tree *rotation(avl_tree *n, int type){         //void rotation(avl_tree **node, enum rotationType type){
    //avl_tree *n = *node;
    if(type == LL){     //Esto también puede implementarse con un switch case
        //Aquí tenemos un caso LEFT - LEFT
        //   Se sigue la nomenclatura de la diapositiva 78
        avl_tree *Y = n->left->right;
        avl_tree *k2 = n;
        n = n->left;
        n->right = k2;
        n->right->left = Y;
        n->right->height--;
    }
    if(type == RR){
        //Aquí tenemos un caso RIGHT - RIGHT
        //   Se sigue la nomenclatura de la diapositiva 78
        avl_tree *Y = n->right->left;
        avl_tree *k1 = n;
        n = n->right;
        n->left = k1;
        n->left->right = Y;
        n->left->height--;

    }
    if(type == LR){
        //Aquí tenemos un caso LEFT - RIGHT    
        n->left = rotation((n->left), RR);
        n = rotation(n, LL);
    }
    if(type == RL){
        //Aquí tenemos un caso RIGHT - LEFT    
        n->right = rotation((n->right), LL);
        n = rotation(n, RR);
    }
    return n;
}

//Función para encontrar la dirección del valor mínimo 
avl_tree *avl_find_min(avl_tree *n){
    //Esta línea rompe con la recursvidad 
    if( n == NULL){
        return NULL;
    }
    else if(n->left == NULL){
        return n;                          //Si eres el más chico, regresa n
    }
    else{
        return avl_find_min( n->left );    //Si aún no eres el más chico, sigue bajando en los lados izquierdos
    }
}

//Función para encontrar la dirección del valor máximo
avl_tree *avl_find_max(avl_tree *n){
    //Esta línea rompe con la recursvidad 
    if( n == NULL){
        return NULL;
    }
    else if(n->right == NULL){
        return n;                          //Si eres el más chico, regresa n
    }
    else{
        return avl_find_max( n->right );    //Si aún no eres el más chico, sigue bajando en los lados izquierdos
    }
}

//Función para determinar el máximo entre dos números
int max(int a, int b){
    //Si a es mayor a b, regresa a. Si no, regresa b.
    return a > b ? a : b;
}

//PROTOTIPO
/*Función para buscar un elemento en el árbol*/
int findAVL(avl_tree *n, int elem);

//Función insert recursivo
//Se busca que la función se mande llamar recursivamenet
avl_tree *insertAVL_int(avl_tree *n, int elem){
    if(findAVL(n, elem)==YES){
        return n;
    }
    //Primer caso: que "n" era nulo (no existía)
    if(n==NULL){
        n = (avl_tree*)malloc(sizeof(avl_tree)*1);
        if(n==NULL){
            fprintf(stderr, "Cannot allocate memory for AVL node!\n");
            exit(1);
        }
        n->elem = elem;
        n->height = 0;
        n->left = NULL;
        n->right = NULL;
        return n;
    }
    //Si la ejecución llega hasta aquí, n ya existía
    //Si el elemento a insertar es menor que el elemento en el nodo preexistente, se inserta en la izquierda
    if(n->elem > elem){
        //Recursividad
        n->left = insertAVL_int(n->left, elem);
        int left_h = nodeHeight(n->left);
        int right_h = nodeHeight(n->right);
        //Se analiza si es necesario una rotación (es decir, si se viola la propiedad AVL)
        if(left_h - right_h >= 2){
            //Tenemos un caso del tipo LEFT
            if(n->left->elem > elem){
                //Aquí tenemos un caso LEFT - LEFT
                n = rotation(n, LL);
            }
            else{
                //Aquí tenemos un caso LEFT - RIGHT
                n = rotation(n, LR);
            }
        }
    }
    
    //Si el elemento a insertar es mayor que el elemento en el nodo preexistente, se inserta en la derecha
    if(n->elem < elem){
        //Recursividad
        n->right = insertAVL_int(n->right, elem);
        int left_h = nodeHeight(n->left);
        int right_h = nodeHeight(n->right);
        //Se analiza si es necesario una rotación (es decir, si se viola la propiedad AVL)
        if(right_h - left_h >= 2){
            //Tenemos un caso del tipo RIGHT
            if(n->right->elem < elem){
                //Aquí tenemos un caso RIGHT - RIGHT
                n = rotation(n, RR);
            }
            else{
                //Aquí tenemos un caso RIGHT - LEFT
                n = rotation(n, RL);
            }
        }
    }
    
    //Se coloca la altura actual del nodo (la altura del hijo mayor más 1)
    int left_h = nodeHeight(n->left);
    int right_h = nodeHeight(n->right);
    n->height = max(left_h, right_h) + 1;
    return n;
}
//Función insert interno
//Se busca que la función se mande llamar internamente
void insertAVL(avl_tree **n, int elem){
    *n = insertAVL_int(*n, elem);
}

/*Función para buscar un elemento en el árbol*/
int findAVL(avl_tree *n, int elem){
    //Con el siguiente condicional se acaba la recursividad (si es nulo, no se encuentra)
    if(n == NULL){               //NOTA: Es imporante colocar paréntesis para definir de dónde sacar un dato (segmentation fault cuando no se encuentra nada en una dirección de memoria)
        return NO;
    }
    //Aquí llega la ejecución si aún no se encuentra el nodo
    //Entonces si el elemento del nodo es mayor al del elemento buscado, buscar en el nodo de la izquierda
    if(n->elem > elem){
        return findAVL(n->left, elem); 
    }
    //Entonces si el elemento del nodo es menor al del elemento buscado, buscar en el nodo de la derecha
    //... o bien, para quitar ambigüedades, al final lo dejamos así (si llega hasta aquí la ejecución, se buscará en el nodo derecho)
    if(n->elem < elem)                //Si ponemos esta línea de código como no comentario, debes agregar el siguiente del return que sigue
        return findAVL(n->right, elem); 
    //printf("Hay algo muy raro, llegué hasta aquí");
    if(n->elem != elem){
        printf("Hay algo raro con el elemento a buscar\n");
        return NO; 
    }
    return YES;
}


/*FUnción recursiva para borrar un elemento*/
avl_tree *deleteAVL(avl_tree **node, int elem){
    avl_tree *n = *node;
    avl_tree *tmp_node;                 //Nodo temporal
    avl_tree *child;
    //El siguiente IF es para un caso donde se quiera borrar un elemento en un nodo nulo (no se va a poder)
    if(n==NULL){
        printf("Element %d not found\n", elem);
        return NULL;
    }
    //Se posiciona al nodo donde se encuentra el elemento a borrar    
    if(n->elem > elem){
        n->left = deleteAVL(&n->left, elem);
        int left_h = nodeHeight(n->left);
        int right_h = nodeHeight(n->right);
        n->height = max(left_h, right_h)+1;
        //Se analiza si es necesario una rotación (es decir, si se viola la propiedad AVL)
        if(left_h - right_h >= 2){
            n = rotation(n, LL);
        }
        if(right_h - left_h >= 2){
            n = rotation(n, RR);
        }
        return n;
    }
    if(n->elem < elem){
        n->right = deleteAVL(&n->right, elem);
        int left_h = nodeHeight(n->left);
        int right_h = nodeHeight(n->right);
        n->height = max(left_h, right_h)+1;
        //Se analiza si es necesario una rotación (es decir, si se viola la propiedad AVL)
        if(left_h - right_h >= 2){
            n = rotation(n, LL);
        }
        if(right_h - left_h >= 2){
            //Tenemos un caso del tipo RIGHT
            n = rotation(n, RR);
        }
        return n;
    }
    else {
        if(n->elem != elem){
            printf("Hay algo raro con el elemento a borrar\n");
            return n;
        }
    }

    //AQUÍ YA SE ENCONTRÓ AL ELEMENTO A BORRAR
    // Borrado
    //Se evalúa si ambos hijos existen (no son nulos)
    if(n->left && n->right){
        //Se busca al más grande de los chicos
        tmp_node = avl_find_max(n->left);
        n->elem = tmp_node->elem;
        //Aquí resolvemos el caso donde el primer elemento de la izquierda no tiene hijos derechos (Este será el
        //más grande de los chicos). Antes de eliminarlo, se pasa el hijo de izquierdo de este al hijo izquierdo de n
        if(tmp_node->left!=NULL && tmp_node->right == NULL){
            n->left = deleteAVL(&n->left, n->elem);
        }
        else{
            //Recursividad
            n->left = deleteAVL(&n->left, n->elem);
            
        }
    }
    else{                               /*Un hijo o ninguno*/
        tmp_node = n;
        if(n->left == NULL){
            child = n->right;
        }
        if(n->right == NULL){
            child = n->left;
        }
        free(tmp_node);
        return child;
    }
    //Se actualiza la altura
    int left_h = nodeHeight(n->left);
    int right_h = nodeHeight(n->right);
    n->height = max(left_h, right_h)+1;
    //Se analiza si es necesario una rotación (es decir, si se viola la propiedad AVL)
    if(left_h - right_h >= 2){
        //Tenemos un caso del tipo LEFT
        n = rotation(n, LL);
    }
    if(right_h - left_h >= 2){
            n = rotation(n, RR);
    }
    return n;
}

/*************************************MAIN***********************************************************************/

int main(){
    char buffer[100];
    printf("Ingrese R para números aleatorios, A para ascedentes, D para descendientes seguido de la cantidad deseada: ");
    char command[5] = " ";
    char number[30] = " ";
    while(fgets(buffer, 100, stdin) != NULL){
        sscanf(buffer, "%s %s", command, number);
        break;
    }
    int numElem = atoi(number);
    int *entrada = malloc(sizeof(int)*numElem);
    if(entrada == NULL){
        fprintf(stderr, "No puedo reservar memoria para vector de entrada!\n");
        exit(1);
    }
    //NOTA: El siguiente arreglo (entrada2) sólo se usa en el caso especial donde se introducen datos de forma ascendente y luego de forma descendente
    int *entrada2 = malloc(sizeof(int)*numElem);
    if(entrada2 == NULL){
        fprintf(stderr, "No puedo reservar memoria para vector de entrada!\n");
        exit(1);
    }
    if(strcmp("R", command)==0){               //Números aleatorios
        //Generador de números aleatorios
        for(int i=0; i<numElem; i++){
            entrada[i] = i;
        }
        for(int i=0; i<numElem; i++){
            int iRand = rand()% numElem;
            int aux = entrada[i];
            entrada[i] = entrada [iRand];
            entrada[iRand] = aux;
        }
    }
    if(strcmp("A", command)==0){               //Números ordenados de forma ascendente
        for(int i=0; i<numElem; i++){
            entrada[i] = i;
        }
    }
    if(strcmp("D", command)==0){               //Números ordenados de forma descedente
        for(int i=numElem; i>0; i--){
            entrada[i] = i;
        }
    }
    //El siguiente es el caso especial donde se insertan de forma ascendente y luego descendente
    //(para evitar problemas, se ingresa el resto del código dentro del mismo condicional)
    if(strcmp("AD", command)==0){              //Caso especial: Ascendente y luego descendente
        for(int i=0; i<numElem; i++){
            entrada[i] = i;
        }
        for(int i=numElem; i>0; i--){
            entrada2[i] = i;
        }
        printf("Listo numeros!\n");
        avl_tree *root = NULL;

        printf("Insertando elementos...\n");
        //Inserta números
        for(int i = 0; i<numElem; i++){
            insertAVL(&root, entrada[i]);
        }
        printf("Borrando elementos...\n");
        //Borra números
        for(int i = 0; i<numElem; i++){
            root = deleteAVL(&root, entrada2[i]);
        }

        if(root == NULL)
           printf("Root es NULL\n");
        else
            printf("Error, Root no es NULL!\n");
        free(entrada);
        return 0;

    }
    //El siguiente es el caso especial donde se insertan de forma aleatoria (dos arreglos diferentes)
    //(para evitar problemas, se ingresa el resto del código dentro del mismo condicional)
    if(strcmp("RR", command)==0){              //Caso especial: Ascendente y luego descendente
        //Generador de números aleatorios
        for(int i=0; i<numElem; i++){
            entrada[i] = i;
        }
        for(int i=0; i<numElem; i++){
            int iRand = rand()% numElem;
            int aux = entrada[i];
            entrada[i] = entrada [iRand];
            entrada[iRand] = aux;
        }
        //Generador de números aleatorios
        for(int i=0; i<numElem; i++){
            entrada2[i] = i;
        }
        for(int i=0; i<numElem; i++){
            int iRand = rand()% numElem;
            int aux = entrada2[i];
            entrada2[i] = entrada2 [iRand];
            entrada2[iRand] = aux;
        }
        printf("Listo numeros!\n");
        avl_tree *root = NULL;

        printf("Insertando elementos...\n");
        //Inserta números
        for(int i = 0; i<numElem; i++){
            insertAVL(&root, entrada[i]);
        }
        printf("Borrando elementos...\n");
        //Borra números
        for(int i = 0; i<numElem; i++){
            root = deleteAVL(&root, entrada2[i]);
        }

        if(root == NULL)
           printf("Root es NULL\n");
        else
            printf("Error, Root no es NULL!\n");
        free(entrada);
        return 0;

    }
    
    printf("Listo numeros!\n");
    avl_tree *root = NULL;

    printf("Insertando elementos...\n");
    //Inserta números
    for(int i = 0; i<numElem; i++){
        insertAVL(&root, entrada[i]);
    }

    printf("Borrando elementos...\n");
    //Borra números 
    for(int i = 0; i<numElem; i++){
        root = deleteAVL(&root, entrada[i]);
    }

    if(root == NULL)
        printf("Root es NULL\n");
    else
        printf("Error, Root no es NULL!\n");
    free(entrada);
    return 0;
}
