#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "lab.h"
#include "interface.h"
#include "functions.h"

#define SIZE 10
#define MAX_COLOURS 5
#define MAX_NUMBERS 10
#define HAND 5
#define NAME 16

struct card {
    int number;
    char colour[SIZE];
};
typedef struct card Deck;
int numbers[MAX_NUMBERS] = {1,1,1,2,2,3,3,4,4,5};
char colours[MAX_COLOURS][SIZE] = {"Amarelo", "Azul", "Verde", "Vermelho", "Branco"}; /* 0, 1, 2, 3, 4 */

struct clue {
	int nc[5];
	int cc[5];
};
typedef struct clue Clues;

void Interface(void);
void ResetGame(void);
void Game(void);
void Turn(void);
void Score(void);
void SaveGame(void);
void LoadGame(void);
int CardColour(Deck arr[], int);
int ColourID(Deck arr[], int);
void ColourID_Reverse(char aux[], int);
int LowestNumber(void);
int RandomColour(void);
int LowestTable(void);
int Table(void);
void InitializeDeck(void);
void ShuffleDeck(void);
void DealCards(void);
void PrintPlayerHand(char*);
void PrintBotHand(void);
void PrintFireworks(void);
void Display(void);
void PrintDiscardDeck(void);
void PlayerTurn(void);
void PlayerDiscard(void);
void PlayerPlay(void);
void PlayerClues(void);
void BotTurn(void);
void BotClues_N(int);
void BotClues_C(char*);
int BotPlayable(void);
void BotPlay(int);
int CountCards_C(Deck hand[], char);
int CountCards_N(Deck hand[], int);
int BotDiscardable(int);
void BotDiscard(int);

char name[NAME]="";
int first;
int dim=49;
int clues = 8;
int lifes = 3; 
int fireworks[5]={0};
int discard_deck[5][5]={0};
Deck deck[50];
Deck player_hand[HAND];
Deck bot_hand[HAND];
Clues player_clues={{0},{0}};
Clues bot_clues={{0},{0}};
FILE *save=NULL;

void main() 
{
	system("MODE con cols=200 lines=50");
	int k=1;
	setlocale(LC_ALL, "");
	Start();
	int option=0;
	while (k==1) 
	{
	Menu(&option);
	system("cls");
	switch(option) {
		case 1:
		{
			ResetGame();
			PlayerName(name,NAME);
			system("cls");
			InitializeDeck();
			ShuffleDeck();
			DealCards();
			Interface();
			PickPlayer(&first);
			system("pause");
			ClearScreen();
			Game();
			break;
		}
		case 2:
		{
			LoadGame();
			Interface();
			Game();
			break;
		}
		case 3:
		{
			FILE* rules=NULL;
			rules=fopen("rules.txt","r");
			char c;
			c = fgetc(rules);
    		while (c != EOF)
    		{
       			printf("%c", c);
        		c = fgetc(rules);
    		}
  			fclose(rules);
  			system("pause");
  			system("cls");
  			break;
  		}	
		case 4:
			exit(0);
			break;
		default:
			puts("\nA opção não existe.");
			system("pause");
  			system("cls");
			break;
	}
	}
}
////////////////////////////////////////////////////////////////////////////// Bot
void BotTurn()
{
	int i, n1, n2, n3, n4, option;
	ClearScreen();
	gotoxy(120,6);
	if(first==0) {
		printf("Deseja guardar o jogo e sair?");
		gotoxy(120,8);
		printf("  Sim(1)/Não(2): ");
		scanf("%d", &option);
		if(option==1) {
		SaveGame();
		exit(0);
	}
	else
		ClearScreen();
	}
	if(first==0 && dim==39 && clues==8) {
		if(CountCards_N(player_hand,1)>0)
			BotClues_N(1);
		else {
			printfAt(120,6,"Não tens cartas '1'.");
		}
	}
	else if(BotPlayable()!=-1) {
		n1=BotPlayable();
		BotPlay(n1);
	}
	else {
		if(clues>0) {
			if(PlayerPlayable()!=-1) {
				n2=PlayerPlayable();
				if(player_clues.nc[n2]==0 && CountCards_N(player_hand, player_hand[n2].number)==1) {
					gotoxy(120,6);
					printf("A %d.ª carta é a tua única carta %d.", n2+1, player_hand[n2].number);
					player_clues.nc[n2]=1;
					clues--;
				}
				else if(player_clues.cc[n2]==0 && CountCards_C(player_hand, player_hand[n2].colour[2])==1) {
					gotoxy(120,6);
					printf("A %d.ª carta é a tua única carta %s.", n2+1, player_hand[n2].colour);
					player_clues.cc[n2]=1;
					clues--;
				}
				else {
					if(player_clues.nc[n2]==0 && player_clues.cc[n2]==1)
						BotClues_N(player_hand[n2].number);
					else if(player_clues.cc[n2]==0 && player_clues.nc[n2]==1)
						BotClues_C(player_hand[n2].colour);
					else if(Random(2)==0)
						BotClues_N(player_hand[n2].number);
					else
						BotClues_C(player_hand[n2].colour);	
				}
			}
			else {
				if(LowestNumber()!=-1) {
					n3=player_hand[LowestNumber()].number;
					BotClues_N(n3);
				}
				else {
					n3=RandomColour();
					BotClues_C(colours[n3]);
				}
			}		
		}
		else {
/*			if(BotDiscardable(1)!=-1) {
				n4=BotDiscardable(1);
		}	*/	
			if(BotDiscardable(2)!=-1) {
				n4=BotDiscardable(2);
			}	
			else if(BotDiscardable(3)!=-1) {
				n4=BotDiscardable(3);
			}
			else if(BotDiscardable(4)!=-1){
				n4=BotDiscardable(4);
			}
			else
				n4=BotDiscardable(5);
			BotDiscard(n4);
		}
	}
	Interface();
}
void BotDiscard(int i)
{
	int c, num;
	gotoxy(120,6);
	printf("O bot descartou a carta %d %s.", bot_hand[i].number, bot_hand[i].colour);
	c=ColourID(bot_hand, i);
	num=bot_hand[i].number-1;
	discard_deck[num][c]++;
	if(dim>=0) {
		bot_hand[i].number=deck[dim].number;
		strcpy(bot_hand[i].colour, deck[dim].colour);
		bot_clues.nc[i]=0;
		bot_clues.cc[i]=0;
		clues++;
		dim--;
	}
}
int BotDiscardable(int n)
{
	int i;
	
	if(n==1) {
		for(i=0; i<HAND; i++) {
			if(bot_clues.cc[i]==0 && bot_clues.nc[i]==0)
				return i;
		}
		if(i==HAND)
			return -1;
	}
	if(n==2) {
		for(i=0; i<HAND; i++) {
			if(bot_clues.nc[i]==1 && bot_hand[i].number<=LowestTable())
				return i;
		}
		if(i==HAND)
			return -1;
	}
	if(n==3) {
		int col;
		for(i=0; i<HAND; i++) {
			col=ColourID(bot_hand,i);
			if(bot_clues.cc[i]==1 && bot_clues.nc[i]==1 && bot_hand[i].number<=fireworks[col])
				return i;
		}
		if(i==HAND)
			return -1;
	}
	if(n==4) {
		int j=0, k=0, *aux;
		for(i=0; i<HAND; i++) {
			if((bot_clues.cc[i]==0&&bot_clues.nc[i]==1)||(bot_clues.cc[i]==1&&bot_clues.nc[i]==0))
				k++;
		}
		if(k==0)
			return -1;
		else {
			aux=malloc(k*sizeof(int));
			for(i=0; i<HAND; i++) {
				if((bot_clues.cc[i]==0&&bot_clues.nc[i]==1)||(bot_clues.cc[i]==1&&bot_clues.nc[i]==0)) {
					aux[j]=i;
					j++;
				}
			}
			return aux[Random(k)];
		}
	}
	if(n==5)
		return Random(HAND);	
}
void BotPlay(int n)
{
	ClearScreen();
	gotoxy(120,6);
	printf("O bot decidiu jogar a carta %d %s.", bot_hand[n].number, bot_hand[n].colour);
	fireworks[ColourID(bot_hand,n)]++;
	if(dim>=0){
		bot_hand[n].number=deck[dim].number;
		strcpy(bot_hand[n].colour, deck[dim].colour);
		bot_clues.nc[n]=0;
		bot_clues.cc[n]=0;
		dim--;
	}
}
int BotPlayable()
{
	int i=0;
	do {
		if(bot_clues.nc[i]==1) {
			if(Table()!=-1 && bot_hand[i].number==(Table()+1))
				return i;
			else if(bot_clues.cc[i]==1) {
					if(bot_hand[i].number==(fireworks[ColourID(bot_hand,i)]+1))
						return i;
				}
		}
		i++;
	} while(i<HAND);
	if(i=HAND)
		return -1;
}
void BotClues_C(char *col)
{
	int i;
	char aux[SIZE];
	for(i=0; i<SIZE; i++) {
		aux[i]=col[i];
	}
	for(i=0; i<HAND; i++) {
		if(strcmp(player_hand[i].colour,aux)==0)
			player_clues.cc[i]=1;
	}
	gotoxy(120,6);
	printf("O bot deu pistas sobre a cor %s.", aux);
	clues--;
}
void BotClues_N(int n)
{
	int i;
	for(i=0; i<HAND; i++) {
		if(player_hand[i].number==n)
			player_clues.nc[i]=1;
	}
	gotoxy(120,6);
	printf("O bot deu pistas sobre o número %d.", n);
	clues--;
}
int PlayerPlayable()
{
	int k=0;
	do {
		if(player_hand[k].number==(fireworks[ColourID(player_hand,k)]+1))
			return k;
		k++;
	} while(k<HAND);
	if(k==HAND)
		return -1;
}
//////////////////////////////////////////////////////////////////////// Jogador
void PlayerTurn()
{
	int jog;
	ClearScreen();
	printfAt(120,6,"\tA sua jogada:");
	printfAt(120,8,"\t 1. Dar uma pista");
	printfAt(120,10,"\t 2. Descartar uma carta");
	printfAt(120,12,"\t 3. Jogar uma carta");
	if(first==1) {
		printfAt(120,14,"\t 4. Gravar o jogo e sair");
	}
	printfAt(120,16,"\t Opção: ");
	scanf("%d", &jog);
	ClearScreen();
	switch(jog) {
		case 1:
			PlayerClues();
			break;
		case 2:
			PlayerDiscard();
			break;
		case 3:
			PlayerPlay();
			break;
		case 4:
			SaveGame();
			exit(0);
			break;
		default:
			printfAt(120,6,"Opção inválida!");
			sleep(1);
			PlayerTurn();
			break;
	}
}
void PlayerClues()
{
	if(clues==0)
	{
		printfAt(120,6,"Jogada ilegal!");
		sleep(2);
		PlayerTurn();
	}
	else {
		ClearScreen();
		char option, clue_col[10]="";
		int i, col, num;
		printfAt(120,6,"Escolha o  tipo de pista que pretende dar:");
		printfAt(120,8,"\t- Cor (C)");
		printfAt(120,10,"\t- Número (N)");
		printfAt(120,12,"  Opção: ");
		fflush(stdin);
		scanf("%c", &option);
		switch(option) {
			case 'c': case 'C':	
			printfAt(120,14,"Escolha a cor:");
			printfAt(120,16,"\t- Amarelo (1)");
			printfAt(120,18,"\t- Azul (2)");
			printfAt(120,20,"\t- Verde (3)");
			printfAt(120,22,"\t- Vermelho (4)");
			printfAt(120,24,"\t- Branco (5)");
			printfAt(120,26,"  Opção: ");
			scanf("%d", &col);
			switch(col) {
				case 1:
				strcpy(clue_col, "Amarelo");
				break;
				case 2:
				strcpy(clue_col, "Azul");
				break;
				case 3:
				strcpy(clue_col, "Verde");
				break;	
				case 4:
				strcpy(clue_col, "Vermelho");
				break;
				case 5:					
				strcpy(clue_col, "Branco");
				break;
				default:
				printfAt(120,28,"  Cor inválida.");
				sleep(1);
				PlayerClues();
				break;
			}
			for(i=0; i<HAND; i++) {
				if(strcmp(bot_hand[i].colour, clue_col)==0)
					bot_clues.cc[i]=1;
			}
			clues--;
			break;
			case 'n': case 'N':
			printfAt(120,14,"  Escolha o número (1-5): ");
			scanf("%d", &num);
			if(num<1||num>5) {
				printfAt(120,16,"  Número inválido.");
				sleep(1);
				PlayerClues();
			}
			else {
				for(i=0; i<HAND; i++) {
					if(bot_hand[i].number==num)
						bot_clues.nc[i]=1;
				}
			}
			clues--;
			break;
			default:
			printfAt(120,14,"  Opção inválida.");
			sleep(1);
			PlayerClues();
			break;
		}
	Interface();
	}
}
void PlayerPlay()
{
	int i, col;
	printfAt(120,6,"Escolha de 1 a 5 a carta que pretende jogar: ");
	scanf("%d", &i);
	i--;
	gotoxy(120,8);
	printf(" Jogaste a carta %d %s.", player_hand[i].number, player_hand[i].colour);
	col=ColourID(player_hand, i);
	if(player_hand[i].number==(fireworks[col]+1)) {
		fireworks[col]++;
		Interface();
	}
	else {
		printfAt(120,10," A carta que jogaste não é válida!");
		discard_deck[player_hand[i].number-1][col]++;
		lifes--;
		Interface();
	}
	if(dim>=0){
		player_hand[i].number=deck[dim].number;
		strcpy(player_hand[i].colour, deck[dim].colour);
		player_clues.nc[i]=0;
		player_clues.cc[i]=0;
		dim--;
	}
	Interface();
}
void PlayerDiscard()
{
	if(clues==8) {
		printfAt(120,6,"Jogada ilegal!");
		sleep(2);
		PlayerTurn();
	}
	else {
		int i, c, num;
		printfAt(120,6,"Escolha de 1 a 5 a carta que pretende descartar:");
		scanf("%d", &i);
		i--;
		gotoxy(120,8);
		printf(" Descartaste a carta %d %s.", player_hand[i].number, player_hand[i].colour);
		c=ColourID(player_hand, i);
		num=player_hand[i].number-1;
		discard_deck[num][c]++;
		if(dim>=0) {
			player_hand[i].number=deck[dim].number;
			strcpy(player_hand[i].colour, deck[dim].colour);
			player_clues.nc[i]=0;
			player_clues.cc[i]=0;
			dim--;
		}
		clues++;
		Interface();
	}
}
/////////////////////////////////////////////////////////////////// Cores e Numeros
int LowestNumber()
{
	int i=0, low_pos, flag=1;
	for(i=0; i<HAND; i++) {
		if(player_clues.nc[i]==1)
			flag=-1;
		else {
			low_pos=i;
			flag=0;
		}
	}
	if(flag==0) {
		for(i=0; i<HAND; i++) {
			if(player_hand[i].number<player_hand[low_pos].number && player_clues.nc[i]==0)
				low_pos=i;
		}
		return low_pos;
	}
	else
		return flag;
}
int RandomColour()
{
	int i, n, j=0, k=0, *aux;
	int col[]={0,1,2,3,4};
	for(i=0; i<HAND; i++) {
		if(player_clues.cc[i]==1)
			col[ColourID(player_hand,i)]=-1;
	}
	for(i=0; i<HAND; i++) {
		if(col[i]!=-1)
			k++;
	}
	aux=malloc(k*sizeof(int));
	for(i=0; i<HAND; i++) {
		if(col[i]!=-1) {
			aux[j]=col[i];
			j++;
		}
	}
	srand(time(NULL));
	n=rand()%k;
	return aux[n];
}
int ColourID(Deck arr[], int k)
{
	if(strcmp(arr[k].colour, "Amarelo")==0)
			return 0;
	else if(strcmp(arr[k].colour, "Azul")==0)
		 	return 1;
	else if(strcmp(arr[k].colour, "Verde")==0)
			return 2;
	else if(strcmp(arr[k].colour, "Vermelho")==0)
			return 3;
	else if(strcmp(arr[k].colour, "Branco")==0)
			return 4;
}
void ColourID_Reverse(char aux[], int b)
{
	switch (b) {
		case 0:
			strcpy(aux, "Amarelo");
			break;
		case 1:
			strcpy(aux, "Azul");
			break;
		case 2:
			strcpy(aux, "Verde");
			break;
		case 3:
			strcpy(aux, "Vermelho");
			break;
		case 4:
			strcpy(aux, "Branco");
			break;
	}
}
int CardColour(Deck arr[], int k)
{
	if(strcmp(arr[k].colour, "Amarelo")==0)
			return 6;
	else if(strcmp(arr[k].colour, "Azul")==0)
		 	return 11;
	else if(strcmp(arr[k].colour, "Verde")==0)
			return 10;
	else if(strcmp(arr[k].colour, "Vermelho")==0)
			return 4;
	else if(strcmp(arr[k].colour, "Branco")==0)
			return 15;
}
///////////////////////////////////////////////////////////////////// Mesa
int CountCards_C(Deck hand[], char c)
{
	int k, counter=0;
	for(k=0; k<HAND; k++) {
		if(hand[k].colour[2]==c)
			counter++;
	}
	return counter;
}
int CountCards_N(Deck hand[], int n)
{
	int k, counter=0;
	for(k=0; k<HAND; k++) {
		if(hand[k].number==n)
			counter++;
	}
	return counter;
}
int LowestTable()
{
	int i, aux[HAND];
	
	for(i=0; i<HAND; i++)
		aux[i]=fireworks[i];
	
	SelectionSort(aux, HAND);
	return aux[0];
}
int Table()
{
	int i=0, flag=-1;
	do {
		if(fireworks[i]==fireworks[i+1])
			flag=1;
		else
			flag=-1;
		i++;
	} while(flag==1||i<HAND);
	if(flag==1)
		return fireworks[i];
	else
		return flag;
}
////////////////////////////////////////////////////////////////////////// Deck
void InitializeDeck()
{
  int i;
  for(i=0;i<dim+1;i++) {
	deck[i].number = numbers[i%MAX_NUMBERS];
    strncpy(deck[i].colour, colours[i/MAX_NUMBERS], SIZE);
  }
}
void ShuffleDeck()
{
  int swapper = 0;
  int i, j;
  Deck temp;
  srand(time(NULL));
  for(j=0;j<5;j++) {
	for(i=0;i<dim+1;i++) {
  		swapper = rand() % (dim+1);
		temp = deck[i];
   		deck[i] = deck[swapper];
   		deck[swapper] = temp;
	}
  }
}
/////////////////////////////////////////////////////////////////////////////// Mãos
void DealCards()
{
	int i;
	for(i=0;i<HAND;i++) {
		player_hand[i].number = deck[dim-2*i].number;
		strcpy(player_hand[i].colour, deck[dim-2*i].colour);
	}
	for(i=0;i<HAND;i++) {
		bot_hand[i].number = deck[dim-1-2*i].number;
		strcpy(bot_hand[i].colour, deck[dim-1-2*i].colour);
	}
	dim-=10;
}
void PrintPlayerHand(char *name)
{
	int k=0, i=0, cardcolour;
	setColor(0,0);
	for(i=10; i<75; i++) {
		for(k=25; k<=31; k++) {	
			showCharAt(i,k,' ');
		}
	}
	resetColor();
	for(k=0; k<NAME; k++)
		showCharAt(80+k,28,name[k]);
	for (k=0; k<HAND; k++) {
		cardcolour=CardColour(player_hand,k);
		if(player_clues.cc[k]==1) {
			setColor(0,cardcolour);
			showRectAt(10+14*k,25,8,6);
			for(i=1; i<4; i++) {
				setColor(cardcolour,cardcolour);
				showRectAt(10+14*k+i,25+i,8-2*i,6-2*i);
			}	
		}
		else {
			resetColor();
			showRectAt(10+14*k,25,8,6);
		}
		if(player_clues.nc[k]==1 && player_clues.cc[k]==1) {
			setColor(0,cardcolour);
			showNumAt(14+14*k,28,player_hand[k].number);
		}
		else if(player_clues.nc[k]==1 && player_clues.cc[k]==0)
			showNumAt(14+14*k,28,player_hand[k].number);
	}
	resetColor();
}
void PrintBotHand()
{
	int k=0, i=0, cardcolour;
	gotoxy(3, 8);
	puts("Bot");
	for (k=0; k<HAND; k++) {
		cardcolour=CardColour(bot_hand,k);
		setColor(0,cardcolour);
		showRectAt(10+14*k,5,8,6);
		if(bot_clues.nc[k]==1 && bot_clues.cc[k]==1) {
			setColor(0, cardcolour);
			showNumAt(10+14*k,12,bot_hand[k].number);
		}
		else if(bot_clues.nc[k]==1 && bot_clues.cc[k]==0) {
			setColor(15,0);
			showNumAt(10+14*k,12,bot_hand[k].number);
		}
		else if(bot_clues.nc[k]==0 && bot_clues.cc[k]==1) {
			setColor(cardcolour, cardcolour);
			showCharAt(10+14*k,12,' ');	
		}
		else {
			setColor(0,0);
			showCharAt(10+14*k,12,' ');
		}
		for(i=1;i<4;i++) {
			setColor(cardcolour,cardcolour);
			showRectAt(10+14*k+i,5+i,8-2*i,6-2*i);
		}
		setColor(0,cardcolour);
		showNumAt(14+14*k,8,bot_hand[k].number);
		resetColor();
	}
}
/////////////////////////////////////////////////////////////////////////////// Interface
void PrintFireworks()
{
	int k=0;
	for(k=0; k<5; k++) {
		switch(k) {
			case 0:
				setForeColor(6);
				break;
			case 1:
				setForeColor(11);
				break;
			case 2:
				setForeColor(10);
				break;
			case 3:
				setForeColor(4);
				break;
			case 4:
				setForeColor(15);
				break;
		}
		showRectAt(11+14*k,16,6,4);
		gotoxy(14+14*k,18);
		printf("%d", fireworks[k]);
		resetColor();
	}
}
void PrintDiscardDeck()
{
	int i=0,j=0;
	for(i=0; i<MAX_COLOURS; i++) {	
		if(strcmp(colours[i], "Amarelo")==0)
			setForeColor(6);
	else if(strcmp(colours[i], "Azul")==0)
		 	setForeColor(11);
	else if(strcmp(colours[i], "Verde")==0)
			setForeColor(10);
	else if(strcmp(colours[i], "Vermelho")==0)
			setForeColor(4);
	else if(strcmp(colours[i], "Branco")==0)
			setForeColor(15);
		printfAt(85,6+i,colours[i]);
		resetColor();
		gotoxy(96+4*i,5);
		printf("%d", i+1);
		for(j=0; j<5; j++) {
		gotoxy(96+4*i,6+j);
		printf("%d", discard_deck[i][j]);
		}
	}
}
void Interface()
{
	PrintPlayerHand(name);
	PrintBotHand();
	PrintFireworks();
	PrintDiscardDeck();
	PrintCL(clues,lifes);
	PrintDeck(dim);
}
//////////////////////////////////////////////////////////////////////////////////////////////// Jogo
void Game()
{
	int k=0;
	while(dim>=0&&lifes!=0&&k<25) {
		Turn();
		k=Sum(fireworks, 5);
	}
	if(k==25) {
		ClearScreen();
		printfAt(120,6,"Lendário! O público nunca esquecerá este espetáculo!!!");
		printfAt(120,8,"\tPontuacão final: 25 pontos");
		sleep(5);
	}
	if(dim==-1) {
		Turn();
		if(lifes==0) {
			ClearScreen();
			printfAt(120,6,"Perdeste todas as vidas!");
			printfAt(120,8,"Os deuses demonstraram a sua ira na forma de uma tempestade");
			printfAt(120,9," que pôs fim ao fogo-de-artifício.");
			printfAt(120,12,"Pontuação final: 0 pontos");
			sleep(5);
		}
		else
			Score();
	}
	else if(lifes==0) {
		ClearScreen();
		printfAt(120,6,"Perdeste todas as vidas!");
		printfAt(120,8,"Os deuses demonstraram a sua ira na forma de uma tempestade");
		printfAt(120,9," que pôs fim ao fogo-de-artifício.");
		printfAt(120,12,"Pontuação final: 0 pontos");
		sleep(5);
	}
	gotoxy(2,34);
}
void Score()
{
	int score=Sum(fireworks, 5);
	ClearScreen();
	gotoxy(120,6);
	if(score>=0 && score <=5)
		puts("Oh! Credo foram todos vaiados.");
	else if(score>=6 && score <=10)
		puts("Muito pobre, os espectadores estão entediados.");
	else if(score>=11 && score <=15)
		puts("O espetáculo foi razoável, mas os espectadores já viram melhor.");
	else if(score>=16 && score<=20)
		puts("Bom espetáculo! O público está satisfeito.");
	else if(score>=21 && score<25)
		puts("Muito bom! O público está entusiasmado!");
	else if(score==25)
		puts("Lendário! O público nunca esquecerá este espetáculo!");
	gotoxy(120,8);
	printf("  Pontuação final: %d pontos", score);
	sleep(5);
}
void Turn()
{
	if(first==0) {
		SaveGame();
		BotTurn();
		gotoxy(120,30);
		system("pause");
		PlayerTurn();
		gotoxy(120,30);
		system("pause");
	}
	else if(first==1) {
		SaveGame();
		PlayerTurn();
		gotoxy(120,30);
		system("pause");
		BotTurn();
		gotoxy(120,30);
		system("pause");
	}
}
void SaveGame()
{
	int i=0, j=0, col;
	save=fopen("save.txt", "w");
	for(i=0; i<49; i++) {
		col=ColourID(deck, i);
		fprintf(save, "%d %d ", deck[i].number, col);	
	}
	fprintf(save, "%d ", dim);
	for(i=0; i<HAND; i++) {
		col=ColourID(player_hand, i);
		fprintf(save, "%d %d %d %d ", player_hand[i].number, col, player_clues.nc[i], player_clues.cc[i]);
	}
	for(i=0; i<HAND; i++) {
		col=ColourID(bot_hand, i);
		fprintf(save, "%d %d %d %d ", bot_hand[i].number, col, bot_clues.nc[i], bot_clues.cc[i]);
	}
	for(i=0; i<HAND; i++)
		fprintf(save, "%d ", fireworks[i]);
	for(j=0; j<HAND; j++) {
		for(i=0; i<HAND; i++) {
			fprintf(save, "%d ", discard_deck[j][i]);
		}
	}
	fprintf(save, "%d %d %d ", first, clues, lifes);
	fprintf(save, "%s", name);
	fclose(save);
}
void LoadGame()
{
	int i, j, a, b, c, d;
	char x, aux[SIZE];
	if((save=fopen("save.txt", "r"))==NULL)
		return;
	else {
		for(i=0; i<49; i++) {
			fscanf(save, "%d %d ", &a, &b);
			deck[i].number=a;
			ColourID_Reverse(aux,b);
			strcpy(deck[i].colour, aux);
		}
		fscanf(save, "%d ", &dim);
		for(i=0; i<HAND; i++) {
			fscanf(save, "%d %d %d %d ", &a, &b, &c, &d);
			player_hand[i].number=a;
			ColourID_Reverse(aux,b);
			strcpy(player_hand[i].colour, aux);
			player_clues.nc[i]=c;
			player_clues.cc[i]=d;
		}
		for(i=0; i<HAND; i++) {
			fscanf(save, "%d %d %d %d ", &a, &b, &c, &d);
			bot_hand[i].number=a;
			ColourID_Reverse(aux,b);
			strcpy(bot_hand[i].colour, aux);
			bot_clues.nc[i]=c;
			bot_clues.cc[i]=d;
		}
		for(i=0; i<HAND; i++)
			fscanf(save, "%d ", &fireworks[i]);
		for(j=0; j<HAND; j++) {
			for(i=0; i<HAND; i++) {
				fscanf(save, "%d ", &discard_deck[j][i]);
			}
		}
		fscanf(save, "%d %d %d ", &first, &clues, &lifes);
		for(i=0; i<NAME; i++) {
			fscanf(save, "%c", &x);
			name[i]=x;
		}
		fclose(save);
	}
}
void ResetGame()
{
	int i=0, j=0;
	dim = 49;
	clues=8;
	lifes=3;
	for(i=0; i<HAND; i++) {
		player_clues.cc[i]=0;
		player_clues.nc[i]=0;
		bot_clues.cc[i]=0;
		bot_clues.nc[i]=0;
		fireworks[i]=0;
		for(j=0; j<HAND; j++)
			discard_deck[i][j]=0;
	}
}
