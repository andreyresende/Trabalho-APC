#include <stdio.h>
typedef struct{
    char nome[10];
    int pontuacao;
}jogadores;
int main(){
    jogadores melhores[10];
    scanf("%s",melhores[1].nome);
    scanf("%d",&melhores[1].pontuacao);
    melhores[0] = melhores[1];
    printf("%s - %d",melhores[0].nome, melhores[0].pontuacao);




    return 0;
}