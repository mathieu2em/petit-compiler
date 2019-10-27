/* fichier: "petit-comp.c" */

/* Un petit compilateur et machine virtuelle pour un sous-ensemble de C.  */
// TODO dealer avec les NULL des malloc pour eviter le festival de st-tite du SEGFAULT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
/* Grands entiers Structure */

typedef struct grand_entier {
    int size; // number of elements in the bn
    int negatif;
    struct cellule *chiffres;
} BIG_NUM;

typedef struct cellule {
    char chiffre;
    struct cellule *suivant;
} CELL;

// function to transform char to int
int char_to_int(char c)
{
    return c - '0';
}
char int_to_char(int c)
{
    return c + '0';
}
CELL *new_cell(char k, CELL *next)
{
    //printf("new cell\n");
    CELL *cell = malloc(sizeof(CELL));
    cell->chiffre = k;
    cell->suivant = next;
    //printf("nc will ret\n");
    return cell;
}
// returns a pointer to last non-NULL cell of a big num
CELL *bn_last_cell(BIG_NUM *bn){
    //printf("testt\n");
    CELL *cell = bn->chiffres;
    if(cell == NULL){
        //printf("we have a prob\n");
        return bn->chiffres;
    }
    else if(cell->suivant==NULL) return cell;
    else {
        while(cell->suivant != NULL){
            cell = cell->suivant;
        }
        return cell;
    }
}
// pop the last non-NULL cell of a big num1
BIG_NUM *bn_pop(BIG_NUM *bn){
    if(bn->size==0){
        return bn;
    }
    CELL *cell = bn->chiffres;
    if(cell->suivant==NULL){
        bn->chiffres = NULL;
        bn->size = 0;
    } else {
        while(cell->suivant->suivant != NULL){
            cell = cell->suivant;
        }
        cell->suivant = NULL;
        free(cell->suivant);
        bn->size = (bn->size)-1;
    }
    return bn;
}

// on veut une fonction qui initialise un nouveau bignum dans la memoire.
// il l'initialise avec aucun caractere a l'interieur.
BIG_NUM *new_big_num()
{
    BIG_NUM *bn = malloc(sizeof(BIG_NUM));
    bn->negatif = 0;
    bn->chiffres = NULL;
    bn->size = 0;
    return bn;
}
// rajoute un caractere a un bignum existant
BIG_NUM *bn_new_num(BIG_NUM *bn, char k)
{
    CELL *cell = new_cell(k, bn->chiffres);
    bn->chiffres = cell;
    bn->size=(bn->size)+1;
    return bn;
}
// verify if big_num has zeros at the start and if so
BIG_NUM *bn_verif_correc_zero(BIG_NUM *bn)
{
    while (bn_last_cell(bn)->chiffre=='0')
        {
            bn_pop(bn);
            bn->size = (bn->size)-1;
        }
    return bn;
}
// rajoute un caractere de facon inversee a un big num
BIG_NUM *bn_new_num_reverse(BIG_NUM *bn, char k){
    //printf("test43\n");
    CELL *cell = bn_last_cell(bn);
    //printf("test44\n");
    if(cell==NULL){
        bn->chiffres = new_cell(k, NULL);
    } else {
        cell->suivant = new_cell(k, NULL);
    }
        bn->size = (bn->size)+1;  
    //printf("test45\n");
    return bn;
}

void bn_print(BIG_NUM *bn)
{
  if(bn->negatif==1) printf("-");
    if(bn->size==0){
        printf("0");
    } else {
        //printf("t1\n");
        CELL *c = bn->chiffres;
        //printf("t2\n");
        while(c!=NULL)
            {
                //printf("t3\n");
                printf("%c", c->chiffre);
                c = c->suivant;
            }
    }
    printf("\n");
}

BIG_NUM *bn_IADD(BIG_NUM *a, BIG_NUM *b)
{
    //printf("bn_IADD\n");
    int restant = 0; // store le restant pour prochain calcul
    int temp_result = 0; // store the temporary add result
    CELL *ca = a->chiffres; // chiffre 1
    CELL *cb = b->chiffres; // chiffre 2
    BIG_NUM *result = new_big_num();
    //printf("cc1\n");
    //printf("%d %d",a,b);
    //bn_print(a);
    while(ca!=NULL || cb!=NULL)
        {
            //printf("cc2 %d %d %d\n", ((ca==NULL)?0:char_to_int(ca->chiffre)), ((cb==NULL)?0:char_to_int(cb->chiffre)), restant);
            temp_result = ((ca==NULL)?0:char_to_int(ca->chiffre))
                + ((cb==NULL)?0:char_to_int(cb->chiffre)) + restant;
            // verify if result is bigger than 10
            if (temp_result>=10){
                restant = temp_result/10;
            } else {
                restant = 0;
            }
            if (ca!=NULL) ca = ca->suivant;
            if (cb!=NULL) cb = cb->suivant;

            //printf("%c\n",int_to_char(temp_result));
            bn_new_num_reverse(result, int_to_char(temp_result));
        }
    //bn_print(result);
    return result;
}
// divise le resultat du bignum par 10
BIG_NUM *bn_DIV(BIG_NUM *bn){
  //printf("test1\n");
    BIG_NUM *result = new_big_num();
    //printf("bn->size = %d\n",bn->size);
    if(bn->size==0){
        printf("ERROR ZERO DIVISION");
        return 0;
    }
    if(bn->size==1){
        return result;
    } else {
      //printf("test2\n");
        CELL *c = bn->chiffres->suivant;
        while(c != NULL){
          //printf("%c\n",c->chiffre);
            bn_new_num_reverse(result, c->chiffre);
            c = c->suivant;
        }
    }
    return result;
}
// bignum modulo 10
BIG_NUM *bn_MOD(BIG_NUM *bn){
    if(bn->size==0){ // si la taille est 1 on retourne le chiffre
        return new_big_num();
    }
    BIG_NUM *result = new_big_num();
    return bn_new_num(result, bn->chiffres->chiffre);
}
// method returns a pointer to cell at position i
CELL *bn_get_cell(BIG_NUM *bn, int i){
    if(bn->size <= i){
        printf("ERROR bn_get_cell");
        return 0;
    } else {
        int count = 0;
        CELL *c = bn->chiffres;
        while(count++<i){
            c = c->suivant;
        }
        return c;
    }
}
// returns true if a is bigger than b else false
// doesnt check sign nor if bn is illegal
int bn_bigger(BIG_NUM *a, BIG_NUM *b){
    if(a->size > b->size) return 1;
    else if(a->size < b->size) return 0;
    else {
        int i = a->size;
        int val_a = 0;
        int val_b = 0;
        while(i-- > 0){
            val_a = char_to_int(bn_get_cell(a, i)->chiffre);
            val_b = char_to_int(bn_get_cell(b, i)->chiffre);
            if(val_a > val_b) return 1;
            else if(val_b > val_a) return 0;
        }
        return 2;
    }
}
// substract a - b
BIG_NUM *bn_ISUBB(BIG_NUM *a, BIG_NUM *b)
{
    int temp_result = 0; // store the temporary add result
    int retenue = 0;// pour indiquer la retenue
    int reverse = 0;// pour indiquer s'il y a une retenue

    CELL *ca = a->chiffres; // unite de a
    CELL *cb = b->chiffres; // unite de b
    BIG_NUM *result = new_big_num();

    // compare les 2 nombres.
    // si a==b, status == 2
    // si a>b, status == 1
    // si a<b, status == 0
    int status = bn_bigger(a,b);

    if(status == 2){// a == b => a-a=0
        return result;
    }else if(status == 0){ // a<b, on calcule b-a au lieu de a-b
        CELL *temp = ca;
        ca = cb;
        cb = temp;
        reverse = 1;
    }
    // etape de soustraction
    while(ca!=NULL || cb!=NULL){

        int ai = char_to_int(ca->chiffre);// nombre dans cellule a
        int bi = ((cb==NULL)?0:char_to_int(cb->chiffre));// nombre dans cellule b
        if(retenue == 1){
            if(ai == 0){// cas special pour les 0 avec retenue a leur gauche
                ai = 9;
            }else{
                retenue = 0;
                ai -= 1;
            }
        }
        if(ai < bi){// pour la retenue
            ai += 10;
            retenue = 1;
        }
        temp_result = ai - bi;
        bn_new_num_reverse(result,int_to_char(temp_result));
        if(ca!=NULL) ca = ca->suivant;
        if(cb!=NULL) cb = cb->suivant;
        if(reverse == 1) result->negatif = 1;//TODO verifier si tjrs bon signe
    }
    bn_verif_correc_zero(result);
    return result;
}

int main(void) {
  BIG_NUM *bn = new_big_num();
  BIG_NUM *bn2 = new_big_num();

  bn_new_num(bn2, '1');
  bn_new_num(bn2, '2');
  bn_new_num(bn2, '3');
  
  bn_new_num(bn, '0');
  bn_new_num(bn, '0');
  bn_new_num(bn, '1');
  bn_new_num(bn, '2');
  bn_new_num(bn, '3');
  printf("SHOULD BE 32100: ");
  bn_print(bn);
  bn_verif_correc_zero(bn);
  printf("SHOULD BE 321: ");
  bn_print(bn);

  BIG_NUM *bn7 = new_big_num();
  
  char c = bn->chiffres->suivant->chiffre;
  printf("TEST big num number at position 1 sb 2: %c\n", c);
  printf("\n");

  printf("TEST IADD :\nSHOULD BE 642: ");
  BIG_NUM *bn3 = bn_IADD(bn, bn2);
  printf("bn3: ");
  bn_print(bn3);
  printf("should be 0: ");
  bn_print(bn_IADD(bn7,bn7));
  printf("should be 321: ");
  bn_print(bn_IADD(bn7, bn2));
  printf("should be 321: ");
  bn_print(bn_IADD(bn2, bn7));
  printf("\n");

  printf("TEST ISUBB\n");
  BIG_NUM *bn4 = new_big_num();
  bn_new_num(bn4, '1');
  bn_new_num(bn4, '1');
  bn_new_num(bn4, '1');
  printf("bn4 sb 111: ");
  bn_print(bn4);
  BIG_NUM *bn5 = new_big_num();
  bn_new_num(bn5, '9');
  bn_new_num(bn5, '9');
  printf("bn5 sb 99: ");
  bn_print(bn5);
  printf("result should be 21: ");
  BIG_NUM *bn6 = bn_ISUBB(bn4,bn5);
  printf("bn6: ");
  bn_print(bn6);
    printf("result should be 0: ");
  bn_print(bn_ISUBB(bn7,bn7));
  printf("result should be -321: ");
  bn_print(bn_ISUBB(bn7, bn2));
  printf("result should be 321: ");
  bn_print(bn_ISUBB(bn2, bn7));
  printf("\n");

  printf("TEST bn_DIV\n");
  printf("result should be 21: ");
  bn_print(bn_DIV(bn2));
  printf("result should be 42: ");
  bn_print(bn_DIV(bn3));

  // test 
  return 0;
}