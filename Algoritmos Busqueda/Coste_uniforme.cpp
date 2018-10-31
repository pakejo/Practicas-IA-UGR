/*
La idea fundamental de este nuevo algoritmo consiste en explorar
siempre el nodo de la frontera que tenga menor coste, entendido como suma
de todos los costes desde el nodo inicial a ese nodo.

Denominamos al algoritmo como de búsqueda con coste uniforme 
porque la forma de explorar propuesta expande el arbol de forma uniforme
respecto al coste de sus nodos. Todos los nodos de la frontera van a tener
tendencia a tener un coste similar.

El algoritmo para búsqueda con coste uniforme hereda la sencillez 
del algoritmo para búsqueda en anchura y es capaz de localizar la solución óptima
siempre que no haya una secuencia infinita de arcos con coste cero en el árbol del búsqueda.

El algoritmo de búsqueda con coste uniforme es completo en situaciones razonables.
Tiene los mismos problemas de complejidad que el algoritmo para búsqueda en anchura
aunque puede funcionar en algunos problemas en los que aquel se vuelve inmanejable.
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
    int coste = 0;
    nodo *padre = NULL;

    bool operator<(const nodo &b) { return coste < b.coste; }
    bool operator==(const nodo &b) { return (fila == b.fila) && (col == b.col); }
};

vector<vector<char>> cargaMapa(std::string nombre);
vector<nodo> obtenVecinos(nodo &actual);
bool buscaEnLista(list<nodo> &lista, nodo &nodo);

bool esObstaculo(nodo nodo, vector<vector<char>> mapa)
{
    if ((mapa[nodo.fila][nodo.col] == 'B') || (mapa[nodo.fila][nodo.col] == 'A') ||
        (mapa[nodo.fila][nodo.col] == 'P') || (mapa[nodo.fila][nodo.col] == 'M') ||
        (mapa[nodo.fila][nodo.col] == 'D'))
        return true;
    return false;
}

bool pathfinding(nodo &origen, nodo &destino, vector<vector<char>> mapa)
{
    list<nodo> frontera;
    list<nodo> visitados;

    frontera.push_back(origen);

    if (esObstaculo(destino, mapa))
        return false;

    while (!frontera.empty())
    {
        nodo actual = frontera.front();
        frontera.pop_front();
        visitados.push_back(actual);

        if (actual == destino)
            return true;

        vector<nodo> vecinos = obtenVecinos(actual);
        vector<nodo> vecinosNoVisitados;

        for (int i = 0; i < vecinos.size(); i++)
        {
            if (!buscaEnLista(visitados, vecinos[i]))
                vecinosNoVisitados.push_back(vecinos[i]);
        }

        for (int i = 0; i < vecinosNoVisitados.size(); i++)
        {
            int costePadreHijo = vecinosNoVisitados[i].coste - actual.coste;
            int total = costePadreHijo + actual.coste;

            if (buscaEnLista(frontera, vecinosNoVisitados[i]) && (vecinosNoVisitados[i].coste > total))
            {
                vecinosNoVisitados[i].coste = total;
                frontera.sort();
            }
            else if(!buscaEnLista(frontera, vecinosNoVisitados[i]))
            {
                vecinosNoVisitados[i].coste = total;
                frontera.push_back(vecinosNoVisitados[i]);
            }
        }

        frontera.sort();
    }
}

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

vector<nodo> obtenVecinos(nodo &actual)
{
    vector<nodo> salida;

    nodo norte;
    norte.fila = actual.fila - 1;
    norte.col = actual.col;
    norte.coste = actual.coste + 1;
    norte.padre = &actual;
    salida.push_back(norte);

    nodo sur;
    sur.fila = actual.fila + 1;
    sur.col = actual.col;
    sur.padre = &actual;
    sur.coste = actual.coste + 1;
    salida.push_back(sur);

    nodo este;
    este.fila = actual.fila;
    este.col = actual.col + 1;
    este.padre = &actual;
    este.coste = actual.coste + 1;
    salida.push_back(este);

    nodo oeste;
    oeste.fila = actual.fila;
    oeste.col = actual.col - 1;
    oeste.coste = actual.coste + 1;
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
    destino.fila = 43;
    destino.col = 22;

    bool hayPlan = pathfinding(origen, destino, mapa);
    cout << hayPlan;
}