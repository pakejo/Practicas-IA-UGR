/*
Como todo algoritmo de búsqueda en amplitud, A* es un algoritmo completo:
en caso de existir una solución, siempre dará con ella.

Si para todo nodo n del grafo se cumple g(n)=0 nos encontramos ante una búsqueda voraz.
Si para todo nodo n del grafo se cumple h(n)=0, A* pasa a ser una búsqueda de coste uniforme no informada.

Para garantizar la optimización del algoritmo, la función h(n) debe ser heurística admisible,
esto es, que no sobrestime el coste real de alcanzar el nodo objetivo.

De no cumplirse dicha condición, el algoritmo pasa a denominarse simplemente A,
y a pesar de seguir siendo completo, no se asegura que el resultado obtenido sea 
el camino de coste mínimo. Asimismo, si garantizamos que h(n) es consistente, es decir, 
que para cualquier nodo n y cualquiera de sus sucesores, el coste estimado de alcanzar 
el objetivo desde n no es mayor que el de alcanzar el sucesor más el coste de alcanzar
el objetivo desde el sucesor.

La complejidad computacional del algoritmo está íntimamente relacionada con la 
calidad de la heurística que se utilice en el problema. En el caso peor, 
con una heurística de pésima calidad, la complejidad será exponencial, 
mientras que en el caso mejor, con una buena h(n), el algoritmo se ejecutará en tiempo lineal.
*/

#include <iostream>
#include <list>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

enum Action {actFORWARD, actTURN_L, actTURN_R, actIDLE};

struct nodo
{
    int fila;
    int col;
    int orientacion;
    double g = 0.0;
    double h = 0.0;
    double f = 0.0;
    nodo *padre;

    bool operator<(const nodo &b) { return f < b.f; }
    bool operator==(const nodo &b) { return (fila == b.fila) && (col == b.col); }

    void CalculaF(nodo destino)
    {
        g = sqrt(pow((fila - padre->fila), 2) + pow((col - padre->col), 2)) + padre->g;
        //h = sqrt(pow((fila - destino.fila), 2) + pow((col - destino.col), 2));
        h = abs(fila - destino.fila) + abs(col - destino.col);
        f = g + h;
    }
};


void construyePlan(vector<nodo> camino, list<Action> plan)
{

	int i = 1;
	int j = 0;

	while (j < camino.size())
	{
		//El vecino esta al norte
		if ((camino[i].fila + 1 == camino[j].fila) && (camino[i].col == camino[j].col))
		{
			switch (camino[j].orientacion)
			{
			case 0:
				plan.push_back(actFORWARD);
				break;

			case 1:
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;

			case 2:
				plan.push_back(actTURN_R);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;

			case 3:
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				break;
			}
			camino[i].orientacion = 0;
		}

		//El vecino esta al sur
		if ((camino[i].fila - 1 == camino[j].fila) && (camino[i].col == camino[j].col))
		{
			switch (camino[j].orientacion)
			{
			case 0:
				plan.push_back(actTURN_R);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;

			case 1:
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;

			case 2:
				plan.push_back(actFORWARD);
				break;

			case 3:
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				break;
			}

			camino[i].orientacion = 2;
		}

		//El vecino esta al este
		if ((camino[i].fila == camino[j].fila) && (camino[i].col - 1 == camino[j].col))
		{
			switch (camino[j].orientacion)
			{
			case 0:
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;

			case 1:
				plan.push_back(actFORWARD);
				break;

			case 2:
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				break;

			case 3:
				plan.push_back(actTURN_R);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;
			}

			camino[i].orientacion = 2;
		}

		//El vecino esta al oeste
		if ((camino[i].fila == camino[j].fila) && (camino[i].col + 1 == camino[j].col))
		{
			switch (camino[j].orientacion)
			{
			case 0:
				plan.push_back(actTURN_L);
				plan.push_back(actFORWARD);
				break;

			case 1:
				plan.push_back(actTURN_R);
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;

			case 2:
				plan.push_back(actTURN_R);
				plan.push_back(actFORWARD);
				break;

			case 3:
				plan.push_back(actFORWARD);
				break;
			}

			camino[i].orientacion = 3;
		}

		i++, j++;
	}
}



bool esObstaculo(nodo nodo, vector<vector<char>> mapa)
{
    if ((mapa[nodo.fila][nodo.col] == 'B') || (mapa[nodo.fila][nodo.col] == 'A') ||
        (mapa[nodo.fila][nodo.col] == 'P') || (mapa[nodo.fila][nodo.col] == 'M') ||
        (mapa[nodo.fila][nodo.col] == 'D'))
        return true;
    return false;
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
    salida.push_back(norte);

    nodo sur;
    sur.fila = actual.fila + 1;
    sur.col = actual.col;
    salida.push_back(sur);

    nodo este;
    este.fila = actual.fila;
    este.col = actual.col + 1;
    salida.push_back(este);

    nodo oeste;
    oeste.fila = actual.fila;
    oeste.col = actual.col - 1;
    salida.push_back(oeste);

    return salida;
}

vector<vector<char>> cargaMapa(std::string nombre)
{

    vector<vector<char>> mapa;
    ifstream file;
    std::string linea;
    string n, m;
    int filas, cols;

    file.open(nombre);

    std::getline(file, n);
    std::getline(file, m);

    filas = std::stoi(n);
    cols = std::stoi(m);

    mapa.resize(filas);
    for (int i = 0; i < filas; i++)
        mapa[i].resize(cols);

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

bool pathfinding(nodo &origen, nodo &destino, vector<vector<char>> mapa)
{
    list<nodo> abierta;
    list<nodo> cerrada;

    list<Action> plan;

    abierta.push_back(origen);

    if (esObstaculo(destino, mapa))
        return false;

    while (!abierta.empty())
    {
        nodo actual = abierta.front();
        camino.push_back(actual);

        if (actual == destino)
        {
            cout << "Destino encotrado" << endl;
            cout << "Construyendo plan" << endl;
            construyePlan(camino,plan);
            return true;
        }

        abierta.pop_front();
        cerrada.push_back(actual);

        vector<nodo> vecinos = obtenVecinos(actual);

        vector<nodo> vecinosNoObstaculos;

        for (int i = 0; i < vecinos.size(); i++)
        {
            vecinos[i].padre = &actual;

            if (!esObstaculo(vecinos[i], mapa))
            {
                vecinosNoObstaculos.push_back(vecinos[i]);
            }
            else
                cerrada.push_back(vecinos[i]);
        }

        for (int i = 0; i < vecinosNoObstaculos.size(); i++)
        {
            vecinosNoObstaculos[i].CalculaF(destino);

            if (!buscaEnLista(abierta, vecinosNoObstaculos[i]))
            {
                abierta.push_back(vecinosNoObstaculos[i]);
            }
            else
            {
                for (auto it = abierta.begin(); it != abierta.end(); ++it)
                {
                    if (*it == vecinosNoObstaculos[i])
                    {
                        if (it->g > vecinosNoObstaculos[i].g)
                        {
                            *it = vecinosNoObstaculos[i];
                        }
                    }
                }
            }
        }

        abierta.sort();
    }

    return false;
}

int main()
{
    vector<vector<char>> mapa;

    mapa = cargaMapa("mapas/cuadricula.map");

    nodo origen;
    origen.fila = 96;
    origen.col = 15;
    origen.orientacion = 0;

    nodo destino;
    destino.fila = 91;
    destino.col = 45;

    bool hayPlan = pathfinding(origen, destino, mapa);
    cout << hayPlan;
}