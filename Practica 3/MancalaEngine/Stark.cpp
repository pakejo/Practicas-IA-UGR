/*
 * Stark.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Stark.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

Stark::Stark()
{
	// Inicializar las variables necesarias para ejecutar la partida
}

Stark::~Stark()
{
	// Liberar los recursos reservados (memoria, ficheros, etc.)
}

void Stark::initialize()
{
	// Inicializar el bot antes de jugar una partida
}

string Stark::getName()
{
	return "Stark"; // Sustituir por el nombre del bot
}

Move Stark::nextMove(const vector<Move> &adversary, const GameState &state)
{

	Move movimiento = M_NONE;
	GameState estado_actual = state;
	double alfa = -9999999999.0;
	double beta = 9999999999.0;
	int valor, accion;

	valor = Poda_AlfaBeta(estado_actual, 10, accion, alfa, beta);
	movimiento = (Move)accion;

	return movimiento;
}

double Stark::Poda_AlfaBeta(const GameState &estado, int prof, int &accion, double &alfa, double &beta)
{
	GameState hijo;
	double mejorValor;
	bool poda = false;
	Player stark = this->getPlayer();

	//Si la profundida es 0 o es un nodo final devolver la heuristica
	if (prof == 0 || estado.isFinalState())
	{
		int valor;
		valor = Heuristica(estado);
		return valor;
	}

	//Si somos el jugador 1 jugamos como MAX
	if (estado.getCurrentPlayer() == stark)
	{

		mejorValor = alfa;

		//Vamos obteniendo todos las opciones posibles del estado actual
		for (int i = 1; i < 7 && !poda; ++i)
		{
			hijo = estado.simulateMove((Move)i);
			int otra = i;
			double valorHijo = Poda_AlfaBeta(hijo, prof - 1, otra, mejorValor, beta);

			//Si el valor del hijo es mejor que el que ya llevamos elegiremos esa opcion (el hijo es mas viable)
			if (valorHijo > mejorValor)
				accion = i;

			//Alfa sera por tanto el maximo entre el mejorValor que llevamos y el obenido en el hijo
			mejorValor = max(mejorValor, valorHijo);

			//En el caso de que beta sea menor a nuestro alfa realizamos la poda (break)
			if (beta <= mejorValor)
				poda = true;
		}
	}
	else //En caso contrario jugaremos como MIN
	{

		mejorValor = beta;

		//Vamos obteniendo todos las opciones posibles del estado actual
		for (int i = 1; i < 7 && !poda; ++i)
		{
			hijo = estado.simulateMove((Move)i);
			int otra = i;

			double valorHijo = Poda_AlfaBeta(hijo, prof - 1, otra, alfa, mejorValor);

			//Si el valor del hijo es mejor que el que ya llevamos elegiremos esa opcion (el hijo es mas viable)
			if (valorHijo < mejorValor)
				accion = i;

			//Beta sera por tanto el minimo entre el mejorValor que llevamos y el obenido en el hijo
			mejorValor = min(mejorValor, valorHijo);

			//Si beta ess menor a alfa realizamos la poda (break)
			if (mejorValor <= alfa)
				poda = true;
		}
	}

	return mejorValor;
}

double Stark::Heuristica(const GameState &estado)
{
	Player miBot = this->getPlayer();
	double puntuacion = 0.0;

	Player contrincante;

	if (miBot == Player::J1)
		contrincante = Player::J2;
	else
		contrincante = Player::J1;

	//Comprobar si puedo meter una para turno extra
	bool Extra = false;
	bool encontrado = false;

	for (int i = 1; i < 7 && !encontrado; i++)
	{
		if (estado.getSeedsAt(miBot, (Position)i) == i)
		{
			puntuacion += 10 * estado.getScore(miBot) - estado.getScore(contrincante);
			Extra = true;
			encontrado = true;
		}
	}

	//Comprobar si puedo dar la vuelta
	if (!Extra)
	{
		for (int i = 0; i < 7; i++)
		{
			int temp = estado.getSeedsAt(miBot, (Position)i);
			int sobrantes = temp - i - 7;

			if (sobrantes > 0) //Si puedo dar la vuelta compuebo compruebo si caigo en casilla vacia
			{
				//Casilla donde va a caer la ultima semilla de la vuelta estaba vacia
				if (estado.getSeedsAt(miBot, (Position)(7 - sobrantes)) == 0)
					puntuacion += 2 * estado.getScore(miBot) - estado.getScore(contrincante);

				else //Si no cae en vacia comprobar si cae en una que pueda marcar extra
				{
					int ultCasilla = 7 - sobrantes;

					if (estado.getSeedsAt(miBot, (Position)ultCasilla) + 1 == ultCasilla)
						puntuacion += 2 * estado.getScore(miBot) - estado.getScore(contrincante);
					else
						puntuacion -= estado.getScore(contrincante) + estado.getScore(miBot);
				}
			}
			//Le sumo 6 que serian las que se quedaria el otro
			else if (sobrantes + 6 != 0)
			{
				sobrantes += 6;

				//Comprobar si las que pasan al contrario le quita un turno extra
				if (estado.getSeedsAt(contrincante, (Position)(7 - sobrantes)) == (7 - sobrantes))
				{
					puntuacion += 2 * estado.getScore(miBot) - estado.getScore(contrincante);
				}

				//Comprobar si mi movimiento le da turno extra al otro
				if (estado.getSeedsAt(contrincante, (Position)(7 - sobrantes)) == (7 - sobrantes) + 1)
				{
					puntuacion -= estado.getScore(contrincante) + estado.getScore(miBot);
				}
			}
		}
	}

	return puntuacion;
}