#include "raylib.h"

int main() {
    // 1. INICIALIZAÇÃO (Roda apenas uma vez)
    InitWindow(800, 600, "Meu Primeiro Jogo Raylib");
    SetTargetFPS(60); // Crava o jogo em 60 frames por segundo

    // Variáveis do seu "jogador"
    float quadradoX = 400.0f;
    float quadradoY = 300.0f;

    float zumbiX = 500.0f;
    float zumbiY = 400.0f;


    // 2. O GAME LOOP (Roda 60 vezes por segundo)
    while (!WindowShouldClose()) { 
        
        // --- FASE LÓGICA (Update) ---
        // Não usa getch(), ele apenas checa se a tecla está pressionada neste exato frame
        if (IsKeyDown(KEY_RIGHT)) quadradoX += 20.0f;
        if (IsKeyDown(KEY_LEFT)) quadradoX -= 20.0f;
        zumbiX -= 5.0f;

        // --- FASE GRÁFICA (Draw) ---
        BeginDrawing();
            
            // Limpa a tela inteira (substitui aquele PosicionarCursor do terminal)
            ClearBackground(RAYWHITE); 

            // Desenha as formas na nova posição
            DrawRectangle(quadradoX, quadradoY, 50, 50, RED);
            DrawRectangle(zumbiX, zumbiY, 50, 50, GREEN);
           

        EndDrawing();
    }

    // 3. ENCERRAMENTO (Limpa a memória)
    CloseWindow();
    return 0;
}