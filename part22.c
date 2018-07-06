/* Universidade de Brasilia
Instituto de Ciencias Exatas
Departamento de Ciencia da Computacao
Algoritmos e Programacao de Computadores – 1/2018
Aluno(a): Andrey Calaca Resende
Matricula: 180062433
Turma: A
Versao do compilador: gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.9)
Descricao: O jogo acontece na ordem definida pela funcao jogo, que e chamada pela Main, como explicado no comentario abaixo, as funcoes nao estao na melhor ordem para o entendimento, e sim para melhor compilacao, recomendo a leitura/correcao seguindo a ordem em que aparecem sendo chamadas pela funcao jogo, mantendo sempre as variaveis globais em mente. */
/*O codigo comeca de fato na linha 71, ate la sao linhas definindo especificacoes do trabalho.*/
/* Detalhe na organizacao das funcoes: A ordem delas pode parecer confusa mas o objetivo dessa ordem e tirar os Warnings de 'Implicit Declaration' do terminal ao compilar */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define RAND () ((rand()%100 + 1))

int altura, largura;
char tabuleiro[10][135];//Basicamente esse eh o tamanho maximo da matriz que o jogador pode colocar
char borda[1][135];
int probX, probF, probO, probT;
int velocidade;
int combustivel = 400;
int pontuacao = 1;
int colisao = 0;
int vicio = 1;//Serve para, ao final de cada jogo, verificar se o usuario deseja jogar novamente
int vidas;//Vida dos inimigos tipo O
int contador = 0;//Conta a quantidade de inimigos tipo O 
int contador2 = 0;//Conta a quantidade de inimigos tipo T
int ammo;
struct inimigo{
    int municao;
    int x;
    int y;
};
struct inimigo T[1000];
struct enemy{
    int hp;
    int y;
    int x;
};
struct enemy O[1000];

#ifdef _WIN32
    #define CLEAR system("cls");
#else
    #define CLEAR system("clear");
#endif

#ifndef _WIN32
    int kbhit(){
        struct termios oldt, newt;
        int ch, oldf;
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if(ch != EOF){
            ungetc(ch,stdin);
            return 1;
        }
        return 0;
    }
    int getch(void) {
        int ch;
        struct termios oldt;
        struct termios newt;
        tcgetattr(STDIN_FILENO,&oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
    #else
        #include <conio.h>
#endif
void welcome(){//Da as boas-vindas ao player.
    int n;
    printf("Bem-Vindo ao River Raid, C Version =D.\nDigite 1 para continuar\n");
    scanf("%d",&n);
    while(n != 1){
        printf("Digite 1 e enter para continuar:\n");
        scanf("%d",&n);
    }
}
void modificarTamanho(int height, int width){
    FILE *fp;
    fp = fopen("size.txt","w");
    if(fp == NULL){
        printf("Problema ao abrir arquivo size.txt");
        exit(0);
    }
    fprintf(fp,"%d\n",height);
    fprintf(fp,"%d",width);
    fclose(fp);
}
void modificarProb(int valor1, int valor2, int valor3, int valor4){
    FILE *fp;
    fp = fopen("prob.txt","w");
    fprintf(fp,"%d\n",valor1);//X
    fprintf(fp,"%d\n",valor2);//F
    fprintf(fp,"%d\n",valor3);//O
    fprintf(fp,"%d",valor4);//T
    fclose(fp);
}
void modificarVelocidade(int speed){
    FILE *fp;
    fp = fopen("velocidade.txt","w");
    fprintf(fp,"%d",speed);
    fclose(fp);
}
void modificarVida(int vidaO){
    FILE *fp;
    fp = fopen("vidaO.txt","w");
    fprintf(fp,"%d",vidaO);
    fclose(fp);
}
void modificarMunicao(int municaoT){
    FILE *fp;
    fp = fopen("municao.txt","w");
    fprintf(fp,"%d",municaoT);
    fclose(fp);
}
void leituraDosArquivos(){
    FILE *config;
    config = fopen("size.txt","r");
    if(config == NULL){
        printf("\nArquivo nao existente/Problema na abertura\n");
        exit(0);
    }
    fscanf(config,"%d %d",&altura,&largura);
    fclose(config);
    

    config = fopen("prob.txt","r");
    if(config == NULL){
        printf("\nArquivo nao existente/Problema na abertura\n");
        exit(0);
    }
    fscanf(config,"%d %d %d %d", &probX, &probF, &probO, &probT);
    fclose(config);


    config = fopen("velocidade.txt","r");
    if(config == NULL){
        printf("\nArquivo nao existente/Problema na abertura\n");
        exit(0);
    }
    fscanf(config,"%d",&velocidade);
    fclose(config);

    config = fopen("vidaO.txt","r");
    if(config == NULL){
        printf("\nArquivo nao existente/Problema na abertura\n");
        exit(0);
    }
    fscanf(config,"%d",&vidas);
    fclose(config);

    config = fopen("municao.txt","r");
    if(config == NULL){
        printf("\nArquivo nao existente/Problema na abertura\n");
        exit(0);
    }
    fscanf(config,"%d",&ammo);
    fclose(config);
}
void configurarMapa(){
    int escolheu, height, width, speed;
    CLEAR;
    printf("Main Menu > Configuracoes > Tabuleiro\n");
    printf("1 - Tamanho.\n");
    printf("2 - Velocidade.\n");
    printf("3 - Voltar.\n");
    printf("Digite a opcao desejada.\n");
    scanf("%d",&escolheu);
    while(escolheu != 3){
        if(escolheu != 1 && escolheu != 2){
            printf("Opcao invalida, escolha a opcao desejada");
            scanf("%d",&escolheu);
        }
        if(escolheu == 1){
            printf("Voce escolheu alterar o tamanho do mapa, digite a altura e a largura desejadas, separadas por espaco.\n");
            printf("A altura minima eh 3 e a maxima eh 10, ja a largura minima eh 3 e a largura maxima eh 135.\n");
            printf("O tamanho atual eh %dx%d.\n",altura,largura);
            scanf("%d %d", &height, &width);
            while(height < 3 || height > 10 || width < 3 || width > 135){
                printf("Dimensoes invalidas, digite novamente.\n");
                scanf("%d %d", &height, &width);
            }
            modificarTamanho(height,width);
            leituraDosArquivos();
            printf("Valores modificados, tamanho atual: %dx%d.\n",altura,largura);
            printf("Escolha a opcao desejada.\n");
        }
        else if(escolheu == 2){
            printf("Voce escolheu alterar a velocidade do jogo, digite o tempo de espera a cada iteracao(em microssegundos).\n");
            printf("O tempo deve ser maior que 0 mas nao possui limite.\n");
            printf("O tempo padrao eh 60000, valores muito maiores ou muito menores podem prejudicar a jogabilidade.\n");
            printf("Tempo atual: %d\n",velocidade);
            scanf("%d",&speed);
            while(speed<=0){
                printf("Velocidade invalida, digite novamente.\n");
                scanf("%d",&speed);
            }
            modificarVelocidade(speed);
            leituraDosArquivos();
            printf("Valor modificado, velocidade atual: %d \n",velocidade);
            printf("Escolha a opcao desejada.\n");
        }
        scanf("%d",&escolheu);
    }
}
void configurarNPCs(){
    int escolheu, chanceX, chanceF, chanceO, chanceT, vidaO, municaoT;
    CLEAR;
    printf("Main Menu > Configuracoes > NPCs\n");
    printf("1 - Probabilidades.\n");
    printf("2 - Vida do O.\n");
    printf("3 - Municao do T.\n");
    printf("4 - Voltar.\n");
    printf("Digite a opcao desejada.\n");
    scanf("%d",&escolheu);
    while(escolheu != 4){
        if(escolheu != 1 && escolheu != 2 && escolheu != 3){
            printf("Opcao invalida, digite uma opcao valida.\n");
            scanf("%d",&escolheu);
        }
        if(escolheu == 1){
            printf("Voce escolheu alterar as probabilidades de surgimento dos NPCs.\n");
            printf("Digite as quatro probabilidades(X, F, O, T) nessa ordem, todas variando de 0 a 100.\n");
            printf("Probabilidades atuais: X(%d), F(%d), O(%d), T(%d)\n",probX, probF, probO, probT);
            scanf("%d %d %d %d",&chanceX, &chanceF, &chanceO, &chanceT);
            while(chanceX < 0 || chanceX > 100 || chanceF < 0 || chanceF > 100 || chanceO < 0 || chanceO > 100 || chanceT < 0 || chanceT > 100){
                printf("Valores invalidos, todos devem estar entre 0 e 100, digite novamente.\n");
                scanf("%d %d %d %d",&chanceX, &chanceF, &chanceO, &chanceT);
            }
            modificarProb(chanceX, chanceF, chanceO, chanceT);
            leituraDosArquivos();
            printf("Valores modificados, probabilidades atuais: X(%d), F(%d), O(%d), T(%d).\n",probX, probF, probO, probT);
            printf("Escolha a opcao desejada.\n");
        }
        else if(escolheu == 2){
            printf("Voce escolheu alterar a vida do O.\n");
            printf("Digite a quantidade de vidas desejadas para os inimigos tipo O, com um minimo de 1.\n");
            printf("Vidas atuais: %d\n",vidas);
            scanf("%d",&vidaO);
            while(vidaO < 1){
                printf("Valor invalido, a quantidade de vidas deve ser maior que 1, digite novamente.\n");
                scanf("%d",&vidaO);
            }
            modificarVida(vidaO);
            leituraDosArquivos();
            printf("Valores modificados, vida atual: %d\n",vidas);
            printf("Escolha a opcao desejada.\n");
        }
        else if(escolheu == 3){
            printf("Voce escolheu alterar a municao do T.\n");
            printf("Digite a quantidade de projeteis dos inimigos tipo T, nao serao aceitos valores negativos.\n");
            printf("Municao atual: %d\n",ammo);
            scanf("%d",&municaoT);
            while(municaoT<0){
                printf("Valor invalido, digite um valor maior ou igual a 0.\n");
                scanf("%d",&municaoT);
            }
            modificarMunicao(municaoT);
            leituraDosArquivos();
            printf("Valores modificados, municao atual: %d\n",ammo);
            printf("Escolha a opcao desejada.\n");
        }
        scanf("%d",&escolheu);
    }
}
void configuracoes(){
    int escolheu = 0;
    while(escolheu != 4){
        CLEAR;
        printf("Main Menu > Configuracoes\n");
        printf("1 - Tabuleiro.\n");
        printf("2 - NPCs.\n");
        printf("3 - Ativar Modo Ranqueado.\n");
        printf("4 - Voltar.\n");
        printf("Digite a opcao desejada.\n");
        scanf("%d",&escolheu);
        if(escolheu != 1 && escolheu != 2 && escolheu != 3 && escolheu != 4){
            printf("Opcao invalida, escolha a opcao desejada\n");
            scanf("%d",&escolheu);
        }
        if(escolheu == 1)
            configurarMapa();
        else if(escolheu == 2)
            configurarNPCs();
    }
}
void opcao(int escolheu){//Funcao utilizada pela funcao menu, no momento so funciona para instrucoes. Colocada antes no codigo para evitar Warnings no terminal, ja que se ela for chamada por uma funcao antes de ser declarada ele reclama com "Implicit Declaration".
    int jogar=0, valor1, valor2, valor3, valor4;
    switch (escolheu){
        case 2 :
            configuracoes();
            break;
        case 3 ://Implementar depois
            break;
        case 4 :
            CLEAR;
            printf("Comandos:\nw(move o personagem para cima e gasta 2 de combustivel).\ns(move o personagem para baixo e gasta 2 de combustivel).\nd(atira e gasta 3 de combustivel).\n");
            printf("Voce eh uma nave (+), inimigos(X) virao pela direita, bem como combustivel(F).\nEncostar em um inimigo finaliza o jogo, encostar em um (F) aumenta o seu combustivel em 40 unidades.\n");
            printf("O seu tiro destroi tanto inimigos quanto combustivel, sendo que ao destruir um inimigo 50 ponto sao ganhos, entretanto, destruir um combustivel nao lhe proporciona nenhum beneficio.\n");
            printf("ATENCAO: Apertar alguma tecla que nao seja um comando subtrai 2 de combustivel.\n");
            printf("Digite 1 para jogar, ou 2 para sair: ");
            scanf("%d",&jogar);
            printf("\n");
            while(jogar != 1 && jogar != 2){
                printf("Valor invalido, digite 1 para jogar, ou 2 para sair: ");
                scanf("%d",&jogar);
                printf("\n");
            }
            if(jogar==2){
                exit(0);
            }
            else{
                CLEAR;
                break;
            }
    }
}
void menu(){//Basicamente da as opcoes, chamando a devida funcao, recusando comandos invalidos e limpando a tela
    int escolheu;
    while(escolheu != 1){
        CLEAR;
        printf("Main Menu\n");
        printf("1 - Jogar\n");
        printf("2 - Configuracoes\n");
        printf("3 - Ranking\n");
        printf("4 - Instrucoes\n");
        printf("5 - Sair\n\n");
        printf("Escolha uma opcao: ");
        scanf("%d",&escolheu);
        while(escolheu != 1 && escolheu != 2 && escolheu != 3 && escolheu != 4 && escolheu != 5){
            printf("Opcao invalida, digite uma opcao valida.\n");
            scanf("%d",&escolheu);
            printf("\n");
        }
        switch (escolheu){
            case 1 :
                CLEAR;
                break;
            case 2 :
                opcao(2);
                break;
            case 3 :
                opcao(3);
                break;
            case 4 :
                opcao(4);
                break;
            case 5 :
                exit(0);
        }
    }
}
void map(){//define o mapa e as bordas
    int i;
    for(i=0;i<altura;i++){
        for(int j=0;j<largura;j++){
            tabuleiro[i][j] = ' ';
            if(i==(altura/2) && j==0 ){
                tabuleiro[i][j] = '+';
            }
            if(i==0){
                borda[i][j] = '#';
            }
        }
    }
}
void spawn(){//Cuida do surgimento dos NPCs
    int random;
    if((rand()%100)<probX){//Chance de inimigo X
        tabuleiro[rand()%altura][largura-1] = 'X';
    }
    if((rand()%100)<probF){//Chance de combustivel
        tabuleiro[rand()%altura][largura-1] = 'F';
    }
    if((rand()%100)<probO){//Chance de inimigo O
        random = rand()%altura;//Posicao vertical em que o inimigo surgira
        tabuleiro[random][largura-1] = 'O';
        O[contador].hp = vidas;//Inicializa a vida de acordo com a configuracao
        O[contador].x = largura-1;//Inicializa a posicao na coluna mais a direita
        O[contador].y = random; //Inicializa a posicao na linha aleatoria escolhida
        contador++;
    }
    if((rand()%100)<probT){//Chance de inimigo T
        random = rand()%altura;
        tabuleiro[random][largura-1] = 'T';
        T[contador2].municao = ammo;//Inicializa a municao
        T[contador2].x = largura-1;//Inicializa a posicao na coluna mais a direita
        T[contador2].y = random;//Inicializa a posicao na linha aleatoria escolhida
        contador2++;
    }
}
void colidiu(){//Procura o personagem e verifica se houve uma colisao
    int i=0;
    if(tabuleiro[0][0] == '+' && (tabuleiro[0][1]=='X' || tabuleiro[0][1]=='F' || tabuleiro[0][1] == 'T' || tabuleiro[0][1] == '<')){
        if(tabuleiro[0][1]=='F'){
            combustivel += 40;
            tabuleiro[0][1] = ' ';
        }
        else if(tabuleiro[0][1] == 'X' || tabuleiro[0][1] == 'T' || tabuleiro[0][1] == '<' || tabuleiro[0][2] == '<'){
            colisao++;
        }
        else if(tabuleiro[0][1] == 'O'){
            tabuleiro[0][1] = ' ';
        }
    }
    while(tabuleiro[i][0] != '+'){
        if(tabuleiro[i+1][0] == '+'){
            if(tabuleiro[i+1][1] == 'X' || tabuleiro[i+1][1] == 'T' || tabuleiro[i+1][1] == '<'){
                colisao++;
            }
            else if(tabuleiro[i+1][1] == 'F'){
                combustivel += 40;
                tabuleiro[i+1][1]=' ';
            }
            else if(tabuleiro[i+1][1] == 'O'){
                tabuleiro[i+1][1] = ' ';
            }
        }
        i++;
    }
}
void morteO(){//Qnd o O morre, limpa todos os X da tela
    int i, j;
    for(i=0;i<altura;i++){
        for(j=largura - 1;j>0;j--){
            if(tabuleiro[i][j] == 'X'){
                tabuleiro[i][j] = ' ';
                pontuacao += 10;
            }
        }
    }
}
void moverTiro(){//Move o tiro, eh chamada a cada frame pela funcao jogo
    int i, j, k;
    for(i=0;i<altura;i++){
        for(j=largura-1;j>0;j--){//Procura o tiro
            if(tabuleiro[i][j] == '>'){//Quando acha
                if((tabuleiro[i][j+1]=='F') || (tabuleiro[i][j+1]=='X') || (tabuleiro[i][j+1]=='T') || (tabuleiro[i][j+1]=='<') || (tabuleiro[i][j+1]=='O')){//se tiver alguma coisa na frente zera ambos
                    if(tabuleiro[i][j+1] == 'O'){//A menos que seja um O
                        tabuleiro[i][j] = ' ';
                        k = -1;
                        do{
                            k++;
                            if(O[k].x == j+1 && O[k].y == i){
                                O[k].hp--;
                                if(O[k].hp <= 0){
                                    tabuleiro[i][j+1] = ' ';
                                    morteO();
                                }
                            }
                        }while(O[k].x != j+1);
                    }
                    else{
                        if(tabuleiro[i][j+1]=='X'){
                            pontuacao+=50;
                        }
                        tabuleiro[i][j+1]=' ';
                        tabuleiro[i][j]=' ';
                    }
                }
                else{//senao, o tiro continua
                    tabuleiro[i][j+1]='>';
                    tabuleiro[i][j]=' ';
                }
            }
        }
    }
}
void arraste(){//Arrasta a matriz para a esquerda
    int i, j, k, identificador;
    for(i=0;i<altura;i++){
        for(j=0;j<largura;j++){
            if((tabuleiro[i][j]=='X' || tabuleiro[i][j]=='F' || tabuleiro[i][j]=='T' || tabuleiro[i][j]=='O' || tabuleiro[i][j] == '<') && tabuleiro[i][j-1] != '+'){
                if(tabuleiro[i][j] == 'O' && tabuleiro[i][j-1] == '>'){//Caso o tiro colida com um O
                    k = -1;
                    do{
                        k++;
                        if(O[k].x == j && O[k].y == i){//Basicamente procura qual eh aquele O com base na localizacao dele
                            O[k].hp--;
                            if(O[k].hp <= 0){
                                tabuleiro[i][j] = ' ';
                                tabuleiro[i][j-1] = ' ';
                                morteO();
                            }
                            else{
                                tabuleiro[i][j-1] = 'O';
                            }
                        }
                    }while(O[k].x != j);
                    tabuleiro[i][j] = ' ';
                }
                else if(tabuleiro[i][j-1]=='>'){
                    tabuleiro[i][j] = ' ';
                    tabuleiro[i][j-1] = ' ';
                }
                else{
                    tabuleiro[i][j-1] = tabuleiro[i][j];
                    tabuleiro[i][j] = ' ';
                    if(tabuleiro[i][j-1] == '<'){
                        if(tabuleiro[i][j-2] == ' '){
                            tabuleiro[i][j-1] = ' ';
                            tabuleiro[i][j-2] = '<';
                        }
                        else if(tabuleiro[i][j-3] == ' '){
                            tabuleiro[i][j-1] = ' ';
                            tabuleiro[i][j-3] = '<';
                        }
                        else if(tabuleiro[i][j-4] == ' '){
                            tabuleiro[i][j-1] = ' ';
                            tabuleiro[i][j-4] = '<';
                        }
                    }
                }
            }
        }
    }
    if(contador>0){//Diminui o x de todas as structs O
        for(identificador=0;identificador<contador;identificador++){
            O[identificador].x--;
        }
    }
    if(contador2>0){//Diminui o x de todas as structs T
        for(identificador=0;identificador<contador2;identificador++){
            T[identificador].x--;
        }
    }
}
void show(){//printa o mapa na tela, e chamada a cada loop, depois da arraste.
    int i, j;
    printf("Combustivel: %d               Pontos: %d\n",combustivel,pontuacao);
    for(i=0;i<largura;i++){
        printf("%c",borda[0][i]);
    }
    printf("\n");
    for(i=0;i<altura;i++){
        for(j=0;j<largura;j++){
            printf("%c",tabuleiro[i][j]);
        }
        printf("\n");
    }
    for(i=0;i<largura;i++){
        printf("%c",borda[0][i]);
    }
    printf("\n");
    if(contador2>0){
        printf("\n\nMunicao restante do primeiro T = %d\n",T[0].municao);
    }
}
void queTiroFoiEsse(){//Cria os tiros do personagem =D, eh chamada no final da funcao moverPersonagem.
    int i=0;
    if(tabuleiro[0][0] == '+'){//Caso especial para a primeira linha, pois se deixasse apenas o loop ele nao seria executado 
        if(tabuleiro[0][1]=='X' || tabuleiro[0][1]=='F'){//Caso especifico quando o inimigo esta na casa imediatamente a direita do personagem
            tabuleiro[0][1]=' ';
        }
        else{
            tabuleiro[0][1]='>';
        }
    }
    for(i=0;tabuleiro[i][0] != '+';i++){//Procura o personagem
        if(tabuleiro[i+1][0] == '+'){//Quando acha
            if(tabuleiro[i+1][1]=='X' || tabuleiro[i+1][1]=='F'){
            tabuleiro[i+1][1]=' ';
            }
            else{
            tabuleiro[i+1][1]='>';
            }
        }
    }
    combustivel -= 2;//Note que o tiro retira 3 unidades de combustivel, pois a funcao jogo ja tira 1 a cada 'frame'.
}
void tiroInimigo(){//Cria os tiros dos inimigos tipo T
    int i, j, taxa, k;
    taxa = (((ammo*100)/largura)/100)+1;//Cria uma probabilidade razoavel para que o T atire
    for(i=0;i<altura;i++){
        for(j=0;j<largura-1;j++){
            if(tabuleiro[i][j] == 'T'){
                k = - 1;
                do{
                    k++;
                    if(T[k].x == j && T[k].y == i){//Basicamente procura qual eh aquele T com base na localizacao dele
                        if(T[k].municao > 0){//Se tiver municao
                            if(rand()%100<(taxa*3)){//Tem uma chance de atirar
                                if(tabuleiro[i][j-1] == ' '){//Se o espaco estiver livre ele ja cria o tiro
                                    tabuleiro[i][j-1] = '<';
                                    T[k].municao--;
                                }
                                else if(tabuleiro[i][j-2] == ' '){//Se nao estiver ele procura algum que esteja, usei apenas 3 if's porque dificilmente havera mais de 3 inimigos na frente do T quando ele atirar
                                    tabuleiro[i][j-2] = '<';
                                    T[k].municao--;
                                }
                                else if(tabuleiro[i][j-3] == ' '){
                                    tabuleiro[i][j-3] = '<';
                                    T[k].municao--;
                                }
                            }
                        }
                    }
                }while(T[k].x != j);
            }
        }
    }
}
void moverPersonagem(){//Move o personagem. Identifica quando uma tecla e pressionada e qual, depois procura o + e move.
    int i=0;
    char apertou;
    if(kbhit() == 1){
        apertou = getch();
        if((apertou == 'w'|| apertou == 'W') && tabuleiro[0][0] != '+'){//Se apertou W ele move pra cima
            while(tabuleiro[i][0] != '+'){//Procura o personagem na coluna
                if(tabuleiro[i+1][0] == '+'){//Quando acha 
                    tabuleiro[i][0] = '+';//Move pra cima
                }
                i++;
            }
            tabuleiro[i][0] = ' ';//Fica fora do while se nao ele troca o personagem de lugar e nunca acha cade ele, ai da problema
            combustivel--;//Note que como a cada 'frame' um combustivel ja e retirado pela funcao jogo, a acao de mover so precisa retirar mais 1 para que se tire 2 naquele frame.
        }
        else if((apertou == 's' || apertou == 'S') && tabuleiro[0][0] == '+'){
            tabuleiro[0][0] = ' ';
            tabuleiro[1][0] = '+';
        }
        else if((apertou == 's' || apertou == 'S') && tabuleiro[altura-1][0] != '+'){//Se apertou S ele move pra baixo
            while(tabuleiro[i][0] != '+'){//mesma coisa do while do W
                if(tabuleiro[i+1][0] == '+'){
                    tabuleiro[i+2][0] = '+';
                }
                i++;
            }
            tabuleiro[i][0] = ' ';
            combustivel--;
        }
        else if(apertou == 'd' || apertou == 'D'){
            queTiroFoiEsse();
        }
    }
}
void jogo(){//Realiza o jogo de fato
    map();//Monta o mapa
    while(colisao == 0 && combustivel > 0){
        CLEAR;
        colidiu();//Eh chamada antes das outras porque a cada frame ele verifica se o proximo char eh um inimigo/F e depois move, assim ele ve se colidiu.
        tiroInimigo();
        moverTiro();
        arraste();
        moverPersonagem();
        spawn();
        show();
        usleep(velocidade);
        pontuacao++;
        combustivel--;
    }
    CLEAR;
    printf("GAME OVER\n");
    printf("Pontuacao: %d\n",pontuacao );
    if(colisao>0){
        printf("Morte por Colisao\n");
    }
    if(combustivel <= 0){
        printf("Sem combustivel\n");
    }
    printf("Deseja jogar novamente?\nDigite 0 para sair, ou 1 para jogar: ");
    vicio = 2;
    while(vicio != 0 && vicio != 1){
        scanf("%d",&vicio);
    }
    CLEAR;
}
int main(){
    while(vicio == 1){
        srand(time(0));//especificado pelo trabalho
        CLEAR;
        welcome();
        CLEAR;
        leituraDosArquivos();
        menu();
        leituraDosArquivos();
        jogo();
        colisao = 0;
        combustivel = 400;
        pontuacao = 0;
    }
    return 0;
}