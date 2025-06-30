#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Mostrar tablero visible
void mostrarTablero(char visible[4][4]) {
    cout << "   1 2 3 4\n";
    for (int i = 0; i < 4; i++) {
        cout << char('A' + i) << "  ";
        for (int j = 0; j < 4; j++) {
            cout << visible[i][j] << " ";
        }
        cout << endl;
    }
}

// Verifica si el juego terminó
bool juegoTerminado(char visible[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (visible[i][j] == 'X')
                return false;
    return true;
}

// Convierte entrada tipo "A1" a coordenadas fila/columna
bool obtenerCoordenada(string entrada, int& fila, int& col) {
    if (entrada.length() != 2) return false;
    char letra = toupper(entrada[0]);
    char numero = entrada[1];
    if (letra < 'A' || letra > 'D' || numero < '1' || numero > '4') return false;
    fila = letra - 'A';
    col = numero - '1';
    return true;
}

// Mezclar vector
void mezclar(vector<char>& valores) {
    for (int i = valores.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(valores[i], valores[j]);
    }
}

int main() {
    srand(time(0)); // Mezcla diferente cada vez
    int opcion;

    do {
        // Menú
        cout << "===============================\n";
        cout << "   Bienvenido al JUEGO DE MEMORIA :)\n";
        cout << "===============================\n";
        cout << "1. Nuevo juego\n";
        cout << "2. Salir\n";
        cout << "===============================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            system("CLS");

            // Crear pares de números como caracteres
            vector<char> valores;
            for (int i = 1; i <= 8; i++) {
                valores.push_back(i + '0');
                valores.push_back(i + '0');
            }

            mezclar(valores);

            // Tableros
            char matrizReal[4][4];
            char matrizVisible[4][4];

            int k = 0;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++) {
                    matrizReal[i][j] = valores[k++];
                    matrizVisible[i][j] = 'X';
                }

            int intentos = 0;

            while (!juegoTerminado(matrizVisible)) {
                mostrarTablero(matrizVisible);

                string entrada1, entrada2;
                int fila1, col1, fila2, col2;

                
                do {
                    cout << "Elige la primera carta (ej: A1): ";
                    cin >> entrada1;
                } while (!obtenerCoordenada(entrada1, fila1, col1) || matrizVisible[fila1][col1] != 'X');

                
                do {
                    cout << "Elige la segunda carta (ej: B3): ";
                    cin >> entrada2;
                } while (!obtenerCoordenada(entrada2, fila2, col2) || (fila1 == fila2 && col1 == col2) || matrizVisible[fila2][col2] != 'X');

                
                matrizVisible[fila1][col1] = matrizReal[fila1][col1];
                matrizVisible[fila2][col2] = matrizReal[fila2][col2];

                system("CLS"); 
                mostrarTablero(matrizVisible);
                intentos++;

                
                if (matrizReal[fila1][col1] == matrizReal[fila2][col2]) {
                    cout << "? ¡Es un par YEAAAA!\n";
                } else {
                    cout << "No es un par... :(\n";
                    cin.ignore(); 
                    cin.get();    
                    matrizVisible[fila1][col1] = 'X';
                    matrizVisible[fila2][col2] = 'X';
                }

                system("CLS"); 
            }
			if (intentos > 20) {
    			cout << "Felicidades, encontraste todos los pares en " << intentos << " intentos, puedes mejorar.\n";
    		}
			else {
    			cout << "Felicidades, encontraste todos los pares en " << intentos << " intentos, eres una BESTIA.\n";
			}
			cin.ignore();
			cin.get(); 
			system("CLS");
            

        } else if (opcion == 2) {
            cout << "Gracias por jugar. Vuelva pronto :).\n";
        } else {
            cout << "? Opción no válida. Intente de nuevo.\n";
        }

    } while (opcion != 2);

    return 0;
}
