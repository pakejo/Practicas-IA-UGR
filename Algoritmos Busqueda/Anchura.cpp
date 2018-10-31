/*
Ventajas del algoritmo

La principal ventaja del algoritmo para búsqueda en anchura es su sencillez de implementación
dada la sencillez de su mecanismo de funcionamiento.

Por otro lado, el algoritmo es completo en el sentido de que siempre va a localizar el nodo 
destino y lo va a hacer usando el camimo más corto posible, por su propia dinámica 
interna que exige explorar en primer lugar siempre el nodo más cercano al origen.

Inconvenientes del algoritmo

Desde el punto de vista de optimalidad de la solución localizada, este algoritmo sólo 
funciona si la función de coste es no decreciente en la longitud de cada camino, 
siendo el caso de coste uniforme (todas las acciones o arcos del árbol tienen el mismo coste) 
el caso más sencillo de esta situación.


Rendimiento del algoritmo

En general, para una profundidad  d  la cantidad de nodos a evaluar sería:

h + h2 + h3 + … + hd

Si  d  es suficientemente grande, el último término es mucho mayor que el resto 
y por ello decimos que su complejidad es del orden de  hd  o, equivalentemente, que es  O(hd)

Respecto al espacio de almacenamiento, para el mismo caso anterior, 
almacenar los nodos visitados precisaría de algo proporcional a:

h + h2 + h3 + … + hd-1

Mientras que almacenar la frontera requeriría de algo porporcional a  hd,
por lo que el almacenamiento de la frontera domina al almacenamiento de los nodos visitados
y podemos decir que la complejidad de almacenamiento es  O(hd), igual que para el caso del tiempo.

Estas complejidades exponenciales no son viables para problemas complejos que 
puedan resultar en árboles muy grandes.

Es más grave el tema del almacenamiento, 
porque es fijo y no es tan fácil dimensionarlo como puede ser el tiempo de ejecución.
Así que tenemos que este algoritmo no va a ser viable para problemas de cierto tamaño.
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
    nodo *padre = NULL;
};

vector<vector<char>> cargaMapa(std::string nombre);
vector<nodo> obtenVecinos(nodo &actual);
bool buscaEnLista(list<nodo> &lista, nodo &nodo);

bool pathfinding(const nodo &origen, const nodo &destino, vector<vector<char>> mapa)
{
    list<nodo> frontera;  //Frontera de nodos explorados
    list<nodo> visitados; //Lista de nodos ya visitados

    //Insertamos nodo inicial en la lista
    frontera.push_back(origen);

    //Si el destino es obstaculo devuelvo false
    if ((mapa[destino.fila][destino.col] == 'B') || (mapa[destino.fila][destino.col] == 'A') ||
        (mapa[destino.fila][destino.col] == 'P') || (mapa[destino.fila][destino.col] == 'M') ||
        (mapa[destino.fila][destino.col] == 'D'))
        return false;

    //Si la frontera esta vacia el problema no tiene solucion
    while (!frontera.empty())
    {
        //Sacamos el siguiente nodo de la frontera
        nodo actual = frontera.front();

        //Lo borramos de fronstera y pasamos a visitados
        frontera.pop_front();
        visitados.push_back(actual);

        //Obtenemos los hijos de actual
        vector<nodo> vecinos = obtenVecinos(actual);

        //Comprobamos que nodos no son obstaculos. En tal caso lo pasamos a visitados
        for (int i = 0; i < vecinos.size(); i++)
        {
            if ((mapa[vecinos[i].fila][vecinos[i].col] == 'B') || (mapa[vecinos[i].fila][vecinos[i].col] == 'A') ||
                (mapa[vecinos[i].fila][vecinos[i].col] == 'P') || (mapa[vecinos[i].fila][vecinos[i].col] == 'M') ||
                (mapa[vecinos[i].fila][vecinos[i].col] == 'D'))
                visitados.push_back(vecinos[i]);
        }

        //Separamos los nodos que estan en visitados y la frontera del resto
        vector<nodo> nodosNoEnFronteraNiVisitados;

        for (int i = 0; i < vecinos.size(); i++)
        {
            if (!buscaEnLista(frontera, vecinos[i]) && !buscaEnLista(visitados, vecinos[i]))
                nodosNoEnFronteraNiVisitados.push_back(vecinos[i]);
        }

        //Comprobamos para cada nodo
        for (int i = 0; i < nodosNoEnFronteraNiVisitados.size(); i++)
        {
            //Si el nodo es el destino hemos terminado.
            if (nodosNoEnFronteraNiVisitados[i].col == destino.col && nodosNoEnFronteraNiVisitados[i].fila == destino.fila)
            {
                cout << "Camino encontrado" << endl;
                return true;
            }

            //Si no es el destino lo insertamos en la ffrontera
            frontera.push_back(nodosNoEnFronteraNiVisitados[i]);
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
    norte.padre = &actual;
    salida.push_back(norte);

    nodo sur;
    sur.fila = actual.fila + 1;
    sur.col = actual.col;
    sur.padre = &actual;
    salida.push_back(sur);

    nodo este;
    este.fila = actual.fila;
    este.col = actual.col + 1;
    este.padre = &actual;
    salida.push_back(este);

    nodo oeste;
    oeste.fila = actual.fila;
    oeste.col = actual.col - 1;
    oeste.padre = &actual;
    salida.push_back(oeste);

    return salida;
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
    origen.fila = 15;
    origen.col = 8;

    nodo destino;
    destino.fila = 52;
    destino.col = 49;

    bool hayPlan = pathfinding(origen, destino, mapa);
    cout << hayPlan;
}