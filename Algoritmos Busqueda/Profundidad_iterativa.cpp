/*
BPI combina la eficiencia del espacio de estados de la búsqueda en profundidad
y la completitud de la búsqueda en anchura. 
Es óptima cuando el costo del camino es una función no decreciente de la profundidad del nodo.

La complejidad en espacio de la BPI es O(bd), donde b es el factor de ramificación 
y d es la profundidad de la solución más superficial. 
Dado que BPI visita los estados múltiples veces, puede parecer extremadamente costoso,
pero no lo es, dado que la mayor parte de los nodos se encuentran en el nivel más profundo del árbol, 
por lo tanto no tiene mucha importancia que se visiten los niveles superiores varias veces.

La principal ventaja de BPI en búsquedas en árboles de juegos es que las búsquedas anteriores
tienen a mejorar la heurística usada, de forma que se puede obtener una estimación 
más precisa de la puntuación de varios nodos en la última búsqueda 
y la búsqueda se completa más rápidamente ya que se hace en un orden mejor.
​
Una segunda ventaja es la complejidad en tiempo del algoritmo.
Porque las primeras iteraciones usa valores pequeños para d, es decir, se ejecutan extremadamente rápido.
*/

#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct nodo
{
    int fila;
    int col;
    int profundidad = 0;
    nodo *padre = NULL;
};

vector<vector<char>> cargaMapa(std::string nombre);
vector<nodo> obtenVecinos(nodo &actual);
bool esObstaculo(nodo Nodo,vector<vector<char>> mapa);
bool busquedaProfundidadLimitada(nodo &origen, nodo &destino, int profundidad);

bool buscaEnLista(list<nodo> &lista, nodo &estado)
{
    for (auto it = lista.begin(); it != lista.end(); ++it)
    {
        if ((it->col == estado.col) && (it->fila == estado.fila))
        {
            return true;
        }
    }

    return false;
}

bool pathfinding(nodo &origen, nodo &destino, vector<vector<char>> mapa)
{
    int profundidad = 0;
    bool encontrado = false;

    if(esObstaculo(destino,mapa))
        return false;

    while (!encontrado)
    {
        encontrado = busquedaProfundidadLimitada(origen, destino, profundidad);
        profundidad++;
    }

    return true;
}

bool busquedaProfundidadLimitada(nodo &origen, nodo &destino, int profundidad)
{

    list<nodo> abiertos;
    list<nodo> cerrados;
    nodo actual;

    abiertos.push_back(origen);

    while (!abiertos.empty())
    {
        actual = abiertos.front();
        abiertos.pop_front();
        cerrados.push_back(actual);

        if ((actual.fila == destino.fila) && (actual.col == destino.col))
            return true;
        else
        {
            if (actual.profundidad < profundidad)
            {
                vector<nodo> sucesores = obtenVecinos(actual);
                vector<nodo> SucesoresNoAbiertosCerrados;

                for (int i = 0; i < sucesores.size(); i++)
                {
                    if (!buscaEnLista(abiertos, sucesores[i]) && !buscaEnLista(cerrados, sucesores[i]))
                        SucesoresNoAbiertosCerrados.push_back(sucesores[i]);
                }

                for (int i = 0; i < sucesores.size(); i++)
                {
                    abiertos.push_front(sucesores[i]);
                }
            }
        }
    }
}

bool esObstaculo(nodo nodo,vector<vector<char>> mapa)
{
    if ((mapa[nodo.fila][nodo.col] == 'B') || (mapa[nodo.fila][nodo.col] == 'A') ||
        (mapa[nodo.fila][nodo.col] == 'P') || (mapa[nodo.fila][nodo.col] == 'M') ||
        (mapa[nodo.fila][nodo.col] == 'D'))
        return true;
    return false;
}

vector<nodo> obtenVecinos(nodo &actual)
{
    vector<nodo> salida;

    nodo norte;
    norte.fila = actual.fila - 1;
    norte.col = actual.col;
    norte.profundidad = actual.profundidad + 1;
    norte.padre = &actual;
    salida.push_back(norte);

    nodo sur;
    sur.fila = actual.fila + 1;
    sur.col = actual.col;
    sur.padre = &actual;
    sur.profundidad = actual.profundidad + 1;
    salida.push_back(sur);

    nodo este;
    este.fila = actual.fila;
    este.col = actual.col + 1;
    este.padre = &actual;
    este.profundidad = actual.profundidad + 1;
    salida.push_back(este);

    nodo oeste;
    oeste.fila = actual.fila;
    oeste.col = actual.col - 1;
    oeste.profundidad = actual.profundidad + 1;
    oeste.padre = &actual;
    salida.push_back(oeste);

    return salida;
}

vector<vector<char>> cargaMapa(std::string nombre)
{

    vector<vector<char>> mapa;
    ifstream file;
    std::string linea;
    string n, m;
    int filas, columnas;

    file.open(nombre);

    std::getline(file, n);
    std::getline(file, m);

    filas = std::stoi(n);
    columnas = std::stoi(m);

    mapa.resize(filas);
    for (int i = 0; i < filas; i++)
        mapa[i].resize(columnas);

    for (int fila = 0; fila < filas; fila++)
    {
        std::getline(file, linea);

        for (int i = 0; i < linea.length(); i++)
        {
            mapa[fila][i] = linea[i];
        }
    }

    return mapa;
}

int main()
{
    vector<vector<char>> mapa;

    mapa = cargaMapa("mapas/cuadricula.map");

    nodo origen;
    origen.fila = 72;
    origen.col = 33;

    nodo destino;
    destino.fila = 53;
    destino.col = 71;

    bool hayPlan = pathfinding(origen, destino, mapa);
    cout << hayPlan;
}