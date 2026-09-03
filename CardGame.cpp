
#include <iostream>
#include <termios.h>
#include <unistd.h>
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
  printf("\033[?25l"); //código para o desativar o cursor no console    
    
  int pli = 2, pco = 2; // pli e pco são a linha e a coluna do personagem na matriz
  int m[20][16] = {{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1},
                 {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1},
                 {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1},
                 {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
                 {1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1},
                 {0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
                 {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                 {1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                 {1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1},
                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                 {0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
                 {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                 {1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1},
                 {1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                 {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1},
                 {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                 {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                 {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1},
                 {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}};
  char x;
  while (true) {
    cout << "texto antes da matriz\n";
    // imprime matriz
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        switch (m[i][j]) {
        case 0:
          cout << " ";
          break;
        case 1:
          printf("\033[0;44m");// fundo do texto azul
          cout << " ";
          printf("\033[0m");//todo texto normal
          break;
        case 2:
          cout << "@";    
          break;
        default:
          cout << " ";
        }
      }
      cout << endl;
    }
    cout << "texto depois da matriz\n";
    // atualiza jogador
    x = getch(); // pega a tecla
    switch (x) {
    case 'w':
      // remove da posição antiga
      m[pli][pco] = 0;
      // adiciona na nova posição
      pli = pli - 1;
      m[pli][pco] = 2;
      break;
    case 's':
      // remove da posição antiga
      m[pli][pco] = 0;
      // adiciona na nova posição
      pli = pli + 1;
      m[pli][pco] = 2;
      break;
    case 'a':
      // remove da posição antiga
      m[pli][pco] = 0;
      // adiciona na nova posição
      pco = pco - 1;
      m[pli][pco] = 2;
      break;
    case 'd':
      // remove da posição antiga
      m[pli][pco] = 0;
      // adiciona na nova posição
      pco = pco + 1;
      m[pli][pco] = 2;
      break;
      // default:
    }
    // coloca o cursor na posição inicial
    SetCursorPos(0, 0);
    //(void)system("clear");
  }
  return 0;
}
