#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This file is to test structures and functions

// this is the head of the bigNum.
// numbers in the chained list should be reversed
// ex: 100 234 => 4->3->2->0->0->1->NULL
struct grand_entier {
  int negatif;
  struct cellule *chiffres;
};

struct cellule {
  char chiffre;
  struct cellule *suivant;
}  


