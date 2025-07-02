#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <algorithm>

using namespace std;

const int FONDO_MORADO = 13;
const int TEXTO_VERDE = 10;
const int TEXTO_AMARILLO = 14;
const int TEXTO_BLANCO = 15;

struct Puntaje {
    string nombre;
    int intentos;
};

bool soloLetras(const string& texto) {
    for (char c : texto) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

string leerTexto(const string& mensaje) {
    string entrada;
    do {
        cout << mensaje;
        getline(cin >> ws, entrada);
        if (!soloLetras(entrada)) {
            cout << "Error solo se permiten letras\n";
        }
    } while (!soloLetras(entrada));
    return entrada;
}

void guardarPuntaje(const string& nombre, int intentos, const string& dificultad) {
    ofstream archivo(dificultad + ".txt", ios::app);
    if (archivo.is_open()) {
        archivo << nombre << " " << intentos << endl;
        archivo.close();
    }
}

void mostrarTabla(const string& dificultad);

void tablaPocisiones() {
    system("CLS");
    system("color 60");
    Beep(800, 120);
    cout << "======================================\n";
    cout << "|| Ver tabla de posiciones          ||\n";
    cout << "======================================\n";
    cout << "|| 1. Facil                         ||\n";
    cout << "|| 2. Normal                        ||\n";
    cout << "|| 3. Dificil                       ||\n";
    cout << "======================================\n";
    int dif;
    cout << "|| Selecciona una opcion: ";
    cin >> dif;
    if (dif == 1) mostrarTabla("facil");
    else if (dif == 2) mostrarTabla("normal");
    else mostrarTabla("dificil");
}

void mostrarTabla(const string& dificultad) {
    ifstream archivo(dificultad + ".txt");
    vector<Puntaje> puntajes;
    string nombre;
    int intentos;
    while (archivo >> nombre >> intentos) {
        puntajes.push_back({nombre, intentos});
    }

    int subopcion;
    do {
        system("CLS");
        cout << "======================================\n";
        cout << "|| TABLA DE POSICIONES - " << dificultad << "      ||\n";
        cout << "======================================\n";

        if (puntajes.empty()) {
            cout << "|| No hay jugadores por el momento  ||\n";
            cout << "======================================\n";
        } else {
            sort(puntajes.begin(), puntajes.end(), [](Puntaje a, Puntaje b) {
                return a.intentos < b.intentos;
            });
            for (int i = 0; i < min(5, (int)puntajes.size()); ++i) {
                cout << i + 1 << ". " << puntajes[i].nombre << " - " << puntajes[i].intentos << " intentos\n";
            }
        }
		cout << "======================================\n";
        cout << "|| 1. Otra Dificultad\n";
        cout << "|| 2. Borrar un jugador \n";
        cout << "|| 3. Menu Principal \n";
        cout << "|| Opcion: ";
        cin >> subopcion;

        if (subopcion == 1) {
            tablaPocisiones();
            return;
        } else if (subopcion == 2) {
            cin.ignore();
            string nombreEliminar;
            cout << "Nombre exacto del jugador a eliminar: ";
            getline(cin >> ws, nombreEliminar);

            auto nuevoFin = remove_if(puntajes.begin(), puntajes.end(), [&](const Puntaje& p) {
                return p.nombre == nombreEliminar;
            });

            if (nuevoFin == puntajes.end()) {
                cout << "Jugador no encontrado.\n";
            } else {
                puntajes.erase(nuevoFin, puntajes.end());
                ofstream nuevoArchivo(dificultad + ".txt");
                for (const Puntaje& p : puntajes) {
                    nuevoArchivo << p.nombre << " " << p.intentos << endl;
                }
                cout << "Jugador eliminado exitosamente.\n";
            }

            cout << "Presiona ENTER para continuar...\n";
            cin.get();
        }

    } while (subopcion != 3);
    system("CLS");
}

void mostrarTablero(const vector<vector<char>>& visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "   ";
    for (int j = 0; j < visible[0].size(); j++) {
        cout << j + 1 << " ";
    }
    cout << endl;

    for (int i = 0; i < visible.size(); i++) {
        cout << char('A' + i) << "  ";
        for (int j = 0; j < visible[i].size(); j++) {
            if (visible[i][j] != '#') {
                SetConsoleTextAttribute(hConsole, FONDO_MORADO * 16 + TEXTO_VERDE);
            } else {
                SetConsoleTextAttribute(hConsole, FONDO_MORADO * 16 + TEXTO_AMARILLO);
            }
            cout << visible[i][j] << " ";
        }
        cout << endl;
    }
    SetConsoleTextAttribute(hConsole, FONDO_MORADO * 16 + TEXTO_BLANCO);
}

bool juegoTerminado(const vector<vector<char>>& visible) {
    for (const auto& fila : visible)
        for (char c : fila)
            if (c == '#') return false;
    return true;
}

bool obtenerCoordenada(string entrada, int& fila, int& col, int dimension) {
    if (entrada.length() < 2 || entrada.length() > 3) return false;
    char letra = toupper(entrada[0]);
    string numero = entrada.substr(1);
    int num = stoi(numero);
    if (letra < 'A' || letra >= 'A' + dimension || num < 1 || num > dimension) return false;
    fila = letra - 'A';
    col = num - 1;
    return true;
}

void mezclar(vector<char>& valores) {
    for (int i = valores.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(valores[i], valores[j]);
    }
}

int main() {
    srand(time(0));
    int opcion;
    do {
        system("color 70");
        system("CLS");
        cout << "=======================================\n";
        cout << "|| Bienvenido al JUEGO DE MEMORIA :) ||\n";
        cout << "=======================================\n";
        cout << "|| 1. Nuevo juego                    ||\n";
        cout << "|| 2. Tabla de posiciones            ||\n";
        cout << "|| 3. Salir                          ||\n";
        cout << "=======================================\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            string nombre = leerTexto("Ingrese su nombre: ");

            system("CLS");
            int dificultad;
            cout << "=======================================\n";
            cout << "|| Seleccione la dificultad:         ||\n";
            cout << "=======================================\n";
            cout << "|| 1. Facil (4x4)                    ||\n";
            cout << "|| 2. Normal (6x6)                   ||\n";
            cout << "|| 3. Dificil (8x8)                  ||\n";
            cout << "=======================================\n";          
            cout << "|| Opcion: ";
            cin >> dificultad;

            int dimension;
            string nomDif;
            if (dificultad == 1) {
                dimension = 4; nomDif = "facil";
            }
            else if (dificultad == 2) {
                dimension = 6; nomDif = "normal";
            }
            else {
                dimension = 8; nomDif = "dificil";
            }

            system("CLS");
            system("color de");
            Beep(800, 120);
            Beep(1000, 100);

            vector<char> simbolos;
            for (char c = 'A'; c <= 'Z'; c++) simbolos.push_back(c);
            for (char c = 'a'; c <= 'z'; c++) simbolos.push_back(c);

            int pares = (dimension * dimension) / 2;
            if (pares > simbolos.size()) {
                cout << "No hay suficientes letras para este tama\u00f1o de tablero.\n";
                return 1;
            }

            vector<char> valores;
            for (int i = 0; i < pares; i++) {
                valores.push_back(simbolos[i]);
                valores.push_back(simbolos[i]);
            }

            mezclar(valores);

            vector<vector<char>> matrizReal;
            vector<vector<char>> matrizVisible;
            for (int i = 0; i < dimension; ++i) {
                vector<char> filaReal;
                vector<char> filaVisible;
                for (int j = 0; j < dimension; ++j) {
                    filaReal.push_back(' ');
                    filaVisible.push_back('#');
                }
                matrizReal.push_back(filaReal);
                matrizVisible.push_back(filaVisible);
            }

            int k = 0;
            for (int i = 0; i < dimension; i++)
                for (int j = 0; j < dimension; j++)
                    matrizReal[i][j] = valores[k++];

            int intentos = 0;

            while (!juegoTerminado(matrizVisible)) {
                mostrarTablero(matrizVisible);

                string entrada1, entrada2;
                int fila1, col1, fila2, col2;

                do {
                    cout << "Elige la primera carta (ej: A1): ";
                    cin >> entrada1;
                } while (!obtenerCoordenada(entrada1, fila1, col1, dimension) || matrizVisible[fila1][col1] != '#');

                do {
                    cout << "Elige la segunda carta (ej: B3): ";
                    cin >> entrada2;
                } while (!obtenerCoordenada(entrada2, fila2, col2, dimension) || (fila1 == fila2 && col1 == col2) || matrizVisible[fila2][col2] != '#');

                matrizVisible[fila1][col1] = matrizReal[fila1][col1];
                matrizVisible[fila2][col2] = matrizReal[fila2][col2];

                system("CLS");
                mostrarTablero(matrizVisible);
                intentos++;

                if (matrizReal[fila1][col1] == matrizReal[fila2][col2]) {
                    Beep(1000, 150);
                    cout << "¡Es un par YEAAAA!\n";
                    cin.ignore();
                    cin.get();
                } else {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, FONDO_MORADO * 16 + TEXTO_BLANCO);
                    Beep(400, 300);
                    cout << "No es un par... :(\n";
                    cin.ignore();
                    cin.get();
                    matrizVisible[fila1][col1] = '#';
                    matrizVisible[fila2][col2] = '#';
                }

                system("CLS");
            }

            guardarPuntaje(nombre, intentos, nomDif);

            if (intentos > dimension * 2) {
                system("color e0");
                cout << "Felicidades, encontraste todos los pares, puedes hacerlo mejor.\n";
                cout << "No. De intentos: " << intentos;
            } else {
                system("color b0");
                cout << "Felicidades, encontraste todos los pares, eres una BESTIA.\n";
                cout << "No. De intentos: " << intentos;
            }

            Beep(1200, 200);
            Beep(1500, 250);
            Beep(1800, 300);
            cin.ignore();
            cin.get();
            system("color de");
            system("CLS");
        }
        else if (opcion == 2) {
            tablaPocisiones();
        }
        else if (opcion == 3) {
            cout << "Gracias por jugar. Vuelva pronto :).\n";
        } else {
            cout << "Opcion no valida. Intente de nuevo.\n";
            cin.ignore();
            cin.get();
        }
    } while (opcion != 3);

    return 0;
}

