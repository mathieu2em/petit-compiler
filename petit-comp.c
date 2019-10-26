/* fichier: "petit-comp.c" */

/* Un petit compilateur et machine virtuelle pour un sous-ensemble de C.  */
// TODO dealer avec les NULL des malloc pour eviter le festival de st-tite du SEGFAULT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "petit-comp.h"

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
  CELL *cell = bn->chiffres;
  if(cell == NULL){
    return bn;
  } else if(cell->suivant==NULL){
    bn->chiffres = NULL;
    return bn;
  } else {
    while(cell->suivant->suivant != NULL){
	cell = cell->suivant;
      }
      cell->suivant = NULL;
      free(cell->suivant);
      return bn;
  }
}

// on veut une fonction qui initialise un nouveau bignum dans la memoire.
// il l'initialise avec aucun caractere a l'interieur.
// TODO pas certain si on devrais utiliser le * ici ou pas dans la declar et return
BIG_NUM *new_big_num()
{
  BIG_NUM *bn = malloc(sizeof(BIG_NUM));
  bn->negatif = 0;
  bn->chiffres = NULL;
  return bn;
}
// rajoute un caractere a un bignum existant 
BIG_NUM *bn_new_num(BIG_NUM *bn, char k)
{
  CELL *cell = new_cell(k, bn->chiffres);
  bn->chiffres = cell;
  return bn;
}
// verify if big_num has zeros at the start and if so 
BIG_NUM *bn_verif_correc_zero(BIG_NUM *bn)
{
  while (bn_last_cell(bn)->chiffre=='0')
    {
      bn_pop(bn);
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
  //printf("test45\n");
  return bn;
}

void bn_print(BIG_NUM *bn)
{
  //printf("t1\n");
  CELL *c = bn->chiffres;
  //printf("t2\n");
  while(c!=NULL)
    {
      //printf("t3\n");
      printf("%c", c->chiffre);
      c = c->suivant;
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
  bn_print(a);
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
  bn_print(result);
  return result;
}
// divise le resultat du bignum par 10
BIG_NUM *bn_DIV(BIG_NUM *bn){
  BIG_NUM *result = new_big_num();
  if(bn->size==1){
    return result;
  } else {
    CELL *c = bn->chiffres->suivant;
    while(c != NULL){
      bn_new_num_reverse(result, c->chiffre);
      c = c->suivant;
    } 
  }
  return result;
}
// bignum modulo 10
BIG_NUM *bn_MOD(BIG_NUM *bn){
  if(bn->size==1){
    if(bn->chiffres==NULL){
      return new_big_num();
    } else {
      BIG_NUM *result = new_big_num();
      bn_new_num(result, bn->chiffres->chiffre);
      return result;
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
    return 1;
  }
}

/* Analyseur lexical. */

enum { DO_SYM, ELSE_SYM, IF_SYM, WHILE_SYM, LBRA, RBRA, LPAR,
       RPAR, PLUS, MINUS, LESS, SEMI, EQUAL, INT, ID, EOI };

char *words[] = { "do", "else", "if", "while", NULL };

int ch = ' ';
int sym;
//int int_val;
BIG_NUM *big_num;
char id_name[100]; // 27?

void syntax_error() { fprintf(stderr, "syntax error\n"); exit(1); }

void next_ch() { ch = getchar(); }

void next_sym()
{
  while (ch == ' ' || ch == '\n') next_ch();
  switch (ch)
    { case '{': sym = LBRA;  next_ch(); break;
      case '}': sym = RBRA;  next_ch(); break;
      case '(': sym = LPAR;  next_ch(); break;
      case ')': sym = RPAR;  next_ch(); break;
      case '+': sym = PLUS;  next_ch(); break;
      case '-': sym = MINUS; next_ch(); break;
      case '<': sym = LESS;  next_ch(); break;
      case ';': sym = SEMI;  next_ch(); break;
      case '=': sym = EQUAL; next_ch(); break;
      case EOF: sym = EOI;   next_ch(); break;
      default:
        if (ch >= '0' && ch <= '9')
          {
            //int_val = 0; /* overflow? */

	    // on a un chiffre donc on initialise un bignum
	    big_num = new_big_num();

	    int count = 0;

            while (ch >= '0' && ch <= '9')
              {
                //int_val = int_val*10 + (ch - '0');
		// increment the counter of elements
		count++;
		//tant qu'il y a des chiffres on les rajoute au bignum
		// TODO faut voir comment le traiter maintenant quon a plus int_val
		bn_new_num(big_num, ch);
                next_ch();
              }
	    // verify special case 0
	    if (count == 1 && big_num->chiffres->chiffre == 0)
	      { // reset big num to NULL value which is 0
		big_num = new_big_num();
	      }
	    else
	      {
		// TODO pas certain que l'assignation est necessaire
		bn_verif_correc_zero(big_num);
	      }
	    big_num->size = count;
	    sym = INT;

          }
        else if (ch >= 'a' && ch <= 'z')//TODO jai limpression que ca va pas chercher la var comme il faut jai limpression que ca va chercher un mot complet. faut arranger ca
          {
            int i = 0; /* overflow? */
	    
            while ((ch >= 'a' && ch <= 'z') || ch == '_')
              {
                id_name[i++] = ch;
                next_ch();
              }

            id_name[i] = '\0';
            sym = 0;

            while (words[sym]!=NULL && strcmp(words[sym], id_name)!=0)
              sym++;

            if (words[sym] == NULL)
              {
                if (id_name[1] == '\0') sym = ID; else syntax_error();
              }
          }
        else syntax_error();
    }
}

/*---------------------------------------------------------------------------*/

/* Analyseur syntaxique. */

enum { VAR, CST, ADD, SUB, LT, ASSIGN,
       IF1, IF2, WHILE, DO, EMPTY, SEQ, EXPR, PROG };

struct node
  {
    union
    {
      int val;
      BIG_NUM *bn;
    };
    int kind;
    struct node *o1;
    struct node *o2;
    struct node *o3;
  };

typedef struct node node;

node *new_node(int k)
{
  node *x = malloc(sizeof(node));
  x->kind = k;
  return x;
}

node *paren_expr(); /* forward declaration */

node *term() /* <term> ::= <id> | <int> | <paren_expr> */
{
  node *x;

  if (sym == ID)           /* <term> ::= <id> */
    {
      x = new_node(VAR);
      x->val = id_name[0]-'a';
      next_sym();
    }
  else if (sym == INT)     /* <term> ::= <int> */
    {
      x = new_node(CST);
      x->bn = big_num; // TODO BIG_NUM
      bn_print(x->bn);
      next_sym();
    }
  else                     /* <term> ::= <paren_expr> */
    x = paren_expr();

  return x;
}

node *sum() /* <sum> ::= <term>|<sum>"+"<term>|<sum>"-"<term> */
{
  node *x = term();

  while (sym == PLUS || sym == MINUS)
    {
      node *t = x;
      x = new_node(sym==PLUS ? ADD : SUB);
      next_sym();
      x->o1 = t;
      x->o2 = term();
    }

  return x;
}

node *test() /* <test> ::= <sum> | <sum> "<" <sum> */
{
  node *x = sum();

  if (sym == LESS)
    {
      node *t = x;
      x = new_node(LT);
      next_sym();
      x->o1 = t;
      x->o2 = sum();
    }

  return x;
}

node *expr() /* <expr> ::= <test> | <id> "=" <expr> */
{
  node *x;

  if (sym != ID) return test();

  x = test();

  if (sym == EQUAL)
    {
      node *t = x;
      x = new_node(ASSIGN);
      next_sym();
      x->o1 = t;
      x->o2 = expr();
    }

  return x;
}

node *paren_expr() /* <paren_expr> ::= "(" <expr> ")" */
{
  node *x;

  if (sym == LPAR) next_sym(); else syntax_error();

  x = expr();

  if (sym == RPAR) next_sym(); else syntax_error();

  return x;
}

node *statement()
{
  node *x;

  if (sym == IF_SYM)       /* "if" <paren_expr> <stat> */
    {
      x = new_node(IF1);
      next_sym();
      x->o1 = paren_expr();
      x->o2 = statement();
      if (sym == ELSE_SYM) /* ... "else" <stat> */
        { x->kind = IF2;
          next_sym();
          x->o3 = statement();
        }
    }
  else if (sym == WHILE_SYM) /* "while" <paren_expr> <stat> */
    {
      x = new_node(WHILE);
      next_sym();
      x->o1 = paren_expr();
      x->o2 = statement();
    }
  else if (sym == DO_SYM)  /* "do" <stat> "while" <paren_expr> ";" */
    {
      x = new_node(DO);
      next_sym();
      x->o1 = statement();
      if (sym == WHILE_SYM) next_sym(); else syntax_error();
      x->o2 = paren_expr();
      if (sym == SEMI) next_sym(); else syntax_error();
    }
  else if (sym == SEMI)    /* ";" */
    {
      x = new_node(EMPTY);
      next_sym();
    }
  else if (sym == LBRA)    /* "{" { <stat> } "}" */
    {
      x = new_node(EMPTY);
      next_sym();
      while (sym != RBRA)
        {
          node *t = x;
          x = new_node(SEQ);
          x->o1 = t;
          x->o2 = statement();
        }
      next_sym();
    }
  else                     /* <expr> ";" */
    {
      x = new_node(EXPR);
      x->o1 = expr();
      if (sym == SEMI) next_sym(); else syntax_error();
    }

  return x;
}

node *program()  /* <program> ::= <stat> */
{
  node *x = new_node(PROG);
  next_sym();
  x->o1 = statement();
  if (sym != EOI) syntax_error();
  return x;
}

/*---------------------------------------------------------------------------*/

/* Generateur de code. */

enum { ILOAD, ISTORE, BIPUSH, DUP, POP, IADD, ISUB,
       GOTO, IFEQ, IFNE, IFLT, RETURN };

typedef long int code;

code object[1000], *here = object;

void gen(code c) { *here++ = c; } /* overflow? */ //rempli la pile de la MC

#ifdef SHOW_CODE
#define g(c) do { printf(" %d",c); gen(c); } while (0)
#define gi(c) do { printf("\n%s", #c); gen(c); } while (0)
#else
#define g(c) gen(c)
#define gi(c) gen(c)
#endif

void fix(code *src, code *dst) { *src = dst-src; } /* overflow? */

void c(node *x) //Premiere etape, cree un array avec la liste des operations
{ switch (x->kind)
    { case VAR   : gi(ILOAD); g(x->val); break;

    case CST   : gi(BIPUSH); g(x->bn); break; // TODO au lieu de changer val pour bn faire un union et une var qui dit quel aller chercher selon sam hyvon

      case ADD   : c(x->o1); c(x->o2); gi(IADD); break;

      case SUB   : c(x->o1); c(x->o2); gi(ISUB); break;

      case LT    : gi(BIPUSH); g(1);
                   c(x->o1);
                   c(x->o2);
                   gi(ISUB);
                   gi(IFLT); g(4);
                   gi(POP);
                   gi(BIPUSH); g(0); break;

      case ASSIGN: c(x->o2);
                   gi(DUP);
                   gi(ISTORE); g(x->o1->val); break;

      case IF1   : { code *p1;
                     c(x->o1);
                     gi(IFEQ); p1 = here++;
                     c(x->o2); fix(p1,here); break;
                   }

      case IF2   : { code *p1, *p2;
                     c(x->o1);
                     gi(IFEQ); p1 = here++;
                     c(x->o2);
                     gi(GOTO); p2 = here++; fix(p1,here);
                     c(x->o3); fix(p2,here); break;
                   }

      case WHILE : { code *p1 = here, *p2;
                     c(x->o1);
                     gi(IFEQ); p2 = here++;
                     c(x->o2);
                     gi(GOTO); fix(here++,p1); fix(p2,here); break;
                   }

      case DO    : { code *p1 = here; c(x->o1);
                     c(x->o2);
                     gi(IFNE); fix(here++,p1); break;
                   }

      case EMPTY : break;

      case SEQ   : c(x->o1);
                   c(x->o2); break;

      case EXPR  : c(x->o1);
                   gi(POP); break;

      case PROG  : c(x->o1);
                   gi(RETURN); break;
    }
}

/*---------------------------------------------------------------------------*/

/* Machine virtuelle. */

long int globals[26];

void run()
{
  long int stack[1000], *sp = stack; /* overflow? */ //pile
  code *pc = object;

  for (;;)
    switch (*pc++)
      {
      case ILOAD : *sp++ = globals[*pc++];             break;
      case ISTORE: globals[*pc++] = *--sp;             break;
      case BIPUSH: *sp++ = *pc++;                      break;
      case DUP   : sp++; sp[-1] = sp[-2];              break;
      case POP   : --sp;                               break;
      case IADD  : printf("sp[-2] = %d and &sp[-2] = %d\n", sp[-2], &sp[-2]);
	sp[-2] = bn_IADD((BIG_NUM *)sp[-2],(BIG_NUM *)sp[-1]);
	--sp;  break;//TODO changer IADD
      case ISUB  : sp[-2] = sp[-2] - sp[-1]; --sp;     break;//TODO changer ISUB
      case GOTO  : pc += *pc;                          break;
      case IFEQ  : if (*--sp==0) pc += *pc; else pc++; break;
      case IFNE  : if (*--sp!=0) pc += *pc; else pc++; break;
      case IFLT  : if (*--sp< 0) pc += *pc; else pc++; break;
      case RETURN: return;
      }
}

/*---------------------------------------------------------------------------*/

/* Programme principal. */

int main()
{
  int i;

  c(program());//Cree la liste des operations

#ifdef SHOW_CODE
  printf("\n");
#endif

  for (i=0; i<26; i++) // Initie la liste des variables globales
    globals[i] = 0;

  run(); //Fait les operations etapes par etapes selon la pile cree dans c(programs())

  for (i=0; i<26; i++)//TODO doit enlever eventuellement
    if (globals[i] != 0)
      {
	printf("%c = %d\n", 'a'+i, globals[i]);
	bn_print((BIG_NUM *)&globals[i]);
      }
  return 0;
}

/*---------------------------------------------------------------------------*/

//Algo de petite ecole
//[12,23,ADD] Pile
//s[courant] == ADD
//sp[-2] = 12 (pointeur vers un GE)
//sp[-1] = 23 ((pointeur vers un GE)
//TODO il y a une meilleur de le faire avec quelque chose de recursif
//BIG_NUM addition()
//{
//  BIG_NUM bn1 = sp[-2];//En attribu? direct?
//  BIG_NUM bn2 = sp[-1];
//  
//  BIG_NUM resultat;
// 
//  int chiffre1;
//  int chiffre2;
//  int ch_re;
//  CELL *cell1 = bn1.chiffres;
//  CELL *cell2 = bn2.chiffres;
// 
//  //Si un des num est 0
//  if(cell1 == NULL){
//    return bn2;
//  }else if(cell2 == NULL){
//    return bn1;
//  }
//  
//  int reste = 0;
//  
//  while(cell1 != NULL){
//    if(cell2 != NULL){
//      chiffre1 = char_to_int(cell1->chiffre);
//      chiffre2 = char_to_int(cell2->chiffre);
// 
//      add_big_num(resultat, chiffre1,chiffre2, reste);
//      
//      bn_new_num(resultat,ch_re);
//      cell1 = cell1->suivant;
//      cell2 = cell2->suivant;
//    }else{
//      chiffre1 = char_to_int(cell1->chiffre);
//      chiffre2 = 0;
//      
//      add_big_num(resultat, chiffre1,chiffre2, reste);
//      
//      cell1 = cell1->suivant;
//    }
//  }
//  while(cell2 != NULL){//Si le 2e chiffre est plus grand que le premier
//    chiffre1 = char_to_int(cell2->chiffre);
//    chiffre2 = 0;
//       
//    add_big_num(resultat, chiffre1,chiffre2, reste);
//    
//    cell2 = cell2->suivant;
//  }
//  if(reste == 1){//Si il restait une retenue
//     bn_new_num(resultat,ch_re);
//  }
//  return resultat;
//}//
//void add_big_num(BIG_NUM resultat, int chiffre1, int chiffre2, int reste){
//  int ch_re = chiffre1 + chiffre2 + reste;
//  if(ch_re >= 10){
//    reste = 1;
//    ch_re = ch_re % 10;//recupere le chiffre le moin significatif
//  }else{
//    reste = 0;
//  }
//  bn_new_num(resultat,ch_re);
//}//
