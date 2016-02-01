/*
 * Simple linked list con método de duplicación
 * Marco Antonio Chavez
 */
 
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

// FIN: Símbolo que indica el final de la lista de argumentos variables de la 
// función crea_lista. 
#define FIN -1 

struct nodo {
    int info;
    struct nodo* sig;
};

void duplica(struct nodo* p) {    
	struct nodo* temp = p;
	while(temp != NULL){
		struct nodo* dup = malloc(sizeof(struct nodo));			
		dup->info = temp->info;
		dup->sig  = temp->sig;
		
		temp->sig = dup;
		temp = temp->sig->sig; 
	}
	
}

void imprime(struct nodo* p) {
    bool primero = true;
    printf("(");
    while (p != NULL) {
        if (primero) {
            primero = false;
        } else {
            printf(", ");
        }
        printf("%d", p -> info);
        p = p -> sig;
    }
    printf(")\n");    
}

void agrega(int x, struct nodo** pp) {    
    struct nodo* nuevo = malloc(sizeof(struct nodo));    
    nuevo -> info = x;
    nuevo -> sig = NULL;    
    if (*pp == NULL) {
        *pp = nuevo;        
    } else {        
        struct nodo* p = *pp;        
        while (p -> sig != NULL) {
            p = p -> sig;
        }        
        p -> sig = nuevo;
    }
}

void crea_lista(struct nodo** pp, int x, ...) {    
    va_list args;    
    va_start(args, x);
    while (x != FIN) {
        agrega(x, pp);
        x = va_arg(args, int);
    } 
    va_end(args);
}

int main(void) {
    struct nodo* lst1 = NULL;
    struct nodo* lst2 = NULL;
    
    crea_lista(&lst1, 1, 2, 3, 4, FIN);
    imprime(lst1);
    duplica(lst1);    
    imprime(lst1);
    
    crea_lista(&lst2, 4, 8, 15, 16, 23, 42, FIN);
    imprime(lst2);
    duplica(lst2);    
    imprime(lst2);

    return EXIT_SUCCESS;
}
