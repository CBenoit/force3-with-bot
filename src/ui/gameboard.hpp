///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FORCE3_GAMEBOARD_HPP
#define FORCE3_GAMEBOARD_HPP

#include <QWidget>

#include "logic/gamestate.hpp"
#include "logic/ai.hpp"
#include "common/move.hpp"

class QGridLayout;

class Gamesquare;

namespace square {
enum class type: unsigned char;
}

class QPoint;

class Gameboard : public QWidget
{
	Q_OBJECT

public:

	explicit Gameboard(QWidget* parent = nullptr);
	Gameboard(const Gameboard&) = delete;
	Gameboard& operator=(const Gameboard&) = delete;
	~Gameboard();

	void square(unsigned char x, unsigned char y, square::type pawn_type);
	square::type square(unsigned char x, unsigned char y) const;

	void swap(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);

	static std::pair<uint, heuristic::function_t> blue_brain;
	static std::pair<uint, heuristic::function_t> red_brain;

	static unsigned char blue_depth;
	static unsigned char red_depth;

	static bool blue_is_ai;
	static bool red_is_ai;

	static bool alternative_rules;
signals:
	void winner_detected(bool m_blue_win);

private slots:
	void gamesquare_pressed(int x, int y);
	void gamesquare_released(int x, int y);
	void AI_play();

protected:
	virtual void resizeEvent(QResizeEvent* event) override;

private:
	void draw();

	void play(move::Slide);
	void play(move::Swap);
	void play(move::SetColor);

	void next_turn();

	bool ia_turn() const;

	bool m_blue_turn;
	bool m_game_finished;
	std::array<std::array<Gamesquare*,3>,3> m_squares;
	QGridLayout* m_layout;
	QPoint m_last_square_pressed;
	GameState m_game_state;
	AI m_blue_ai;
	AI m_red_ai;
};

#endif // FORCE3_GAMEBOARD_HPP
