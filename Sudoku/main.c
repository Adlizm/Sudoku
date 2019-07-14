/**
TRABALHO DE PROGAMAÇAO PROCEDIMENTAL:SUDOKU
GRUPO PAP
INTEGRANTES:
       PAULO CÉSAR GARCIA JÚNIOR ,         MATRÍCULA:11811BCC003
       ANDRÉ LUIZ MORAIS PERES DE QUINTA   MATRÍCULA:11821BCC003
       PEDRO HENRIQUE DA SILVA OLIVEIRA    MATRÍCULA:11811BCC040
*/

#include <stdio.h>
#include <stdlib.h>

//caixinha 1x1 do sudoku
struct block{
    int valor; //valor colocado dentro da caixinha
    int mutavel; //o valor dessa caixinha pode ser mudado ou nao
    int possibilidades[9];//valores que podem ser colocados nessa caixa
    int qntPos;//quantidade de possibilidades de valores que pode ser colocados nessa caixa
};
struct sudoku{
    struct block Sudoku[9][9]; //sudoku 9x9 atual
    int *valorTestar; //vetor com os valores para testar  no sudoku atual
    int Lmenor,Cmenor; //linha e coluna da caixa com a menor quantidade de possibilidades no sudoku atual
    int qntPos;//quantiidade de possibilidades a testar no sudoku atual
};
struct sudoku *S;//vetor com sudokus salvos ao longo do programa
int ok;//status do programa{
            //0:testa um sudoku
            //1:sudoku resolvido
            //2:sudoku atual invalido(nao possui solucao)
        //}

//ler um arquivo
void le_arquivo(char *nome, int mat[][9]){
    FILE *arq = fopen(nome,"r");
    if(arq == NULL){
        printf("Erro de leitura\n");
        system("pause");
        exit(1);
    }

    int i,j;
    for(i=0; i< 9; i++)
        for(j=0; j< 9; j++)
            fscanf(arq,"%d",&mat[i][j]);

    fclose(arq);
}

//verifica se o sudoku esta completo
int verificaSudoku(struct block Sudoku[9][9]){
    for(int i=0;i<9;i++){
        for(int f=0;f<9;f++){
            if(Sudoku[i][f].valor == 0){
                    return 0; //sudoku nao concluido;
            }
        }
    }
    return 1; //solucao do sudoku encontrada;
}

//verifica se o valor pode ser colocado na linha
int vValorLinha(struct block Sudoku[9][9], int lin, int valor){
    for(int i=0;i<9;i++){
        if(Sudoku[lin][i].valor == valor){
            return 0; // esse valor nao pode ser colocado nessa linha
        }
    }
    return 1;//verdadeiro = valor pode ser colocado nessa linha
}

//verfica se o valor pode ser colocado na coluna
int vValorColuna(struct block Sudoku[9][9], int col, int valor){
    for(int i=0;i<9;i++){
        if(Sudoku[i][col].valor == valor){
            return 0; // esse valor nao pode ser colocado nessa coluna
        }
    }
    return 1;//verdadeiro = valor pode ser colocado nessa coluna
}

//verfica se o valor pode ser colocado no bloco 3x3
int vValorRegiao(struct block Sudoku[9][9], int lin, int col, int valor){
    lin = (lin/3)*3;
    col = (col/3)*3;
    for(int i=lin;i<lin+3;i++){
        for(int f=col;f<col+3;f++){
            if(Sudoku[i][f].valor == valor){
                return 0; // esse valor nao pode ser colocado nesse quadrado 3x3
            }
        }
    }
    return 1; //verdadeiro = valor pode ser colocado nesse quadrado 3x3
}

//calcula todas as possibilidades de cada caixinha vazia no sudoku
void calculaPossibilidades(struct block Sudoku[9][9]){
    for(int i=0;i<9;i++){
        for(int f=0;f<9;f++){
            if(Sudoku[i][f].mutavel){
                int pos = 0;
                for(int num=1;num<=9;num++){
                    if(vValorColuna(Sudoku,f,num) && vValorLinha(Sudoku,i,num) && vValorRegiao(Sudoku,i,f,num)){
                        Sudoku[i][f].possibilidades[pos] = num;
                        pos++;
                    }
                }
                if(pos != 0){
                    Sudoku[i][f].qntPos = pos;
                }else{
                    ok = 2;
                    break;
                }
            }
        }
        if(ok == 2){
            break;
        }
    }
}

//coloca os valores de um sudoku em outro
void colocaValores(struct block Sudoku[9][9],int Sudo[9][9]){
    for(int i=0;i<9;i++){
        for(int f=0;f<9;f++){
            if(Sudo[i][f] != 0){
                Sudoku[i][f].mutavel = 0;//valor nao pode ser mudado
            }else{
                Sudoku[i][f].mutavel = 1;//valor pode ser mudado
            }
            Sudoku[i][f].valor = Sudo[i][f];
        }
    }
}

int main(){
    int sudoAux[9][9];
    int PA = 1;//posicao atual do vetor com sudokus salvos
    int menorPosI;//linha com menor possiblidade s
    int menorPosF;//coluna com menor possibilidades
    int menorPos;//menor quantidade de possibilidades

    S = (struct sudoku *) malloc(PA * sizeof(struct sudoku));
    le_arquivo("sudokuMD.txt", sudoAux); //le arquivo sudoku e coloca os valores no sudoAux
    colocaValores(S[PA - 1].Sudoku,sudoAux);//coloca os valores de sudoAux no Sudoku atual

    do{
        ok = 0;
        do{
            calculaPossibilidades(S[PA - 1].Sudoku);//calcula as polibilidades de cada caixinha no sudoku atual
            if(ok != 2){
                menorPos = 10;
                for(int i=0;i<9;i++){
                    for(int f=0;f<9;f++){
                        if(menorPos > S[PA - 1].Sudoku[i][f].qntPos && S[PA - 1].Sudoku[i][f].mutavel == 1 ){
                            menorPos = S[PA - 1].Sudoku[i][f].qntPos; //menor quantidades de possibilidades
                            menorPosI = i;  //linha com a menor quantidade de possibilidades
                            menorPosF = f;  //coluna com a menor quantidade de possibilidades
                        }
                    }
                }
                if(menorPos == 1){//se so possuir uma possibilidade coloca o valor no sudoku atual
                    S[PA - 1].Sudoku[menorPosI][menorPosF].valor = S[PA - 1].Sudoku[menorPosI][menorPosF].possibilidades[0];
                    S[PA - 1].Sudoku[menorPosI][menorPosF].mutavel = 0;
                }else if(menorPos > 1){
                    PA++;
                    S = realloc(S,PA * sizeof(struct sudoku));//adiciona uma posição ao vetor de sudokus salvos
                    S[PA - 1].Lmenor = menorPosI;
                    S[PA - 1].Cmenor = menorPosF;
                    S[PA - 1].qntPos = menorPos;
                    S[PA - 1].valorTestar = (int *) malloc(menorPos * sizeof(int));
                    for(int i=0;i<menorPos;i++){
                        S[PA - 1].valorTestar[i] = S[PA - 2].Sudoku[menorPosI][menorPosF].possibilidades[i]; //coloca as possibilidades nos valores a serem testados
                    }
                    for(int i=0;i<9;i++){
                        for(int f=0;f<9;f++){
                            sudoAux[i][f] = S[PA - 2].Sudoku[i][f].valor;
                        }
                    }
                    colocaValores(S[PA - 1].Sudoku,sudoAux);//coloca os valores do sudoku salvo anterior no atual
                    S[PA - 1].qntPos--;
                    S[PA - 1].Sudoku[menorPosI][menorPosF].valor = S[PA - 1].valorTestar[S[PA - 1].qntPos];//coloca um valor das possibilidades para testar o sudoku
                    S[PA - 1].Sudoku[menorPosI][menorPosF].mutavel = 0;
                }

                if(verificaSudoku(S[PA - 1].Sudoku)){
                    ok=1;//solucao foi encontrada
                }
                //printa o sudoku na tela
                for(int i=0;i<9;i++){
                    for(int f=0;f<9;f++){
                        printf("%d ",S[PA - 1].Sudoku[i][f].valor);
                    }
                    printf("\n");
                }
                system("cls");//limpa a tela
            }
        }while(ok == 0);
        if(ok == 2){ //se o sudoku atual e invalido
            if(PA == 1){ //se a posicao invalida e a primeira implica que o sudoku nao possui uma solucao
                system("cls");
                printf("Sudoku nao possui solucao");
                exit(0);//encera o programa
            }
            if(S[PA - 1].qntPos > 0){//se possui mais possibilidades para serem testadas no sudoku atual
                                     //entao coloca o valores da posicao anterior e coloca no sudokou atual e testa uma possibilidade
                for(int i=0;i<9;i++){
                    for(int f=0;f<9;f++){
                        sudoAux[i][f] = S[PA - 2].Sudoku[i][f].valor;
                    }
                }
                colocaValores(S[PA - 1].Sudoku,sudoAux);
                S[PA - 1].qntPos--;
                S[PA - 1].Sudoku[S[PA - 1].Lmenor][S[PA - 1].Cmenor].valor = S[PA - 1].valorTestar[S[PA - 1].qntPos];//coloca uma possibilidade para ser testada
                S[PA - 1].Sudoku[S[PA - 1].Lmenor][S[PA - 1].Cmenor].mutavel = 0;
            }else{//nao possui mais possibillidades para testar no sudoku atual
                do{//procura por uma posicao no vetor de sudokus que tem possilidades para serem testadas
                    PA--;
                }while(S[PA - 1].qntPos == 0);
                if(PA == 1){//se a posicao invalida e a primeira implica que o sudoku nao possui uma solucao
                    system("cls");
                    printf("Sudoku nao possui solucao");
                    exit(0);//encerra o programa

                }
                //coloca o valores da posicao anterior e coloca no sudokou atual e testa uma possibilidade
                for(int i=0;i<9;i++){
                    for(int f=0;f<9;f++){
                            sudoAux[i][f] = S[PA - 2].Sudoku[i][f].valor;
                        }
                    }
                colocaValores(S[PA - 1].Sudoku,sudoAux); //coloca os valores do sudoku anterior no atual
                S[PA - 1].qntPos--;
                S[PA - 1].Sudoku[S[PA - 1].Lmenor][S[PA - 1].Cmenor].valor = S[PA - 1].valorTestar[S[PA - 1].qntPos];//coloca uma possibilidade para ser testada
                S[PA - 1].Sudoku[S[PA - 1].Lmenor][S[PA - 1].Cmenor].mutavel = 0;
                S = realloc(S,PA*sizeof(struct sudoku));
            }
        }
    }while(ok == 2);
    //imprime a solucao do sudoku na tela
    for(int i=0;i<9;i++){
        for(int f=0;f<9;f++){
            printf("%d ",S[PA - 1].Sudoku[i][f].valor);
        }
        printf("\n");
    }

    free(S);//libera a memoria do vetor de sudokus salvos

    printf("\n Solucao Encontrada!\n\n");
    return 0;
}
