/* Merge Sort
 Marco Ch‡vez */
#include<stdlib.h>
#include<stdio.h>

int fusionar (int *arr_b, int i, int j, int* num_comp){
     
     int num_elem = (j-i)-1;
     int mitad_elem = (j-i)-1;
     i--;
     j--;
     num_elem +=j;
     mitad_elem += i;
     
     int arr_f[num_elem+1];
     int ind_af = 0;
     int ind_arr =i;
     while(i<= mitad_elem || j<=num_elem){
               
               if(arr_b[i]<=arr_b[j]){
                     arr_f[ind_af] = arr_b[i];
                     ind_af++;
                     i++;
               }else{
                     arr_f[ind_af] = arr_b[j];
                     ind_af++;
                     j++;
               }
               (*num_comp)++;
              
                      while(j>num_elem && i<= mitad_elem){
                               arr_f[ind_af] = arr_b[i];
                               i++;
                               ind_af++;
                      }
               
               
              
                      while(i>mitad_elem && j<=num_elem){
                            arr_f[ind_af] = arr_b[j];
                            ind_af++;
                            j++;         
                      }
     }
     ind_af=0;
     printf("Numeros ingresados al merge:\n");
     int k;
     for(k=ind_arr; k<=num_elem; k++){
             printf("%i ",arr_b[k]);
     }
     
     printf("\nNumeros ordenados en el merge:\n");
     
     for(k=ind_arr; k<=num_elem; k++){
             arr_b[k]=arr_f[ind_af++];
             printf("%i ",arr_b[k]);
     }
     printf("\n-----------------\n");
     return ind_arr+1;
}

int merge(int *arr_b, int i, int j, int* num_comp){
     if (i==j)
        return i;
     else{
          int m=(i+j-1)/2;
          return fusionar(arr_b, merge(arr_b, i, m, num_comp) ,merge(arr_b, m+1, j, num_comp), num_comp);
     }
}

int main(){
    int arr_b[32];
    int limite = 32;
    int num_comp =0;
    
    int i;
    for(i=0; i<limite; i++){
            scanf("%i", (arr_b+i));        
    }
    
    int val = merge(arr_b,1,limite,&num_comp);
    
    printf("\nNumeros Ordenados:\n");
    for(i=0; i<limite; i++){
            printf("%i ", arr_b[i]);        
    }
    printf("\nComparaciones totales: %i\n", num_comp);
    system("pause");
}
