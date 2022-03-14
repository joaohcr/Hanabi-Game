#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "lab.h"

void Menu(int *option)
{
	setlocale(LC_ALL, "");
	puts("\n\n Menu:");
	puts("\n\t 1 - Jogar uma partida de Hanabi");
	puts("\n\t 2 - Carregar uma partida a partir de ficheiro");
	puts("\n\t 3 - Apresentar uma descrição do jogo na consola");
	puts("\n\t 4 - Sair da aplicação");
	printf("\n Opção: ");
	scanf("%d", option);
}
void PlayerName(char *name, int n)
{
	int i;
	printf("\n Digite o seu nome: ");
	for(i=0; i<n; i++)
		scanf("%c", &name[i]);
	
}
void PickPlayer(int *n)
{
	srand(time(NULL));
	*n=rand()%2;
	if(*n==1) {
		gotoxy(120,6);
		printf("O jogador e´ o primeiro a jogar.");
	}
	else {
		gotoxy(120,6);
		printf("O bot e´ o primeiro a jogar.");
	}
}
void PrintDeck(int dim)
{
	char d[]="Baralho:";
	showRectAt(100,15,9,6);
	printfAt(101,14,d);
	gotoxy(104,18);
	if(dim>=9)
		printf("%d", dim+1);
	else
		printf("0%d", dim+1);
}
void Start()
{
	FILE *file;
	file = fopen("Letras.txt", "r");
	char frase;
	gotoxy(1,1);
	frase=fgetc(file);
	while(frase!=EOF) {
			if(frase=='H')
				setForeColor(10);
			else if(frase=='A')
				setForeColor(11);
			else if(frase=='N')
				setForeColor(4);
			else if(frase=='B')
				setForeColor(6);
			else if(frase=='I')
				setForeColor(15);
			printf("%c", frase);
			frase=fgetc(file);
	}
	fclose(file);
	resetColor();
	gotoxy(85,2);
	puts("Trabalho de:");
	gotoxy(85,4);
	puts("-Diogo Medeiros");
	gotoxy(85,5);
	puts("-Eduardo Chaves");
	gotoxy(85,6);
	puts("-João Rodrigues");
	sleep(2);
}
void PrintCL(int clues, int lifes)
{
	char pistas[]="Pistas: ";
	char vidas[]="Vidas: ";
	printfAt(85,15,pistas);
	printf("%d", clues);
	printfAt(85,20,vidas);
	printf("%d", lifes);
}
