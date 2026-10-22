#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <string>
#include <conio.h>
#include <windows.h>
using namespace std;

const int TAM = 20; 

struct Posicao {
    int lin, col;
};

struct Jogo {
    int cenario[TAM][TAM]; 
    int n; 
    int orientacao = 0; 
    Posicao jogador;
    int movimentos = 0, rotacoes = 0;
    int numeroMapa;
    string titulo;
    int item_em_baixo;
};

struct Pontuacao {
    string nome;
    int movimentos;
};


// MÓDULO DE ACESSO A ARQUIVOS (Apenas lê e grava, sem imprimir e sem regras)


bool CarregarJogo(int numero_mapa, Jogo &j){
    string NomeMapa = "mapa" + to_string(numero_mapa) + ".csv";
    ifstream arq(NomeMapa);
    if(!arq.is_open()){
        return false;
    }
    string frase;
    try {
        if (!getline(arq, frase)) return false;
        j.n = stoi(frase);
        if (!getline(arq, frase)) return false;
        j.titulo = frase;

        j.orientacao = 0;
        j.movimentos = 0;
        j.rotacoes = 0;
        j.item_em_baixo = 0;

        for(int i = 0; i < j.n; i++){
            if (!getline(arq, frase)) return false;
            stringstream ss(frase);
            string celula;
            int c = 0;
            while(getline(ss, celula, ',')){
                j.cenario[i][c] = stoi(celula);
                if(j.cenario[i][c] == 2){
                    j.jogador.lin = i;
                    j.jogador.col = c;
                }
                c++;
            }
        }
    } catch (...) {
        arq.close();
        return false;
    }
    arq.close();
    return true;
}

bool CarregarJogoSalvo(Jogo &j){
    ifstream arq("jogo_salvo.csv");
    if(!arq.is_open()){
        return false;
    }
    string frase;
    try {
        if (!getline(arq, frase)) return false;
        j.n = stoi(frase);
        if (!getline(arq, frase)) return false;
        j.orientacao = stoi(frase);
        if (!getline(arq, frase)) return false;
        j.jogador.lin = stoi(frase);
        if (!getline(arq, frase)) return false;
        j.jogador.col = stoi(frase);
        if (!getline(arq, frase)) return false;
        j.movimentos = stoi(frase);
        if (!getline(arq, frase)) return false;
        j.rotacoes = stoi(frase);
        if (!getline(arq, frase)) return false;
        j.numeroMapa = stoi(frase);
        if (!getline(arq, frase)) return false;
        j.titulo = frase;
        if (!getline(arq, frase)) return false;
        j.item_em_baixo = stoi(frase);
        for(int i = 0; i < j.n; i++){
            if (!getline(arq, frase)) return false;
            stringstream ss(frase);
            string celula;
            int c = 0;
            while(getline(ss, celula, ',')){
                j.cenario[i][c] = stoi(celula);
                if(j.cenario[i][c] == 2){
                    j.jogador.lin = i;
                    j.jogador.col = c;
                }
                c++;
            }
        }
    } catch (...) {
        arq.close();
        return false;
    }
    arq.close();
    return true;
}

bool SalvarJogo(const Jogo &j){
    ofstream meu_arquivo;
    meu_arquivo.open("jogo_salvo.csv");
    if (!meu_arquivo.is_open()) return false;
    meu_arquivo << j.n << "\n";
    meu_arquivo << j.orientacao << "\n";
    meu_arquivo << j.jogador.lin << "\n";
    meu_arquivo << j.jogador.col << "\n";
    meu_arquivo << j.movimentos << "\n";
    meu_arquivo << j.rotacoes << "\n";
    meu_arquivo << j.numeroMapa << "\n";
    meu_arquivo << j.titulo << "\n";
    meu_arquivo << j.item_em_baixo << "\n";
    for(int i = 0; i < j.n; i++){
        for(int c = 0; c < j.n; c++){
            meu_arquivo << j.cenario[i][c];
            if(c < j.n - 1){
                meu_arquivo << ",";
            }
        }
        meu_arquivo << "\n";
    }
    meu_arquivo.close();
    return true;
}

bool LerArquivoSolucao(int numero_mapa, string &teclasRetorno) {
    string NomeMapa = "mapa" + to_string(numero_mapa) + ".sol";
    ifstream arq(NomeMapa);
    if(!arq.is_open()){
        return false;
    }
    getline(arq, teclasRetorno);
    arq.close();
    return true;
}

int CarregarRanking(int numero_mapa, Pontuacao ranking[], int max_tam) {
    string nomeArquivo = "ranking" + to_string(numero_mapa) + ".csv";
    ifstream arq(nomeArquivo);
    if (!arq.is_open()) return 0;

    string linha;
    int qtd = 0;
    while (qtd < max_tam && getline(arq, linha)) {
        stringstream ss(linha);
        string nome, movStr;
        if (getline(ss, nome, ',') && getline(ss, movStr, ',')) {
            ranking[qtd].nome = nome;
            try {
                ranking[qtd].movimentos = stoi(movStr);
                qtd++;
            } catch (...) {}
        }
    }
    arq.close();
    return qtd;
}

bool SalvarRanking(int numero_mapa, const Pontuacao ranking[], int qtd) {
    string nomeArquivo = "ranking" + to_string(numero_mapa) + ".csv";
    ofstream arq(nomeArquivo);
    if (!arq.is_open()) return false;

    for (int i = 0; i < qtd; i++) {
        arq << ranking[i].nome << "," << ranking[i].movimentos << "\n";
    }
    arq.close();
    return true;
}


// MÓDULO DE UTILITÁRIOS DE TELA E TERMINAL


int getch() {
    return _getch();
}

void PosicionarCursor(int XPos, int YPos) {
    printf("\033[%d;%dH", YPos + 1, XPos + 1);
}

void alternarCursor(bool exibir) {
    if (exibir) {
        printf("\033[?25h");
    } else {
        printf("\033[?25l");
    }
}

void limparTela() {
    (void)system("cls");
}


// MÓDULO DE REGRAS DE JOGO (Altera o struct Jogo, não imprime e não acessa arquivo)


void gravidade(Jogo &j) {
    for (int l = j.n - 2; l >= 0; l--) {
        for (int c = 0; c < j.n; c++) {
            if (j.cenario[l][c] == 3) {
                int pos = l;
                while (pos < j.n - 1 && j.cenario[pos + 1][c] == 0) {
                    j.cenario[pos + 1][c] = 3;
                    j.cenario[pos][c] = 0;
                    pos++;
                }
            }
        }
    }
}

void moverJogador(Jogo &j, char x) {
    j.movimentos++;
    int deltaLinha = 0;
    int deltaColuna = 0;
    
    if(x == 'w' || x == 'W'){deltaLinha = -1; deltaColuna = 0;}
    if(x == 'a' || x == 'A'){deltaLinha = 0; deltaColuna = -1;}
    if(x == 's' || x == 'S'){deltaLinha = 1; deltaColuna = 0;}
    if(x == 'd' || x == 'D'){deltaLinha = 0; deltaColuna = 1;}

    j.cenario[j.jogador.lin][j.jogador.col] = j.item_em_baixo;
    int novaLinha = j.jogador.lin + deltaLinha;
    int novaColuna = j.jogador.col + deltaColuna;

    if(novaLinha < 0 || novaLinha >= j.n || novaColuna < 0 || novaColuna >= j.n){
        j.cenario[j.jogador.lin][j.jogador.col] = 2;
        j.movimentos--;
        return;
    }

    int destino = j.cenario[novaLinha][novaColuna];
    bool portaFechada = false;

    if (destino == 6) {
        portaFechada = (j.rotacoes % 2 == 0); 
    } else if (destino == 7) {
        portaFechada = (j.rotacoes % 2 == 1); 
    }

    if (destino == 1 || destino == 3 || portaFechada) {
        j.cenario[j.jogador.lin][j.jogador.col] = 2;
        j.movimentos--;
    } else {
        j.jogador.lin = novaLinha;
        j.jogador.col = novaColuna;
        j.item_em_baixo = j.cenario[j.jogador.lin][j.jogador.col];
        j.cenario[j.jogador.lin][j.jogador.col] = 2;
    }
}

void rotacionarMapa(Jogo &j, bool paraDireita) {
    if (j.item_em_baixo != 4) return; 
    int transposta[20][20];
    j.cenario[j.jogador.lin][j.jogador.col] = j.item_em_baixo;

    for (int l = 0; l < 20; l++) {
        for (int c = 0; c < 20; c++) {
            if (paraDireita) {
                transposta[c][19 - l] = j.cenario[l][c];
            } else {
                transposta[19 - c][l] = j.cenario[l][c];
            }
        }
    }
    if(paraDireita){
        j.orientacao = j.orientacao + 1;
    } else {
        j.orientacao = j.orientacao - 1;
        if (j.orientacao < 0) {
            j.orientacao = 3;
        }
    }

    for (int l = 0; l < j.n; l++) {
        for (int c = 0; c < j.n; c++) {
            j.cenario[l][c] = transposta[l][c];
        }
    }

    int old_pli = j.jogador.lin;
    if (paraDireita) {
        j.jogador.lin = j.jogador.col;
        j.jogador.col = 19 - old_pli;
    } else {
        j.jogador.lin = 19 - j.jogador.col;
        j.jogador.col = old_pli;
    }

    j.item_em_baixo = j.cenario[j.jogador.lin][j.jogador.col];
    j.cenario[j.jogador.lin][j.jogador.col] = 2;
    j.rotacoes++;
    gravidade(j);
}

bool verificarVitoria(const Jogo &j) {
    if (j.item_em_baixo == 5) {
        return true;
    }
    return false;
}

void ProcessaTecla(Jogo &j, char x){
    switch (x) {
        case 'W':
        case 'w': moverJogador(j, x); break;
        case 'S':
        case 's': moverJogador(j, x); break;
        case 'A':
        case 'a': moverJogador(j, x); break;
        case 'D':
        case 'd': moverJogador(j, x); break;
        case 'E':
        case 'e': rotacionarMapa(j, true); break; 
        case 'Q':
        case 'q': rotacionarMapa(j, false); break; 
    }
}


void AtualizarRanking(int numero_mapa, string nome_jogador, int movimentos) {
    const int MAX = 10;
    Pontuacao ranking[MAX];
    int qtd = CarregarRanking(numero_mapa, ranking, MAX);

    // Adiciona o novo recorde temporariamente
    ranking[qtd].nome = nome_jogador;
    ranking[qtd].movimentos = movimentos;
    qtd++;

    // Ordenação por Insertion Sort baseada em menor quantidade de movimentos
    for (int i = 1; i < qtd; i++) {
        Pontuacao chave = ranking[i];
        int k = i - 1;
        while (k >= 0 && ranking[k].movimentos > chave.movimentos) {
            ranking[k + 1] = ranking[k];
            k--;
        }
        ranking[k + 1] = chave;
    }

    if (qtd > MAX) qtd = MAX;
    SalvarRanking(numero_mapa, ranking, qtd);
}


// MÓDULO DE DESENHO / INTERFACE (Imprime na tela, não altera e não acessa arquivo)


void desenharMapa(const Jogo &j) {
    for (int i = 0; i < j.n; i++) {
        for (int c = 0; c < j.n; c++) {
            switch (j.cenario[i][c]) {
                case 0:
                    cout << " ";
                    break;
                case 1:
                    printf("\033[0;44m ");
                    printf("\033[0m");
                    break;
                case 2:
                    cout << "@";
                    break;
                case 3:
                    cout << "O";
                    break;
                case 4:
                    cout << "A";
                    break;
                case 5:
                    cout << "S";
                    break;
                case 6:
                    if(j.rotacoes % 2 == 1){ cout<<":"; }
                    if(j.rotacoes % 2 == 0){ cout<<"="; }
                    break;
                case 7:
                    if(j.rotacoes % 2 == 1){ cout<<"|"; }
                    if(j.rotacoes % 2 == 0){ cout<<";"; }
                    break;
                default:
                    cout << " ";
            }
        }
        cout << endl;
    }
}

void desenharInterfaceJogo(const Jogo &j) {
    PosicionarCursor(0, 0);
    desenharMapa(j);
    cout << "Quantidade de rotacoes: " << j.rotacoes << endl;
    cout << "Quantidade de movimentos: " << j.movimentos << endl;
    cout << "A orientacao atual e: " << (j.orientacao * 90) << "°" << "\033[K" << endl;
}

void desenharTelaVitoria() {
    limparTela();
    alternarCursor(true);
    cout << "========================================\n";
    cout << "        PARABENS! VOCE VENCEU!          \n";
    cout << "========================================\n\n";
}

void desenharMenuPrincipal() {
    limparTela();
    cout << "CARDGAME" << endl;
    cout << "Pressione o numero indicado para:" << endl;
    cout << "1 - Novo jogo" << endl;
    cout << "2 - Continuar" << endl;
    cout << "3 - Ver solucoes" << endl;
    cout << "4 - Ver Ranking" << endl;
    cout << "5 - Sobre" << endl;
    cout << "6 - Fim" << endl;
    cout << "Escolha uma opcao: ";
}

void desenharMenuSelecaoMapa() {
    limparTela();
    cout << "========================================\n";
    cout << "            SELECAO DE MAPAS            \n";
    cout << "========================================\n";
    cout << "1 - Mapa 1 (Original)\n";
    cout << "2 - Mapa 2 (Corredores em Cruz)\n";
    cout << "3 - Mapa 3 (Labirinto Periférico)\n";
    cout << "4 - Mapa 4 (Desafio dos Blocos)\n";
    cout << "5 - Mapa 5 (Labirinto Avançado)\n";
    cout << "6 - Voltar ao Menu Principal\n";
    cout << "Escolha uma opcao: ";
}

void desenharMenuSolucoes() {
    limparTela();
    cout << "========================================\n";
    cout << "          SELECAO DE SOLUCOES           \n";
    cout << "========================================\n";
    cout << "1 - Solucao Mapa 1\n";
    cout << "2 - Solucao Mapa 2\n";
    cout << "3 - Solucao Mapa 3\n";
    cout << "4 - Solucao Mapa 4\n";
    cout << "5 - Solucao Mapa 5\n";
    cout << "6 - Voltar ao Menu Principal\n";
    cout << "Escolha uma opcao: ";
}

void ExibirRanking(int numero_mapa) {
    limparTela();
    Pontuacao ranking[10];
    int qtd = CarregarRanking(numero_mapa, ranking, 10);

    cout << "========================================\n";
    cout << "       RANKING - MAPA " << numero_mapa << " (MENOS MOVIMENTOS)\n";
    cout << "========================================\n";
    if (qtd == 0) {
        cout << "Nenhum recorde registrado para este mapa ainda.\n";
    } else {
        int posicaoAtual = 1;
        for (int i = 0; i < qtd; i++) {
            // Regra de desempate: se não for o primeiro e tiver a mesma pontuação do anterior, mantém a posição
            if (i > 0 && ranking[i].movimentos != ranking[i - 1].movimentos) {
                posicaoAtual = i + 1;
            }
            cout << posicaoAtual << "º - " << ranking[i].nome << " : " << ranking[i].movimentos << " movimentos\n";
        }
    }
    cout << "\nPressione qualquer tecla para voltar...";
    getch();
}

void exibirSobre() {
    limparTela();
    cout << "Este jogo foi desenvolvido por Heitor Becker Piazera e Alexandre V Valesan na disciplina de algoritmos 2 lecionada por Thiago Felski\n\n";
    cout << "Como jogar?\n";
    cout << "O objetivo do jogo é chegar a saida (s)\n";
    cout << "As alavancas (A) servem como um meio de rotação do mapa tanto para direita ou para esquerda\n";
    cout << "Para acionar a alavanca, basta se posicionar em cima dela e apertar\n";
    cout << "Q - girar para esquerda\n";
    cout << "E - girar para direita\n";
    cout << "Os blocos (O) tem sistema de gravidade, ou seja, quando o mapa gira eles mudam de posição de acordo com a rotação\n";
    cout << "De acordo com a rotação dos mapa as portas se abrirão e outras fecharão\n";
    cout << "Mais comandos\n";
    cout << "R - reset do mapa\n";
    cout << "Esc - pause o jogo\n\n";
    cout << "Digite qualquer número para voltar ao menu\n";
    getch();
    limparTela();
}


// CONTROLADORES DE FLUXO (Integram as sub-rotinas sem violar as regras)


void executarJogo(Jogo &j) {
    alternarCursor(false);
    printf("\033[H\033[J"); 
    char x;
    
    while (true) {
        desenharInterfaceJogo(j);
        
        if (verificarVitoria(j)){
            desenharTelaVitoria();
            cout << "Insira seu nome para o ranking: ";
            string nomeJogador;
            cin >> nomeJogador;
            AtualizarRanking(j.numeroMapa, nomeJogador, j.movimentos);
            
            cout << "\nPressione qualquer tecla para voltar ao menu...";
            remove("jogo_salvo.csv");
            getch();
            limparTela();
            break; 
        }
        char a;
        a = x;
        x = getch();


        if (x == 27) {
            SalvarJogo(j);
            limparTela();
            alternarCursor(true);
            break;
        }

        if (x == 'r' || x == 'R') {
            CarregarJogo(j.numeroMapa, j);
            continue;
        }
        
        ProcessaTecla(j, x);
    }
}

void ReproduzirSolucao(int numero_mapa){
    string teclas;
    if(!LerArquivoSolucao(numero_mapa, teclas)){
        limparTela();
        cout << "Arquivo de solucao nao existente ou invalido!\n";
        cout << "Pressione qualquer tecla para voltar...";
        getch();
        return;
    }
    
    Jogo meuJogo;
    meuJogo.numeroMapa = numero_mapa;
    if(!CarregarJogo(numero_mapa, meuJogo)){
        return;
    }
    
    for(int i = 0; i < teclas.length(); i++){
        limparTela();
        char t = teclas[i];
        if(t != 'w' && t != 'a' && t != 's' && t != 'd' && t != 'e' && t != 'q' &&  t != 'W' && t != 'A' && t != 'S' && t != 'D' && t != 'E' && t != 'Q'){
            cout << "Esse conjunto solucao apresenta uma tecla invalida, diferente do padrao";
            getch();
            return;
        }
        ProcessaTecla(meuJogo, t);
        PosicionarCursor(0, 0);
        desenharMapa(meuJogo);
        Sleep(300); 
    
        if (verificarVitoria(meuJogo)) {
            cout << "\nSolucao valida! O jogador chegou a saida" << endl;
            cout << "Total de teclas: " << (i + 1) << endl;
            cout << "Movimentos: " << meuJogo.movimentos << " | Rotacoes: " << meuJogo.rotacoes << endl;
            cout << "Pressione qualquer tecla para voltar...";
            getch();
            return; 
        }
    }
    cout << "\nSolucao invalida! O jogador não chegou a saida" << endl;
    getch();
    return;
}

void menuSelecaoMapa() {
    desenharMenuSelecaoMapa();
    int escolhaMapa;
    cin >> escolhaMapa;
    cin.ignore(10000, '\n');

    limparTela();
    if(escolhaMapa >= 1 && escolhaMapa <= 5){
        Jogo meuJogo;
        meuJogo.numeroMapa = escolhaMapa;
        if(CarregarJogo(escolhaMapa, meuJogo)){
            executarJogo(meuJogo);
        } else {
            cout << "Erro ao carregar o arquivo do mapa!\n";
            cout << "Pressione qualquer tecla...";
            getch();
        }
    } else if (escolhaMapa != 6) {
        limparTela();
        cout << "Essa opcao é invalida, por favor insira uma opcao valida entre 1 a 6!!" << endl;
        cout << "Pressione qualquer tecla para continuar...";
        getch();
        limparTela();
    }
}

void menuSolucoes() {
    limparTela();
    desenharMenuSolucoes();
    int escolha_solucao;
    cin >> escolha_solucao;
    cin.ignore(10000, '\n');
    if(escolha_solucao >= 1 && escolha_solucao <= 5){
        ReproduzirSolucao(escolha_solucao);
    }
}

void menuRanking() {
    limparTela();
    cout << "========================================\n";
    cout << "          SELECIONE O RANKING           \n";
    cout << "========================================\n";
    cout << "1 - Mapa 1\n";
    cout << "2 - Mapa 2\n";
    cout << "3 - Mapa 3\n";
    cout << "4 - Mapa 4\n";
    cout << "5 - Mapa 5\n";
    cout << "6 - Voltar\n";
    cout << "Escolha uma opcao: ";
    int escolha;
    cin >> escolha;
    if (escolha >= 1 && escolha <= 5) {
        ExibirRanking(escolha);
    }
}


// FUNÇÃO MAIN (Curta, apenas chamando sub-rotinas de menu)

int main() {
    int escolha_menu1;

    while (true) {
        desenharMenuPrincipal();
        cin >> escolha_menu1;
        cin.ignore(10000, '\n');

        if (escolha_menu1 == 1) {
            menuSelecaoMapa();
        } else if (escolha_menu1 == 2) {
            Jogo meuJogo;
            if(CarregarJogoSalvo(meuJogo)){
                executarJogo(meuJogo);
            } else {
                limparTela();
                cout << "Nenhum jogo salvo foi encontrado ou arquivo corrompido" << "\n";
                cout << "Pressione qualquer tecla para retornar ao menu" << "\n";
                getch();
            }
        } else if (escolha_menu1 == 3) {
            menuSolucoes();
        } else if (escolha_menu1 == 4) {
            menuRanking();
        } else if (escolha_menu1 == 5) {
            exibirSobre();
        } else if (escolha_menu1 == 6) {
            return 0;
        }
    }
}
