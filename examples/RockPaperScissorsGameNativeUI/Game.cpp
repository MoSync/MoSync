/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file Game.cpp
 * @author Emma Tresanszki
 *
 * This file contains the game logic for the game.
 *
 * Game logic:
 *  - The game is started by clicking the Start button, and that enables the
 *    user to select a weapon from the 3 available ones.
 *  - The opponent's weapon is chosen randomly, and it is shown only after
 *    the user has picked his weapon.
 *  - The game result is displayed using two strings: who won the game and why.
 *  - The score info at the bottom is updated after each game is finished.
 *  - Game can be restarted by pressing the Start button again.
 */

// Include library for random number generator.
#include <mastdlib.h>

// Include library for local time.
#include <maapi.h>

// Include header file for the game class.
#include "Game.h"

namespace RockPaperScissors
{

/**
 * Constructor for the Game class.
 * Here instance variables are initialized.
 */
Game::Game():
	mWins(0),
	mLoses(0),
	mTies(0),
	mTotalRounds(0),
	mWinState(""),
	mExplanation("")
{
	// Create a seed for the random number generator using
	// the current time stamp. This will give us different
	// random sequences each time the game is played.
	srand(maTime());
}

/*
 * Empty destructor.
 * We don't need to do any special cleanup.
 */
Game::~Game()
{
}

/**
 * Set the currently used weapon.
 * @param myWeapon The weapon to be used.
 */
void Game::setMyWeapon(const eWeapons myWeapon)
{
	mMyWeapon = myWeapon;
}

/**
 * Set a random weapon for opponent.
 */
void Game::setRandomOpponentWeapon()
{
	int mWeapon = rand() % 3;

	switch (mWeapon)
	{
		case 0:
		{
			mOpponentWeapon = ROCK;
			break;
		}
		case 1:
		{
			mOpponentWeapon = PAPER;
			break;
		}
		case 2:
		{
			mOpponentWeapon = SCISSORS;
			break;
		}
	}
}

/**
 * Get the opponent's weapon.
 * @return The constant describing the weapon.
 */
eWeapons Game::getOpponentWeapon()
{
	return mOpponentWeapon;
}

/**
 * Get the total number of rounds played.
 * @return Number of rounds.
 */
int Game::getTotalRounds()
{
	return mTotalRounds;
}

/**
 * Get the total number of losses for the player.
 * @return Number of losses.
 */
int Game::getLosesNr()
{
	return mLoses;
}

/**
 * Get the total number of wins for the player.
 * @return Number of wins.
 */
int Game::getWins()
{
	return mWins;
}

/**
 * Get the total number of ties.
 * @return Number of ties.
 */
int Game::getTies()
{
	return mTies;
}

/**
 * Get the explanation for the outcome of
 *  the last round in the game.
 * @return String with the explanation, for example:
 *  "ROCK breaks SCISSORS"
 */
const MAUtil::String& Game::getExplanation()
{

	return mExplanation;
}

/**
 * Get the winner of the last round in the game.
 * @return String with winner, for example: "YOU WIN!"
 */
const MAUtil::String& Game::getWinState()
{
	return mWinState;
}

/**
 * Compute the outcome of the current round, based
 * on the players weapon, and the opponent's weapon.
 */
void Game::computeGameResult(){

	mTotalRounds++;

	if (mMyWeapon == mOpponentWeapon)
	{
		// If weapons are equal, it is a tie.
		mResult = TIE;
		mWinState = "IT'S A TIE!";
		mExplanation = " ";
		mTies++;
	}
	else
	{
		// Weapons are not equal, determine who wins.
		switch (mMyWeapon)
		{
			case ROCK:
			{
				// Rock wins against Scissors.
				if (mOpponentWeapon == SCISSORS)
				{
					mResult = WIN;
					mWinState = YOU_WIN;
					mExplanation = ROCK_vs_SCISSORS;
					mWins++;
				}
				else
				{
					// Rock loses against Paper.
					mExplanation = PAPER_vs_ROCK;
					mWinState = YOU_LOSE;
					mResult = LOSS;
					mLoses++;
				}
				break;
			}
			case PAPER:
			{
				// Paper wins against Rock.
				if (mOpponentWeapon == ROCK)
				{
					mResult = WIN;
					mWinState = YOU_WIN;
					mExplanation = PAPER_vs_ROCK;
					mWins++;
				}
				else
				{
					// Paper loses against Scissors.
					mWinState = YOU_LOSE;
					mExplanation = SCISSORS_vs_PAPER;
					mResult = LOSS;
					mLoses++;
				}
				break;
			}
			case SCISSORS:
			{
				// Scissors wins against Paper.
				if (mOpponentWeapon == PAPER)
				{
					mResult = WIN;
					mWinState = YOU_WIN;
					mExplanation = SCISSORS_vs_PAPER;
					mWins++;
				}
				else
				{
					// Scissors loses against Rock.
					mWinState = YOU_LOSE;
					mExplanation = ROCK_vs_SCISSORS;
					mResult = LOSS;
					mLoses++;
				}
				break;
			}
		}
	}
}

} // namespace RockPaperScissors
