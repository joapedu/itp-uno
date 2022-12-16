#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numAlloc = 0, numCard = 0, numBots = 0;
char *players[11];

typedef struct {
  int ativa;
  char num[3];
  char naipe[4];
} card; //definindo card 

card *hand; //card recebe ponteiro da mão
card mesa;

void maoinicial() {
  hand = (card *)malloc(10 * sizeof(card));
  numAlloc = 10;
} //função determina a mão atual

void insertInHand(char *newCard) {
  if (numCard == numAlloc) {
    numAlloc *= 2;
    hand = (card *)realloc(hand, numAlloc * sizeof(card));
  }
  int posNaipe = 1;
  if (newCard[0] == '1') {
    posNaipe = 2;
  }
  strcpy(hand[numCard].naipe, &newCard[posNaipe]);
  newCard[posNaipe] = 0;
  strcpy(hand[numCard].num, newCard);
  hand[numCard].ativa = 1;
  numCard++;
}

int main() {
  setbuf(stdin, NULL);   
  setbuf(stdout, NULL); 
  setbuf(stderr, NULL);

  char temp[100], meuId[10], temp2[100], handtemp[100], tabletemp[10], coringatemp[100], valetetemp[100], buytemp[10];
  char *pedaco, *carta;

  scanf("%*s %[^\n]", temp);

  pedaco = strtok(temp, " ");
  while (pedaco != NULL) {
    players[numBots] = strdup(pedaco);
    numBots++;
    pedaco = strtok(NULL, " ");
  }

  scanf("%*s %s", meuId);

  maoinicial();

  scanf("%*s [ %[^]] ]", handtemp);

  carta = strtok(handtemp, " ");  // separar as cartas
  while (carta != NULL) {
    insertInHand(carta);
    carta = strtok(NULL, " ");
  }

  scanf("%*s %s", tabletemp);

  int posNaipe = 1;
  if (tabletemp[0] == '1') {
    posNaipe = 2;
  }
  strcpy(mesa.naipe, &tabletemp[posNaipe]);
  tabletemp[posNaipe] = 0;
  strcpy(mesa.num, tabletemp); //copia tanto o numero da carta quanto o naipe da carta para tabletemp

  char turnId[10], acao[15], comp1[5], comp2[5], discard[8] = "DISCARD", buy[4]="BUY";

  while (1) {
    while (1) {
      scanf("%*s %s", turnId);
      if (strcmp(meuId, turnId) == 0) {
        break;
      } //verificando se não é meuid para não jogar 
      scanf("%s %s", acao, comp1);
      if (strcmp(acao, discard) == 0) {
        if (comp1[0] == 'A') {
          scanf("%s", comp2); //se comp1 for 'A' armazena em comp2
        }
        if (comp1[0] == 'C') {
          scanf("%s", comp2); //se comp1 for 'C' armazena em comp2
        }
        int posNaipe = 1;
        if (comp1[0] == '1') {
          posNaipe = 2;
        }
        strcpy(mesa.naipe, &comp1[posNaipe]); //verifica o naipe e armazena
        if (comp1[0] == 'A') {
          strcpy(mesa.naipe, comp2);  //copia para mesa o naipe atual 
        }
        if (comp1[0] == 'C') {
          strcpy(mesa.naipe, comp2);   //copia para mesa o naipe atual
        }
        comp1[posNaipe] = 0;
        strcpy(mesa.num, comp1);
      }
    }
    
    char discard[32];
    if (mesa.num[0] == 'C' || mesa.num[0] == 'V' && strcmp(acao, "DISCARD")==0) {
      if (mesa.num[0] == 'C') {
        printf("BUY 4\n");
        for (int i = 0; i < 4; i++) {
          scanf("%s", coringatemp);
          insertInHand(coringatemp);
        }   //caso a carta seja +4 (coringa) compre 4
      }
      else if (mesa.num[0] == 'V') {
        printf("BUY 2\n");
        for (int i = 0; i < 2; i++) {
          scanf("%s", valetetemp);
          insertInHand(valetetemp);
        }   //caso a carta seja +2 (vallete) compre 2
      }
    } 
    else {
      int jaJoguei = 0;
      for (int k = 0; k < numCard; k++) {
        if (hand[k].ativa == 1) {
            if (strcmp(hand[k].naipe, mesa.naipe) == 0) { //se for diferente apenas o naipe
              // se a carta que eu tenho em mãos for diferente do naipe 
              if (hand[k].num[0] == 'C' || hand[k].num[0] == 'A'){ 
                // eu jogo carta para mudar esse naipe (ou coringa ou Ás)
                printf("DISCARD %s%s ♦\n", hand[k].num, hand[k].naipe);
                jaJoguei = 1;
                hand[k].ativa = 0;
                strcpy(mesa.num, hand[k].num); 
                strcpy(mesa.naipe, hand[k].naipe);
                break;
              }
              
              else {
		            printf("DISCARD %s%s\n", hand[k].num, hand[k].naipe);
                jaJoguei = 1;
                hand[k].ativa = 0;
                strcpy(mesa.num, hand[k].num);
                strcpy(mesa.naipe, hand[k].naipe);
                break;
              }
            } 
            else if (strcmp(hand[k].num, mesa.num) == 0) {  //se for diferente agora o numero

              if (hand[k].num[0] == 'A' || hand[k].num[0] == 'C') {
		            printf("DISCARD %s%s ♦\n", hand[k].num, hand[k].naipe);
                jaJoguei = 1;
                hand[k].ativa = 0;
                strcpy(mesa.num, hand[k].num);
                strcpy(mesa.naipe, hand[k].naipe);
                break;
              } 
              
              else {
                printf("DISCARD %s%s\n", hand[k].num, hand[k].naipe);
                jaJoguei = 1;
                hand[k].ativa = 0;
                strcpy(mesa.num, hand[k].num);
                strcpy(mesa.naipe, hand[k].naipe);
                break;
              }
            } 
        }
      }
      
	    if (jaJoguei == 0) {
        for (int k = 0; k < numCard; k++) { //verifica na mão cada carta
          if ((hand[k].num[0] == 'C' || hand[k].num[0] == 'A') && hand[k].ativa == 1) {
            //se a carta for de mudança de naipe (coringa ou Ás) e se tiver o naipe em mãos
            printf("DISCARD %s%s ♦\n", hand[k].num, hand[k].naipe);
            // joga a respectiva carta
            strcpy(mesa.num, hand[k].num);
            strcpy(mesa.naipe, hand[k].naipe);
            jaJoguei = 1;
            hand[k].ativa =0;
            break;  
          }
        }
      }
        if (jaJoguei==0) {
          printf("BUY 1\n");
          scanf("%s", buytemp);
          insertInHand(buytemp);
        } //puxar carta caso não tenha em mãos
    }
  }
  
  return 0;
}
