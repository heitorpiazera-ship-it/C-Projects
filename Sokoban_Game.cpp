#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <chrono>//Para o tempo
#include <thread>//Para o tempo
using namespace std;
int getch(void) {
  int ch;
  struct termios oldt;
  struct termios newt;
  tcgetattr(STDIN_FILENO, &oldt); // guarda as configurações antigas
  newt = oldt; // copia as configurações antigas para as novas
  newt.c_lflag &= ~(ICANON | ECHO); // faz uma mudança nas novas configurações
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); // aplica as novas configurações
  ch = getchar();                          // pega a tecla
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restaura as configurações antigas
  return ch;                               // retorna o caractere lido
}
void SetCursorPos(int XPos, int YPos) {
  printf("\033[%d;%dH", YPos + 1, XPos + 1);
}
int main() {
    int escolhaMenu;
    int escolhaMapa;
    bool WinMapa1 = false;
    bool WinMapa2 = false;
    
    while(true){
    cout<<"SOKOBAN"<<endl;
    cout<<"Pressione o número indicado para:"<<endl;
    cout<<"1 - Jogar"<<endl;
    cout<<"2 - Desenvolvedores"<<endl;
    cout<<"3 - Orientações para jogar"<<endl;
    cout<<"4 - Sair"<<endl;
    cin>>escolhaMenu;
    if(escolhaMenu == 1){
        cout<<"Digite:"<<endl;
        cout<<"1 - Mapa nivel 3 Sokoban";
        if(WinMapa1 == true){
            cout<<"<solucionado>"<<endl;
        }
        cout<<endl;
        cout<<"2 - Mapa nível 1 Sokoban";
        if(WinMapa2 == true){
            cout<<"<solucionado>"<<endl;
        }
        cin>>escolhaMapa;
    }
    if(escolhaMapa == 1){
    (void)system("clear");   
  printf("\033[?25l"); //código para o desativar o cursor no console    
    
  int pli = 1, pco = 14; // pli e pco são a linha e a coluna do personagem na matriz
  int m[10][17] = {{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 1, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 1, 3, 0, 1, 1, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 3, 1, 0, 0},
                 {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 3, 0, 3, 0, 1, 0, 0},
                 {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 3, 0, 1, 0, 1, 1, 1},
                 {1, 4, 4, 4, 4, 0, 0, 1, 1, 0, 3, 0, 0, 3, 0, 0, 1},
                 {1, 1, 4, 4, 4, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 1},
                 {1, 4, 4, 4, 4, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                 {1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  
  char x;
  
    int obj[10][17] = {};
    for(int l = 0; l < 10; l++){
    for(int c = 0; c < 17; c++){
        if(m[l][c] == 4){ 
            obj[l][c] = 1;
        }}}
        
        bool sair = false;
        
  while (true) {
    // imprime matriz
    for (int l = 0; l < 10; l++) {
      for (int c = 0; c < 17; c++) {
        switch (m[l][c]) {
        case 0:
        if(obj[l][c] == 1) cout << "*";
        else cout << " ";
        break;
        case 1:
          printf("\033[0;44m");
          cout << " ";
          printf("\033[0m");
          break;
        case 2:
          cout << "@";    
          break;
        case 3:
        cout<< "#";
        break;
        case 4:
        cout<< "*";
        break;
        default:
          cout << " ";
        }
      }
      cout << endl;
    }
    cout << "E - Voltar ao menu" << endl;
    // atualiza jogador
    x = getch(); // pega a tecla
    switch (x) {
        
    //Tecla W
    case 'w':
      pli = pli - 1; //O personagem tenta ir para a nova posição
      

    if(m[pli][pco] == 1 || (m[pli][pco] == 3 && (m[pli - 1][pco] == 1 || m[pli - 1][pco] == 3))){ //Confere o que tem a frente para verificar se pode andar
     pli = pli + 1;
     break;
}

    if (m[pli][pco] == 1) { // A nova posição é uma parede?
    pli = pli + 1;        // Se for parede, desfaz o passo (volta para onde estava)
    break;
}
 
    if(m[pli][pco] == 3){  //Quando encontra caixa no caminho (3)
    m[pli - 1][pco] = 3;  //Avança a caixa
    m[pli][pco] = 2;    //Torna a posição antiga da caixa agora sendo a do jogador
    m[pli + 1][pco] = 0; //Torna a posição antiga do jogador em espaço livre (0)
    break;
}

  //Se não era parede, atualiza a matriz
  m[pli + 1][pco] = 0; // Apaga a posição antiga
  m[pli][pco] = 2;     // Coloca o jogador na nova posição
  break;
  
  //Fim do código da tecla W
  
  
  //Tecla S
  
    case 's':
    // Tenta andar
     pli = pli + 1; 
     
//Confere se pode andar (obstáculos)
if(m[pli][pco] == 1 || (m[pli][pco] == 3 && (m[pli + 1][pco] == 1 || m[pli + 1][pco] == 3))){
     pli = pli - 1;
     break;
}
//Paredes
if(m[pli][pco] == 1){
    pli = pli - 1; 
    break;
}
//Caixa
if(m[pli][pco] == 3){
    m[pli + 1][pco] = 3;
    m[pli][pco] = 2;    
    m[pli - 1][pco] = 0;
    break;
}
  
//Se não era parede, atualiza a matriz
  m[pli - 1][pco] = 0;
  m[pli][pco] = 2;
  break;
  
  //Fim do código da tecla S
  
  
  //Tecla A
  
    case 'a':
    // Tenta andar
    pco = pco - 1;
   
//Confere se pode andar (obstáculos)
if(m[pli][pco] == 1 || (m[pli][pco] == 3 && (m[pli][pco - 1] == 1 || m[pli][pco - 1] == 3))){
     pco = pco + 1;
     break;
}
//Paredes
if (m[pli][pco] == 1) {
    pco = pco + 1; // Desfaz se for parede
    break;
}
//Caixa
if(m[pli][pco] == 3){
    m[pli][pco - 1] = 3;
    m[pli][pco] = 2;    
    m[pli][pco + 1] = 0;
    break;
}
//Se não era parede, atualiza a matriz
  m[pli][pco + 1] = 0;
  m[pli][pco] = 2;
  break;
  
 //Fim do código da tecla A
  
  
 //Tecla D
 
    case 'd':
    // Tenta andar
    pco = pco + 1; 
  
//Confere se pode andar (obstáculos)
if(m[pli][pco] == 1 || (m[pli][pco] == 3 && (m[pli][pco + 1] == 1 || m[pli][pco + 1] == 3))){
     pco = pco - 1;
     break;
}
//Paredes 
if (m[pli][pco] == 1) {
    pco = pco - 1; // Desfaz se for parede
    break;
}
//Caixa
if(m[pli][pco] == 3){
    m[pli][pco + 1] = 3;
    m[pli][pco] = 2;    
    m[pli][pco - 1] = 0;
    break;
}

//Se não era parede, atualiza a matriz
  m[pli][pco - 1] = 0;
  m[pli][pco] = 2;
  break;
  
  //Fim do código da tecla D
  
  case 'e':
    sair = true;
    break;
}

if(sair == true){
    (void)system("clear");
    break;
}
  
  
  
      // default:

    // coloca o cursor na posição inicial
    SetCursorPos(0, 0);
    //(void)system("clear");
    int soma = 0;
    if(m[6][1] == 3){
        soma = soma + 1;
    }
    if(m[6][2] == 3){
    soma = soma + 1;
    }
    if(m[6][3] == 3){
    soma = soma + 1;
    }
    if(m[6][4] == 3){
    soma = soma + 1;
    }
    if(m[7][2] == 3){
    soma = soma + 1;
    }
    if(m[7][3] == 3){
    soma = soma + 1;
    }
    if(m[7][4] == 3){
    soma = soma + 1;
    }
    if(m[8][1] == 3){
    soma = soma + 1;
    }
    if(m[8][2] == 3){
    soma = soma + 1;
    }
    if(m[8][3] == 3){
    soma = soma + 1;
    }
    if(m[8][4] == 3){
    soma = soma + 1;
    }
    if(soma == 11){
        cout<<"você ganhou!!"<<endl;
        WinMapa1 = true;
        this_thread::sleep_for(chrono::seconds(3));
        (void)system("clear");  
        break;
    }
  }
}
    
    
    if(escolhaMapa == 2){
     (void)system("clear");   
  printf("\033[?25l"); //código para o desativar o cursor no console    
    
  int pli = 8, pco = 11; // pli e pco são a linha e a coluna do personagem na matriz
  int m[11][19] = {{0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 1, 3, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 1, 1, 1, 0, 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 1, 0, 0, 3, 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                 {1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
                 {1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 4, 4, 1},
                 {1, 0, 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 1},
                 {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 2, 1, 1, 0, 0, 4, 4, 1},
                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                 {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0}};
  char x;
  
    int obj[11][19] = {};
    for(int l = 0; l < 11; l++){
    for(int c = 0; c < 19; c++){
        if(m[l][c] == 4){ 
            obj[l][c] = 1;
        }}}
        bool sair = false;
  while (true) {
    // imprime matriz
    for (int l = 0; l < 11; l++) {
      for (int c = 0; c < 19; c++) {
        switch (m[l][c]) {
        case 0:
        if(obj[l][c] == 1) cout << "*";
        else cout << " ";
        break;
        case 1:
          printf("\033[0;44m");
          cout << " ";
          printf("\033[0m");
          break;
        case 2:
          cout << "@";    
          break;
        case 3:
        cout<< "#";
        break;
        case 4:
        cout<< "*";
        break;
        default:
          cout << " ";
        }
      }
      cout << endl;
    }
    cout << "E - Voltar ao menu" << endl;
    // atualiza jogador
    x = getch(); // pega a tecla
    switch (x) {
        
    //Tecla W
    case 'w':
      pli = pli - 1; //O personagem tenta ir para a nova posição
      

    if(m[pli][pco] == 1 || (m[pli][pco] == 3 && (m[pli - 1][pco] == 1 || m[pli - 1][pco] == 3))){ //Confere o que tem a frente para verificar se pode andar
     pli = pli + 1;
     break;
}

    if (m[pli][pco] == 1) { // A nova posição é uma parede?
    pli = pli + 1;        // Se for parede, desfaz o passo (volta para onde estava)
    break;
}
 
    if(m[pli][pco] == 3){  //Quando encontra caixa no caminho (3)
    m[pli - 1][pco] = 3;  //Avança a caixa
    m[pli][pco] = 2;    //Torna a posição antiga da caixa agora sendo a do jogador
    m[pli + 1][pco] = 0; //Torna a posição antiga do jogador em espaço livre (0)
    break;
}

  //Se não era parede, atualiza a matriz
  m[pli + 1][pco] = 0; // Apaga a posição antiga
  m[pli][pco] = 2;     // Coloca o jogador na nova posição
  break;
  
  //Fim do código da tecla W
  
  
  //Tecla S
  
    case 's':
    // Tenta andar
     pli = pli + 1; 
     
//Confere se pode andar (obstáculos)
if(m[pli][pco] == 1 || (m[pli][pco] == 3 && (m[pli + 1][pco] == 1 || m[pli + 1][pco] == 3))){
     pli = pli - 1;
     break;
}
//Paredes
if(m[pli][pco] == 1){
    pli = pli - 1; 
    break;
}
//Caixa
if(m[pli][pco] == 3){
    m[pli + 1][pco] = 3;
    m[pli][pco] = 2;    
    m[pli - 1][pco] = 0;
    break;
}
  
//Se não era parede, atualiza a matriz
  m[pli - 1][pco] = 0;
  m[pli][pco] = 2;
  break;
  
  //Fim do código da tecla S
  
  
  //Tecla A
  
    case 'a':
    // Tenta andar
    pco = pco - 1;
   
//Confere se pode andar (obstáculos)
if(m[pli][pco] == 1 || (m[pli][pco] == 3 && (m[pli][pco - 1] == 1 || m[pli][pco - 1] == 3))){
     pco = pco + 1;
     break;
}
//Paredes
if (m[pli][pco] == 1) {
    pco = pco + 1; // Desfaz se for parede
    break;
}
//Caixa
if(m[pli][pco] == 3){
    m[pli][pco - 1] = 3;
    m[pli][pco] = 2;    
    m[pli][pco + 1] = 0;
    break;
}
//Se não era parede, atualiza a matriz
  m[pli][pco + 1] = 0;
  m[pli][pco] = 2;
  break;
  
 //Fim do código da tecla A
  
  
 //Tecla D
 
    case 'd':
    // Tenta andar
    pco = pco + 1; 
  
//Confere se pode andar (obstáculos)
if(m[pli][pco] == 1 || (m[pli][pco] == 3 && (m[pli][pco + 1] == 1 || m[pli][pco + 1] == 3))){
     pco = pco - 1;
     break;
}
//Paredes 
if (m[pli][pco] == 1) {
    pco = pco - 1; // Desfaz se for parede
    break;
}
//Caixa
if(m[pli][pco] == 3){
    m[pli][pco + 1] = 3;
    m[pli][pco] = 2;    
    m[pli][pco - 1] = 0;
    break;
}

//Se não era parede, atualiza a matriz
  m[pli][pco - 1] = 0;
  m[pli][pco] = 2;
  break;
  
  //Fim do código da tecla D
  
  case 'e':
    sair = true;
    break;
}

if(sair == true){
    (void)system("clear");
    break;
}
  
  
      // default:
    
    // coloca o cursor na posição inicial
    SetCursorPos(0, 0);
    //(void)system("clear");
    int soma = 0;
    if(m[6][17] == 3){
        soma = soma + 1;
    }
    if(m[7][17] == 3){
        soma = soma + 1;
    }
    if(m[8][17] == 3){
        soma = soma + 1;
    }
    if(m[6][16] == 3){
        soma = soma + 1;
    }
    if(m[7][16] == 3){
        soma = soma + 1;
    } 
    if(m[8][16] == 3){
        soma = soma + 1;
    }
    if(soma == 6){
        cout<<"você ganhou!!"<<endl;
        WinMapa2 = true;
        this_thread::sleep_for(chrono::seconds(3));
        (void)system("clear");  
        break;
    }
  }
}
if(escolhaMenu == 2){
    cout<<"Este jogo foi desenvolvido por Heitor Becker Piazera e Alessandro Barroso na disciplina de algortimos 1 lecionada por Thiago Felski"<<endl;
    cout<<"Digite qualquer número para voltar"<<endl;
    int voltar;
    cin>>voltar;
  (void)system("clear");
}
if(escolhaMenu == 3){
     (void)system("clear");  
    cout<<"O objetivo do jogo é alinhar as caixas aonde estão os *"<<endl;
    cout<<"Para isso deve se mover o personagem com as teclas:"<<endl;
    cout<<"W - cima"<<endl;
    cout<<"S - baixo"<<endl;
    cout<<"A - esquerda"<<endl;
    cout<<"D - direita"<<endl;
    cout<<"E assim empurrar as caixas # até os objetivos * com o personagem"<<endl;
    cout<<"Caso tenha se encontrado em uma situação em que é impossível resolver"<<endl;
    cout<<"Aperte o botão de desistir"<<endl;
    cout<<"Digite qualquer número para voltar"<<endl;
    int voltar;
    cin>>voltar;
  (void)system("clear");
}
if(escolhaMenu == 4){
  return 0;
}

}
}