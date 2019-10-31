/* fichier: "petit-comp.c" */

/* Un petit compilateur et machine virtuelle pour un sous-ensemble de C.  */
// TODO dealer avec les NULL des malloc pour eviter le festival de st-tite du SEGFAULT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "petit-comp.h" // TODO enlever a la remise

/*---------------------------------------------------------------------------*/
/* Grands entiers Structure */

typedef struct grand_entier {
  //int size; // number of elements in the bn
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
CELL *bn_last_cell(BIG_NUM *bn)
{
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
int bn_get_size(BIG_NUM *bn)
{
    int count = 0;
    CELL *c = bn->chiffres;
    while(c!=NULL){
        count++;
        c = c->suivant;
    }
    return count;
}
// method returns a pointer to cell at position i
CELL *bn_get_cell(BIG_NUM *bn, int i)
{
    if(bn_get_size(bn) <= i){
        printf("ERROR bn_get_cell");
        return 0;
    }
    int count = 0;
    CELL *c = bn->chiffres;
    while((count++)<i){
        c = c->suivant;
    }
    return c;
}
// pop the last non-NULL cell of a big num1
BIG_NUM *bn_pop(BIG_NUM *bn)
{
    if(bn_get_size(bn)==0){
        return bn;
    }
    CELL *cell = bn->chiffres;
    if(cell->suivant==NULL){
        bn->chiffres = NULL;
    } else {
        while(cell->suivant->suivant != NULL){
            cell = cell->suivant;
        }
        cell->suivant = NULL;
        free(cell->suivant);
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
BIG_NUM *bn_new_num_reverse(BIG_NUM *bn, char k)
{
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
// prints the big num in the order of the chained list 
void bn_print(BIG_NUM *bn)
{
    if(bn->negatif==1) printf("-");
    if(bn_get_size(bn)==0) printf("0");
    else {
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
// print the big num in the order of human reading sens
void bn_print_right(BIG_NUM *bn)
{
    if(bn->negatif==1) printf("-");
    if(bn_get_size(bn)==0) printf("0");
    else {
        //printf("t1\n");
        int i = bn_get_size(bn);
        CELL *c = bn_get_cell(bn, --i);
        //printf("t2\n");
        while(i>=0)
            {
                //printf("t3\n");
                printf("%c", c->chiffre);
                c = bn_get_cell(bn, --i);
            }
    }
    printf("\n");
}
// adds two big nums
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
            //printf("%d\n", temp_result);
            // verify if result is bigger than 10
            if (temp_result>=10){
                restant = temp_result/10;
                temp_result = temp_result%10;
            } else {
                restant = 0;
            }
            if (ca!=NULL) ca = ca->suivant;
            if (cb!=NULL) cb = cb->suivant;

            //printf("%c\n",int_to_char(temp_result));
            bn_new_num_reverse(result, int_to_char(temp_result));
        }
    if(restant!=0) bn_new_num_reverse(result, int_to_char(restant));
    //bn_print(result);
    return result;
}
// divide big_num result by 10
BIG_NUM *bn_DIV(BIG_NUM *bn)
{
    BIG_NUM *result = new_big_num();
    if(bn_get_size(bn)==0){
        printf("ERROR ZERO DIVISION");
        return 0;
    }
    if(bn_get_size(bn)==1){
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
BIG_NUM *bn_MOD(BIG_NUM *bn)
{
    if(bn_get_size(bn)==0){ // si la taille est 1 on retourne le chiffre
        return new_big_num();
    }
    BIG_NUM *result = new_big_num();
    return bn_new_num(result, bn->chiffres->chiffre);
}
// returns true if a is bigger than b else false
// doesnt check sign nor if bn is illegal
int bn_bigger(BIG_NUM *a, BIG_NUM *b)
{
    int size_a = bn_get_size(a);
    int size_b = bn_get_size(b);
    if(size_a > size_b) return 1;
    else if(size_a < size_b) return 0;
    else {
        int i = size_a;
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
// multiply a big num by an int
BIG_NUM *bn_int_mult(int a, BIG_NUM *b)
{
  CELL *c = b->chiffres;

  int result;
  int retenue = 0;

  BIG_NUM *bn = new_big_num();

  while(c!=NULL){
    int temp_result = a*char_to_int(c->chiffre)+retenue;
    if (temp_result > 9){
      result = temp_result%10;
      retenue = temp_result/10;
    } else {
      result = temp_result;
      retenue = 0;
    }
    bn_new_num_reverse(bn, int_to_char(result));
    c = c->suivant;
  }
  return bn;
}
// multiply two big nums
BIG_NUM *bn_mult(BIG_NUM *a, BIG_NUM *b)
{
  CELL *c = a->chiffres;
  int count = 1; // numbers of zero to add
  BIG_NUM *flusher;

  BIG_NUM *temp_bn = bn_int_mult(char_to_int(c->chiffre), b);// first big num
  c = c->suivant; // go get next int
  while(c!=NULL){
    BIG_NUM *temp_bn2 = bn_int_mult(char_to_int(c->chiffre), b);
    int i=0;
    while(i<count){
      bn_new_num(temp_bn2, '0');
      i++;
    }
    count++;
    flusher = temp_bn;
    temp_bn = bn_IADD(temp_bn, temp_bn2);
    free(flusher);
    c = c->suivant;
  }
  return temp_bn;
}
// verifies if big num == 10
// if so returns true
int bn_verif_10(BIG_NUM *bn)
{
  CELL *c = bn->chiffres;
  if(c == NULL
     || c->chiffre!=0
     || c->suivant == NULL
     || c->suivant->chiffre!=1){
    return 0;
  } else {
    return 1;
  }
}
/* Analyseur lexical. */

enum { DO_SYM, ELSE_SYM, IF_SYM, WHILE_SYM, LBRA, RBRA, LPAR,
       RPAR, PLUS, MINUS, MULT, LESS, DIV, MOD, SEMI, EQUAL, INT, ID, EOI };

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
        case '*': sym = MULT;  next_ch(); break;
        case '/': sym = DIV;   next_ch(); break;
        case '%': sym = MOD;   next_ch(); break;
        case '<': sym = LESS;  next_ch(); break;
        case ';': sym = SEMI;  next_ch(); break;
        case '=': sym = EQUAL; next_ch(); break;
        case EOF: sym = EOI;   next_ch(); break;
        default:
            if (ch >= '0' && ch <= '9'){
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
                //TODO maybe erase this comm : big_num->size = count; done in the add num
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

enum { VAR, CST, ADD, SUB, MUL, DIVI, MODU, LT, ASSIGN,
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
            x->bn = big_num;
            bn_print(x->bn);
            next_sym();
        }
    else                     /* <term> ::= <paren_expr> */
        x = paren_expr();

    return x;
}

node *sum() /* <sum> ::= <term>|<sum>"+"<term>|<sum>"-"<term>|<sum>"*"<term>|
             <sum>"/"<term>|<sum>"%"<term>*/
// TODO on va devoir verifier si ca marche
{
    node *x = term();

    while (sym == PLUS || sym == MINUS || sym == MULT)
        {
            node *t = x;
            if(sym==DIV||sym==MOD){
              x = new_node(sym==DIV ? DIVI : MODU);
              next_sym();
              x->o1 = t;
              x->o2 = term();
              if(x->o2->kind==CST){
                if(bn_verif_10(x->o2->bn)){
                  // TODO TRAITER SYNTAX ERROR
                  exit(1); // pas sure lol
                }
              } else {
                // TODO TRAITER SYNTAX ERROR
                exit(1); // pas sure lol
              }
            }else{
              x = new_node(sym==PLUS ? ADD : (sym==MINUS ? SUB : MUL));
              next_sym();
              x->o1 = t;
              x->o2 = term();
            }
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

enum { ILOAD, ISTORE, BIPUSH, DUP, POP, IADD, ISUB, IMULT, IDIV, IMOD,
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

        case DIVI   : c(x->o1); c(x->o2); gi(IDIV); break;

        case MODU   : c(x->o1); c(x->o2); gi(IMOD); break;

        case MUL   : c(x->o1); c(x->o2); gi(IMULT); break;

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
            case IADD  : sp[-2] = bn_IADD((BIG_NUM *)sp[-2],(BIG_NUM *)sp[-1]);
                --sp;  break;
            case ISUB  : sp[-2] = bn_ISUBB((BIG_NUM *)sp[-2],(BIG_NUM *)sp[-1]);
              --sp; break;
            case IMULT : sp[-2] = bn_mult((BIG_NUM *)sp[-2],(BIG_NUM *)sp[-1]);
              --sp; break;
            case IDIV  : sp[-2] = bn_DIV((BIG_NUM *)sp[-2]);//TODO Cest peut etre sp[-1]
              --sp; break;
            case IMOD  : sp[-2] = bn_MOD((BIG_NUM *)sp[-2]);//TODO same
              --sp; break;
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
                printf("%c = ", 'a'+i);
                bn_print_right((BIG_NUM *)globals[i]);
            }
    return 0;
}

/*---------------------------------------------------------------------------*/
