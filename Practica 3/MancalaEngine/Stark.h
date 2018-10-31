/*
 * Stark.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"

#ifndef MANUPCBOT_H_
#define MANUPCBOT_H_

class Stark : Bot
{
  public:
	Stark();
	~Stark();

	void initialize();
	string getName();
	Move nextMove(const vector<Move> &adversary, const GameState &state);
	double Poda_AlfaBeta(const GameState &estado, int prof, int &accion, double &alfa, double &beta);
	double Heuristica(const GameState &estado);
};

#endif /* MANUPCBOT_H_ */
