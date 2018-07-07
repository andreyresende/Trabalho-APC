#include <stdio.h>
typedef struct{
    char nome[10];
    int pontuacao;
}jogadores;
int main(){
    int i;
    FILE *fp;
    jogadores melhores[10];
    fp = fopen("ranking.bin","wb");
    for(i=0;i<10;i++){
        printf("Digite o nome e a pontuacao\n");
        scanf("%s %d",melhores[i].nome,&melhores[i].pontuacao);
    }
    for(i=0;i<10;i++){
        fwrite(&melhores[i],sizeof(jogadores),1,fp);
    }
    fclose(fp);






    return 0;
}