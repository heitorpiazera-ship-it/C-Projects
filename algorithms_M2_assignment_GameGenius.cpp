#include <iostream>
#include <chrono>//Para o tempo
#include <thread>//Para o tempo
#include <random>//Para o rand()
#include <cstdlib>//para o exit(0)
#include <ctime>//para utilizar o srand(time(0))
using namespace std;


int main() {
	//Garante que os rand() vão ser diferentes a cada jogo
	srand(time(0));
	int escolha_menu;
	//While para o loop enquanto opção ser 1,2,3
	while(true) {
		//Menu inicial
		system("clear");
		cout << "Digite" << endl;
		cout << "1 - Jogar" << endl;
		cout << "2 - Dificuldade" << endl;
		cout << "3 - Sobre" << endl;
		cout << "4 - Fim" << endl;
		//Fim do menu inicial
		cin >> escolha_menu;
		//Escolha 1 do menu (Jogar)
		if(escolha_menu == 1) {
		    system("clear");
			//tamanho do vetor
			int sequencia[30];
			int tamanho = 0;
			//booleana para retornar ao menu caso o usuario perca o jogo
			bool perdeu = false;
			for(int rodada = 0; rodada < 10; rodada++) {
				//variação de digitos por rodada (1 a 4)
				int num1 = rand() % 4 + 1;
				int num2 = rand() % 4 + 1;
				//varição de quantos digitos aparecem
				sequencia[tamanho] = num1;
				tamanho++;
				sequencia[tamanho] = num2;
				tamanho++;
				cout << "Nova rodada começando" << endl;
				//comando sleep por 2 segundos
				this_thread::sleep_for(chrono::seconds(2));
				cout << "Memorize" << endl;

				for(int i = 0; i < tamanho; i++) {
					cout << sequencia[i] << " ";
				}
				cout << endl;
				//ajuste do tempo da dificuldade (Padrão média 3 segundos)
				this_thread::sleep_for(chrono::seconds(3));
				system("clear");
				cout << "Digite sua resposta:" <<endl;

				for(int i = 0; i < tamanho; i++) {
					int resposta;
					cin >> resposta;
					if(resposta != sequencia[i]) {
						cout << "Você perdeu!"<<endl;
						perdeu = true;
						//break do segundo for (respostas)
						break;

					}
				}
				//Para voltar ao Menu caso perca
				if(perdeu) {
					//ignore para limpar o buffer
					cin.ignore(100, '\n');
					this_thread::sleep_for(chrono::seconds(2));
					//break do primeiro for (rodada)
					break;

				}
			}
		}
		//Escolha 2 do menu (dificuldade)
		if(escolha_menu == 2) {
		    system("clear");
			int escolha_dificuldade;
			cout << "1 - fácil" << endl;
			cout << "2 - media" << endl;
			cout << "3 - difícil" << endl;
			cin >> escolha_dificuldade;
			
			//Dificuldade fácil
			if(escolha_dificuldade == 1) {
			    system("clear");
				int sequencia[30];
				int tamanho = 0;
				//booleana para retornar ao menu caso o usuario perca o jogo
				bool perdeu = false;
				for(int rodada = 0; rodada < 10; rodada++) {
					//variação de digitos por rodada (1 a 3)
					int num1 = rand() % 3 + 1;
					int num2 = rand() % 3 + 1;
					//varição de quantos digitos aparecem
					sequencia[tamanho] = num1;
					tamanho++;
					sequencia[tamanho] = num2;
					tamanho++;
					cout << "Nova rodada começando" << endl;
					//comando sleep por 2 segundos
					this_thread::sleep_for(chrono::seconds(2));
					cout << "Memorize!" << endl;

					for(int i = 0; i < tamanho; i++) {
						cout << sequencia[i] << " ";
					}

					cout << endl;
					//ajuste do tempo da dificuldade (fácil 5 segundos)
					this_thread::sleep_for(chrono::seconds(5));
					system("clear");
					cout << "Digite sua resposta:"<<endl;

					for(int i = 0; i < tamanho; i++) {
						int resposta;
						cin >> resposta;
						if(resposta != sequencia[i]) {
							cout << "Você perdeu!"<<endl;
							perdeu = true;
							//break do segundo for (respostas)
							break;
						}
					}
					//Para voltar ao Menu caso perca
					if(perdeu) {
						//ignore para limpar o buffer
						cin.ignore(100, '\n');
						this_thread::sleep_for(chrono::seconds(2));
						//break do primeiro for (rodada)
						break;
					}
				}
			}
			//Dificuldade média
			if(escolha_dificuldade == 2) {
			    system("clear");
				int sequencia[30];
				int tamanho = 0;
				//booleana para retornar ao menu caso o usuario perca o jogo
				bool perdeu = false;
				for(int rodada = 0; rodada < 10; rodada++) {
					//variação de digitos por rodada (1 a 4)
					int num1 = rand() % 4 + 1;
					int num2 = rand() % 4 + 1;
					//varição de quantos digitos aparecem
					sequencia[tamanho] = num1;
					tamanho++;
					sequencia[tamanho] = num2;
					tamanho++;
					cout << "Nova rodada começando" << endl;
					//comando sleep por 2 segundos
					this_thread::sleep_for(chrono::seconds(2));
					cout << "Memorize!" << endl;

					for(int i = 0; i < tamanho; i++) {
						cout << sequencia[i] << " ";
					}

					cout << endl;
					//ajuste do tempo da dificuldade (Média 3 segundos)
					this_thread::sleep_for(chrono::seconds(3));
					system("clear");
					cout << "Digite sua resposta:"<<endl;

					for(int i = 0; i < tamanho; i++) {
						int resposta;
						cin >> resposta;
						if(resposta != sequencia[i]) {
							cout << "Você perdeu!"<<endl;
							perdeu = true;
							//break do segundo for (respostas)
							break;
						}
					}
					//Para voltar ao Menu caso perca
					if(perdeu) {
						//ignore para limpar o buffer
						cin.ignore(100, '\n');
						this_thread::sleep_for(chrono::seconds(2));
						//break do primeiro for (rodada)
						break;
					}
				}
			}
			//Dificuldade dificil
			if(escolha_dificuldade == 3) {
			    system("clear");
				int sequencia[30];
				int tamanho = 0;
				//booleana para retornar ao menu caso o usuario perca o jogo
				bool perdeu = false;
				for(int rodada = 0; rodada < 10; rodada++) {
					//variação de digitos por rodada (1 a 4)
					int num1 = rand() % 4 + 1;
					int num2 = rand() % 4 + 1;
					int num3 = rand() % 4 + 1;
					//varição de quantos digitos aparecem
					sequencia[tamanho] = num1;
					tamanho++;
					sequencia[tamanho] = num2;
					tamanho++;
					sequencia[tamanho] = num3;
					tamanho++;
					cout << "Nova rodada começando" << endl;
					//comando sleep por 2 segundos
					this_thread::sleep_for(chrono::seconds(2));
					cout << "Memorize!" << endl;

					for(int i = 0; i < tamanho; i++) {
						cout << sequencia[i] << " ";
					}

					cout << endl;
					//ajuste do tempo da dificuldade (Dificil 1 segundos)
					this_thread::sleep_for(chrono::seconds(1));
					system("clear");
					cout << "Digite sua resposta:"<<endl;

					for(int i = 0; i < tamanho; i++) {
						int resposta;
						cin >> resposta;
						if(resposta != sequencia[i]) {
							cout << "Você perdeu!"<<endl;
							perdeu = true;
							//break do segundo for (respostas)
							break;
						}
					}
					//Para voltar ao Menu caso perca
					if(perdeu) {
						//ignore para limpar o buffer
						cin.ignore(100, '\n');
						this_thread::sleep_for(chrono::seconds(2));
						//break do primeiro for (rodada)
						break;
					}
				}
			}
		}
		//Escolha 3 do menu (Sobre)
		if(escolha_menu == 3) {

			cout << "Desenvolvido por Heitor Becker Piazera e Alessandro Barroso no dia 13/05/2026 na disciplina de algoritmos lecionada por Thiago Felsky";
			system("clear");
			cout << endl;
			this_thread::sleep_for(chrono::seconds(3));
			system("clear");
			cout<< "Como jogar:" << endl;
			cout<< "1° - Memorize a sequencia mostrada na tela" << endl;
			cout<< "2° - Escreva a sequencia dando espaço entre cada numero" << endl;
			cout<< "3° - Divirta-se!" << endl;
			cout<< "aperte ENTER para voltar ao menu inicial";
			cin.ignore();
			cin.get();
			system("clear");
			
		}
		//Escolha 4 do menu (Fim)
		if(escolha_menu == 4) {
			break;
			exit(0);
		}
	}

	return 0;
}