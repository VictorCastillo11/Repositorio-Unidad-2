#include <iostream>
#include <fstream>
#include <vector>
#include <conio.h>
#include <windows.h>

using namespace std;

const int movimientos[4][2] = {{1,0}, {0,1}, {-1,0}, {0,-1}};

struct Coordenada {
    int x, y;
    Coordenada(int px, int py) : x(px), y(py) {} 
};

vector<vector<char> > terreno;
vector<Coordenada> recorrido;
int jugadorX = -1, jugadorY = -1; 

bool cargarNivel(const string& archivo) {
    ifstream nivel(archivo.c_str());
    if (!nivel.is_open()) {
        cout << "Error: No se pudo abrir " << archivo << endl;
        cout << "Asegurate que el archivo existe en la misma carpeta que el programa." << endl;
        return false;
    }

    string fila;
    while (getline(nivel, fila)) {
        vector<char> linea;
        for (size_t j = 0; j < fila.size(); j++) {
            linea.push_back(fila[j]);
            if (fila[j] == 'i') {
                jugadorX = j;
                jugadorY = terreno.size();
            }
        }
        terreno.push_back(linea);
    }
    
    if (jugadorX == -1 || jugadorY == -1) {
        cout << "Error: No se encontro el punto inicial 'i' en el laberinto." << endl;
        return false;
    }
    
    nivel.close();
    return true;
}

void mostrarTerreno() {
    system("cls");
    for (size_t i = 0; i < terreno.size(); i++) {
        for (size_t j = 0; j < terreno[i].size(); j++) {
            cout << terreno[i][j];
        }
        cout << endl;
    }
}

bool esPared(char c) {
    return c == '#' || c == '*';
}

bool explorar() {
    if (terreno[jugadorY][jugadorX] == 'S') {
        return true;
    }

    for (int i = 0; i < 4; i++) {
        int nuevoX = jugadorX + movimientos[i][0];
        int nuevoY = jugadorY + movimientos[i][1];

        if (nuevoY >= 0 && nuevoY < (int)terreno.size() && 
            nuevoX >= 0 && nuevoX < (int)terreno[nuevoY].size() &&
            !esPared(terreno[nuevoY][nuevoX]) &&
            terreno[nuevoY][nuevoX] != '.') { // Evitar revisitar celdas

            char celdaActual = terreno[jugadorY][jugadorX];
            terreno[jugadorY][jugadorX] = '.';
            
            Coordenada anterior(jugadorX, jugadorY); 
            recorrido.push_back(anterior);

            jugadorX = nuevoX;
            jugadorY = nuevoY;

            if (explorar()) {
                return true;
            }

            // Backtracking
            terreno[jugadorY][jugadorX] = ' ';
            jugadorX = recorrido.back().x;
            jugadorY = recorrido.back().y;
            recorrido.pop_back();
        }
    }
    return false;
}

int main() {
    cout << "Cargando laberinto..." << endl;
    if (!cargarNivel("texto.txt")) {
        cout << "Presiona cualquier tecla para salir..." << endl;
        _getch();
        return 1;
    }

    cout << "Laberinto cargado. Resolviendo..." << endl;
    mostrarTerreno();
    Sleep(1000); // Pausa para ver el laberinto inicial

    if (explorar()) {
        mostrarTerreno();
        cout << "\nLaberinto resuelto con exito" << endl;
    } else {
        mostrarTerreno();
        cout << "\nNo se encontro solucion al laberinto." << endl;
    }

    cout << "Presiona cualquier tecla para salir..." << endl;
    _getch();
    return 0;
}
