///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FORCE3_GAMESQUARE_HPP
#define FORCE3_GAMESQUARE_HPP

#include <QWidget>
#include <QColor>

#include "src/logic/boardsquare.hpp"

class QMouseEvent;

namespace square {
	static const std::array<QColor,static_cast<int>(type::size)> COLORS = {{ COLORS_AND_PLAYERS(AS_COLORS) }};
}

class Gamesquare : public QWidget
{
	Q_OBJECT

public:

	explicit Gamesquare(QPoint id, QWidget *parent = nullptr);

	void type(square::type new_type);

	square::type type() const;

	void swap(Gamesquare& other);

signals:
	void pressed(int x, int y);
	void released(int x, int y);


protected:
	virtual void mousePressEvent(QMouseEvent* event) override;

	virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
	void draw();

	square::type m_type;
	const QPoint m_id;
};

#endif // FORCE3_GAMESQUARE_HPP
