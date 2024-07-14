//SPLAY Tree

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

//Banderas auxiliares para la inserción de nuevos elementos
int right_flag = 0;
int left_flag = 0;

struct avl_node {
    int elem;                       //Elemento (entero)
    int height;                     //Altura (de abajo hacia arriba) ->la altura de un nodo que no existe es -1
    struct avl_node *right;         //Hijo derecho
    struct avl_node *left;          //Hijo izquierdo
};
typedef struct avl_node avl_tree;

struct splay_node {
    int elem;                       //Elemento (entero)
    int height;                     //Altura (de abajo hacia arriba) ->la altura de un nodo que no existe es -1
    struct splay_node *right;       //Hijo derecho
    struct splay_node *left;        //Hijo izquierdo
    struct splay_node *father;      //Padre
};
typedef struct splay_node splay_tree;

//Función para una rotación simple 
splay_tree *rotation(splay_tree *n, int type){         //void rotation(avl_tree **node, enum rotationType type){
    //avl_tree *n = *node;
    if(type == LL){     //Esto también puede implementarse con un switch case
        //Aquí tenemos un caso LEFT - LEFT
        //   Se sigue la nomenclatura de la diapositiva 78
        splay_tree *Y = n->left->right;
        splay_tree *k2 = n;
        n = n->left;
        n->right = k2;
        n->right->left = Y;
        n->right->height--;
    }
    if(type == RR){
        //Aquí tenemos un caso RIGHT - RIGHT
        //   Se sigue la nomenclatura de la diapositiva 78
        splay_tree *Y = n->right->left;
        splay_tree *k1 = n;
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

//Función para determinar el máximo entre dos números
int max(int a, int b){
    //Si a es mayor a b, regresa a. Si no, regresa b.
    return a > b ? a : b;
}

//Función para encontrar la dirección del valor máximo
splay_tree *splay_find_max(splay_tree *n){
    //Esta línea rompe con la recursvidad 
    if( n == NULL){
        return NULL;
    }
    else if(n->right == NULL){
        return n;                          //Si eres el más chico, regresa n
    }
    else{
        return splay_find_max( n->right );    //Si aún no eres el más chico, sigue bajando en los lados izquierdos
    }
}

//Función para splaying
splay_tree *splaying(splay_tree *n){
    while(n->father!=NULL){     //NOTA: También puede ser cuando n->Father == NULL
        splay_tree *the_grandfather = n->father->father;
        splay_tree *the_father = n->father;
        //Si el nodo no tiene abuelo, quiere decir que sólo le falta un nivel para llegar a la raíz
        if (the_grandfather == NULL){
            if(the_father->left == n){
                the_father->left = n->right;
                if(n->right!=NULL)
                    n->right->father = the_father;
                n->right=the_father;
                //n = the_father;
                n->father = NULL;
                the_father->father = n;
                break;
            }
            if(the_father->right == n){
                the_father->right = n->left;
                if(n->left!=NULL)
                    n->left->father = the_father;
                n->left=the_father;
                //n = the_father;
                n->father = NULL;
                the_father->father = n;
                break;
            }
            //Si el nodo actual está a la izquierda del padre, se coloca al padre a la derecha del hijo
            if(the_father->left!=NULL){
                n->right=the_father;
                //n = the_father;
                n->father = NULL;
                the_father->father = n;
                break;
            }
            //Si el nodo actual está a la derecha del padre, se coloca al padre a la izquierda del hijo
            if(the_father->right!=NULL){
                n->left=the_father;
                //n = the_father;
                n->father = NULL;
                the_father->father = n;
                break;
            }
        }
        //Si llegamos hasta aquí, entonces sí tiene abuelo el nodo en cuestión. 
        //LEFT ZIG-ZIG
        if(the_father->left!=NULL&&the_grandfather->left!=NULL)
        if(n->elem==the_father->left->elem && the_father->elem==the_grandfather->left->elem){
            //Siguiendo las figuras de la última página del documento de instrucciones...
            splay_tree *x = n;
            splay_tree *p = the_father;
            splay_tree *g = the_grandfather;
            splay_tree *B = n->right;
            splay_tree *C = p->right;
            splay_tree *last_link;
            the_father->left = B;
            if(n->right!=NULL)
                n->right->father = the_father;
            the_grandfather->left = C;
            if(the_father->right!=NULL)
                the_father->right->father = the_grandfather;
            n->right = p;
            the_father->right = g;
            n->father = the_grandfather->father;
            //Añadimos el caso donde el grandfather tiene papá también
            splay_tree *the_grand_grandfather= the_grandfather->father;
            if(the_grand_grandfather!=NULL){
                if(the_grand_grandfather->left != NULL && the_grand_grandfather->left->elem == the_grandfather->elem){
                    the_grand_grandfather->left = n;
                }
                else{
                    if(the_grand_grandfather->right != NULL && the_grand_grandfather->right->elem == the_grandfather->elem)
                        the_grand_grandfather->right = n;
                }
            }
            p->father = n;
            g->father = p;
            the_grandfather = x;
            n = the_grandfather;
            continue;
        }
        //RIGHT ZIG-ZIG
        if(the_father->right!=NULL&&the_grandfather->right!=NULL)
        if(n->elem==the_father->right->elem && the_father->elem==the_grandfather->right->elem){
            //Siguiendo las figuras de la última página del documento de instrucciones...
            splay_tree *x = n;
            splay_tree *p = the_father;
            splay_tree *g = the_grandfather;
            splay_tree *B = p->left;
            splay_tree *C = n->left;
            the_grandfather->right = B;
            if(the_father->left!=NULL)
                the_father->left->father = the_grandfather;
            the_father->right = C;
            if(n->left!=NULL)
                n->left->father = the_father;
            n->left = p;
            the_father->left = g;
            n->father = the_grandfather->father;
            //Añadimos el caso donde el grandfather tiene papá también
            splay_tree *the_grand_grandfather= the_grandfather->father;
            if(the_grand_grandfather!=NULL){
                if(the_grand_grandfather->left != NULL && the_grand_grandfather->left->elem == the_grandfather->elem){
                    the_grand_grandfather->left = n;
                }
                else{
                    if(the_grand_grandfather->right != NULL && the_grand_grandfather->right->elem == the_grandfather->elem)
                        the_grand_grandfather->right = n;
                }
            }
            p->father = n;
            g->father = p;
            the_grandfather = x;
            n = the_grandfather;
            continue;
        }
        //LEFT ZIG-ZAG
        if(the_father->right!=NULL&&the_grandfather->left!=NULL)
        if(n->elem==the_father->right->elem && the_father->elem==the_grandfather->left->elem){
            //Siguiendo las figuras de la última página del documento de instrucciones del proyecto 3...
            splay_tree *x = n;
            splay_tree *p = the_father;
            splay_tree *g = the_grandfather;
            splay_tree *B = n->left;
            splay_tree *C = n->right;
            the_father->right = B;
            if(n->left!=NULL)
                n->left->father = the_father;
            the_grandfather->left = C;
            if(n->right!=NULL)
                n->right->father = the_grandfather;
            n->right = g;
            n->left = p;
            n->father = the_grandfather->father;
            //Añadimos el caso donde el grandfather tiene papá también
            splay_tree *the_grand_grandfather= the_grandfather->father;
            if(the_grand_grandfather!=NULL){
                if(the_grand_grandfather->left != NULL && the_grand_grandfather->left->elem == the_grandfather->elem){
                    the_grand_grandfather->left = n;
                }
                else{
                    if(the_grand_grandfather->right != NULL && the_grand_grandfather->right->elem == the_grandfather->elem)
                        the_grand_grandfather->right = n;
                }
            }
            p->father = n;
            g->father = n;
            the_grandfather = x;
            n = the_grandfather;
            continue;
        }
        //RIGHT ZIG-ZAG
        if(the_father->left!=NULL&&the_grandfather->right!=NULL)
        if(n->elem==the_father->left->elem && the_father->elem==the_grandfather->right->elem){
            //Siguiendo las figuras de la última página del documento de instrucciones...
            splay_tree *x = n;
            splay_tree *p = the_father;
            splay_tree *g = the_grandfather;
            splay_tree *B = n->left;
            splay_tree *C = n->right;
            the_grandfather->right = B;
            if(n->left!=NULL)
                n->left->father = the_grandfather;
            the_father->left = C;
            if(n->right!=NULL)
               n->right->father = the_father;
            n->right = p;
            n->left = g;
            n->father = the_grandfather->father;
            //Añadimos el caso donde el grandfather tiene papá también
            splay_tree *the_grand_grandfather= the_grandfather->father;
            if(the_grand_grandfather!=NULL){
                if(the_grand_grandfather->left != NULL && the_grand_grandfather->left->elem == the_grandfather->elem){
                    the_grand_grandfather->left = n;
                }
                else{
                    if(the_grand_grandfather->right != NULL && the_grand_grandfather->right->elem == the_grandfather->elem)
                        the_grand_grandfather->right = n;
                }
            }
            p->father = n;
            g->father = n;
            the_grandfather = x;
            n = the_grandfather;
            continue;
        }
        
    }
    return n;
}

//Función insert NO recursivo
//Se busca que la función NO se mande llamar recursivamente
splay_tree *insertSPLAY_int(splay_tree *n, int elem){
    //Se guarda el valor de la raíz
    splay_tree *root = n;
    //Variable auxilar para guardar el nodo anterior
    splay_tree *previous_node = NULL;

    //En el siguiente While, se hará un barrido de arriba hacia abajo hasta llegar a un nodo nulo para insertar el nuevo elemento
    while(n!=NULL){
        //Moverse a la izquierda
        if(n->elem > elem){
            previous_node = n;
            n = n->left;
            left_flag = YES;
            right_flag = NO;
            continue;
            
        }
        //Moverse a la derecha
        if(n->elem < elem){
            previous_node = n;
            n = n->right;
            left_flag = NO;
            right_flag = YES;
            continue;
        }
    }
    n = (splay_tree*)malloc(sizeof(splay_tree)*1);
    if(n==NULL){
        fprintf(stderr, "Cannot allocate memory for SPLAY node!\n");
        exit(1);
    }
    n->elem = elem;
    n->left = NULL;
    n->right = NULL;
    n->father = previous_node;
    if(previous_node!=NULL){
        if(left_flag==YES){
            previous_node->left = n;
        }
        else{
            previous_node->right = n;
        }
    }
    //Proceso de splaying
    n = splaying(n);
    return n;
}
//Función insert interno
//Se busca que la función se mande llamar internamente
void insertSPLAY(splay_tree **n, int elem){
    *n = insertSPLAY_int(*n, elem);
}

/*Función No recursiva para buscar un elemento en el árbol*/
int findSPLAY(splay_tree **node, int elem){
    splay_tree *n = *node;
    while(1){
        if(n == NULL){               //NOTA: Es imporante colocar paréntesis para definir de dónde sacar un dato (segmentation fault cuando no se encuentra nada en una dirección de memoria)
            return NO;
        }
        if(n->elem > elem){
            n = n->left;
            continue;
        }
        if(n->elem < elem){
            n = n->right;
            continue;
        }
        if(n->elem != elem){
            printf("Hay algo raro con el elemento a buscar\n");
            return NO; 
        }
        //Si la ejecución llega aquí, entonces ya se encontró al nodo. Se realiza pues un splaying
        break;
    }
    //Proceso de splaying
    n = splaying(n);
    *node = n;
    return YES;
}

/*Función para borrar un elemento*/
void deleteSPLAY(splay_tree **node, int elem){
    splay_tree *root = *node;
    splay_tree *n = *node;
    while(1){
        if(n == NULL){               //NOTA: Es imporante colocar paréntesis para definir de dónde sacar un dato (segmentation fault cuando no se encuentra nada en una dirección de memoria)
            break;
        }
        if(n->elem > elem){
            n = n->left;
            continue;
        }
        if(n->elem < elem){
            n = n->right;
            continue;
        }
        if(n->elem != elem){
            printf("Hay algo raro con el elemento a buscar\n");
            return; 
        }
        break;
    }
    //Splaying (una vez hallado el elemento a borrar)
    root = splaying(n);
    //Se guarda al hijo derecho del root
    splay_tree *right_child = root->right;
    //Se guardará ahora en n el valor más grande de los chicos (a la izquierda del nuevo root)
    n = splay_find_max(root->left);
    //Si no hubo valor mayor de los chicos, entonces directamente se borra el elemento y se coloca como raíz al hijo derecho
    if(n==NULL){
        if(root->right != NULL)
            root->right->father = NULL;
        *node = root->right;
        free(root);
        return;    
    }
    //Se marca al root como nulo con sus hijos
    root->left->father = NULL;
    if(root->right!=NULL)
        root->right->father = NULL;
    //Se borra al root (donde se encuentra el elemento a eliminar)
    free(root);
    //Se hace splay desde el elemento más grande de los chicos
    n = splaying(n);
    //Se conecta al hijo derecho del previo root al nuevo (SÓLO si el hijo derecho no es nulo)
    n->right = right_child;
    if(right_child!=NULL)
        right_child->father = n; 
    *node = n;
    return;
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
    //NOTA: El siguiente arreglo (entrada2) sólo se usa en los casos especiales
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
        splay_tree *root = NULL;

        printf("Insertando elementos...\n");
        //Inserta números
        for(int i = 0; i<numElem; i++){
            insertSPLAY(&root, entrada[i]);
        }
        printf("Borrando elementos...\n");
        //Borra números
        for(int i = 0; i<numElem; i++){
            deleteSPLAY(&root, entrada2[i]);
        }

        if(root == NULL)
           printf("Root es NULL\n");
        else
            printf("Error, Root no es NULL!\n");
        free(entrada);
        return 0;

    }
    //El siguiente es el caso especial donde se insertan de forma aleatoria-aleatoria (dos arreglos diferentes)
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
        splay_tree *root = NULL;

        printf("Insertando elementos...\n");
        //Inserta números
        for(int i = 0; i<numElem; i++){
            insertSPLAY(&root, entrada[i]);
        }
        printf("Borrando elementos...\n");
        //Borra números
        for(int i = 0; i<numElem; i++){
            deleteSPLAY(&root, entrada2[i]);
        }

        if(root == NULL)
           printf("Root es NULL\n");
        else
            printf("Error, Root no es NULL!\n");
        free(entrada);
        return 0;

    }
    printf("Listo numeros!\n");
    splay_tree *root = NULL;

    printf("Insertando elementos...\n");
    //Inserta números
    for(int i = 0; i<numElem; i++){
        insertSPLAY(&root, entrada[i]);
    }

    printf("Borrando elementos...\n");
    //Borra números 
    for(int i = 0; i<numElem; i++){
        deleteSPLAY(&root, entrada[i]);
    }

    if(root == NULL)
        printf("Root es NULL\n");
    else
        printf("Error, Root no es NULL!\n");
    free(entrada);
    return 0;
}       
