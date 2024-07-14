//BJT Tree
#include <stdio.h>
#include <stdlib.h>

#define YES 1
#define NO 0

//Definición de la estructura
struct bst_node {
    int elem;
    struct bst_node *right;
    struct bst_node *left;
};

//Contador auxiliar para debugeo 
int count = 0;

//Definir tipo "bst_tree"
typedef struct bst_node bst_tree;   //Definimos de esta forma porque directamente no podemos usar definiciones de una estructura dentro de sí misma

//Función para encontrar un elemento. Será recursiva
//NOTA: Usa doble asterisco (**) cuando se busca modificar una variable
int findBST(bst_tree *n, int elem){
    //Con el siguiente condicional se acaba la recursividad (si es nulo, no se encuentra)
    if(n == NULL){               //NOTA: Es imporante colocar paréntesis para definir de dónde sacar un dato (segmentation fault cuando no se encuentra nada en una dirección de memoria)
        return NO;
    }
    //Si se encuentra el elemento (el elemento del nodo es igual al elemento buscado), regresa "YES"
    if(n->elem == elem){
        return YES; 
    }
    //Aquí llega la ejecución si aún no se encuentra el nodo
    //Entonces si el elemento del nodo es mayor al del elemento buscado, buscar en el nodo de la izquierda
    if(n->elem > elem){
        return findBST(n->left, elem); 
    }
    //Entonces si el elemento del nodo es menor al del elemento buscado, buscar en el nodo de la derecha
    //... o bien, para quitar ambigüedades, al final lo dejamos así (si llega hasta aquí la ejecución, se buscará en el nodo derecho)
    //if(n->elem < elem)                //Si ponemos esta línea de código como no comentario, debes agregar el siguiente del return que sigue
        return findBST(n->right, elem); 
    //printf("Hay algo muy raro, llegué hasta aquí");
}

//Función para insertar un elemento. Lo mandamos con ** para que en caso de que el nodo mande llamar malloc, se "use a sí mismo"
void insertBST(bst_tree **n, int elem){
    int aux = count++;
    //Si no existía ese nodo, lo creamos
    if((*n)==NULL){
        (*n) = (bst_tree*)malloc(sizeof(bst_tree)*1);
        //TO DO: VERIFICAR QUE JALÓ MALLOC
        (*n)->elem = elem;
        (*n)->right = NULL;
        (*n)->left = NULL;
        return;
    }
    //Si ya estaba insertado, regresa y no se inserta nada 
    if( (*n)->elem == elem){
        return;
    }
    if( (*n)->elem > elem){
        insertBST(&((*n)->left), elem);
        return;
    }
    if( (*n)->elem < elem){
        insertBST(&((*n)->right), elem);
        return;
    }
    printf("Hay algo muy raro!\n");
}

//Función para encontrar la dirección del valor mínimo
bst_tree *bst_find_min(bst_tree *n){
    //Esta línea rompe con la recursvidad 
    if( n == NULL){
        return NULL;
    }
    else if(n->left == NULL){
        return n;                          //Si eres el más chico, regresa n
    }
    else{
        return bst_find_min( n->left );    //Si aún no eres el más chico, sigue bajando en los lados izquierdos
    }
}

//Función para borrar elementos
int deleteMin();

//Función para borrar elementos
//NOTA: "x" es el elemento que queremos borrar; "n" es el nodo "POV"
bst_tree *bst_delete(int x, bst_tree *n){
    bst_tree *tmp_node;                 //Nodo temporal
    bst_tree *child;
    //El siguiente IF es para un caso donde se quiera borrar un elemento en un arbol nulo (no se va a poder)
    if(n==NULL){
        printf("Element %d not found\n", x);
        return NULL;
    }
    //Aquí la ejecución llega si el árbol no era nulo
    if(x < n->elem){
        n->left = bst_delete(x, n->left);
        return n;
    }
    //
    else if(x > n->elem){
        n->right = bst_delete(x, n->right);
        return n;    
    }
                                    /*Si llega aquí la ejecución, entonces se llegó al elemento  (x)*/
    if(n->left && n->right){
        tmp_node = bst_find_min(n->right);
        n->elem = tmp_node->elem;
        //Recursividad
        n->right = bst_delete(n->elem, n->right);
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
    /*Si llega hasta aquí la ejecución, 'it was a leaf, child will be NULL'*/
    return n;
    //TO-DO: Usar un OR para el caso donde no haya ningún hijo

}

//MAIN

int main(){
    char buffer[100];
    printf("Ingrese R para números aleatorios, A para ascedentes, D para descendentes, AD para ascendentes-descendentes seguido de la cantidad deseada: ");
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
	bst_tree *root = NULL;

	printf("Insertando elementos...\n");
	//Inserta números
	for(int i = 0; i<numElem; i++){
	    insertBST(&root, entrada[i]);
	}
	printf("Borrando elementos...\n");
	//Borra números
	for(int i = 0; i<numElem; i++){
	    root = bst_delete(entrada2[i], root);
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
	bst_tree *root = NULL;

	printf("Insertando elementos...\n");
	//Inserta números
	for(int i = 0; i<numElem; i++){
	    insertBST(&root, entrada[i]);
	}
	printf("Borrando elementos...\n");
	//Borra números
	for(int i = 0; i<numElem; i++){
	    root = bst_delete(entrada2[i], root);
	}

	if(root == NULL)
	   printf("Root es NULL\n");
	else
	    printf("Error, Root no es NULL!\n");
	free(entrada);
	return 0;

    }
    printf("Listo numeros!\n");
    bst_tree *root = NULL;

    printf("Insertando elementos...\n");
    //Inserta números
    for(int i = 0; i<numElem; i++){
        insertBST(&root, entrada[i]);
    }

    printf("Borrando elementos...\n");
    //Borra números 
    for(int i = 0; i<numElem; i++){
        root = bst_delete(entrada[i], root);
    }

    if(root == NULL)
        printf("Root es NULL\n");
    else
        printf("Error, Root no es NULL!\n");
    free(entrada);
    return 0;
}
