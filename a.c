#include <stdio.h>
typedef struct{
    char nome[10];
    int pontuacao;
}jogadores;
int main(){
    jogadores melhores[10];
    jogadores novo;
    FILE *fp;
    fp = fopen("a.bin","wb");

    printf("Nome 1\n");
    scanf("%s",melhores[0].nome);
    printf("Pontuacao 1\n");
    scanf("%d",&melhores[0].pontuacao);

    printf("Nome 2\n");
    scanf("%s",melhores[1].nome);
    printf("Pontuacao 2\n");
    scanf("%d",&melhores[1].pontuacao);

    printf("Nome 3\n");
    scanf("%s",melhores[2].nome);
    printf("Pontuacao 3\n");
    scanf("%d",&melhores[2].pontuacao);

    printf("Nome 4\n");
    scanf("%s",melhores[3].nome);
    printf("Pontuacao 4\n");
    scanf("%d",&melhores[3].pontuacao);

    printf("Nome 5\n");
    scanf("%s",melhores[4].nome);
    printf("Pontuacao 5\n");
    scanf("%d",&melhores[4].pontuacao);

    printf("Nome 6\n");
    scanf("%s",melhores[5].nome);
    printf("Pontuacao 6\n");
    scanf("%d",&melhores[5].pontuacao);

    printf("Nome 7\n");
    scanf("%s",melhores[6].nome);
    printf("Pontuacao 7\n");
    scanf("%d",&melhores[6].pontuacao);

    printf("Nome 8\n");
    scanf("%s",melhores[7].nome);
    printf("Pontuacao 8\n");
    scanf("%d",&melhores[7].pontuacao);

    printf("Nome 9\n");
    scanf("%s",melhores[8].nome);
    printf("Pontuacao 9\n");
    scanf("%d",&melhores[8].pontuacao);

    printf("Nome 10\n");
    scanf("%s",melhores[9].nome);
    printf("Pontuacao 10\n");
    scanf("%d",&melhores[9].pontuacao);

    printf("Novo jogador\n");
    scanf("%s",novo.nome);
    printf("Pontuacao nova\n");
    scanf("%d",&novo.pontuacao);

    melhores[4] = melhores[5];


    fwrite(&melhores[0],sizeof(melhores[0]),1,fp);
    fwrite(&melhores[1],sizeof(melhores[1]),1,fp);
    fwrite(&melhores[2],sizeof(melhores[2]),1,fp);
    fwrite(&melhores[3],sizeof(melhores[3]),1,fp);
    fwrite(&melhores[4],sizeof(melhores[4]),1,fp);
    fwrite(&melhores[5],sizeof(melhores[5]),1,fp);
    fwrite(&melhores[6],sizeof(melhores[6]),1,fp);
    fwrite(&melhores[7],sizeof(melhores[7]),1,fp);
    fwrite(&melhores[8],sizeof(melhores[8]),1,fp);
    fwrite(&melhores[9],sizeof(melhores[9]),1,fp);
    fwrite(&novo,sizeof(novo),1,fp);
    fclose(fp);


    return 0;
}