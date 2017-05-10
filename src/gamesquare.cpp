///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "gamesquare.hpp"

#include <QMouseEvent>
#include <QDebug>

Gamesquare::Gamesquare(QPoint id, QWidget *parent):
	QWidget(parent),
	m_type(square::type::available),
	m_id(id)
{
	draw();
	setAutoFillBackground(true);
}


void Gamesquare::type(square::type new_type) {
	m_type = new_type;
	draw();
}

square::type Gamesquare::type() const {
	return m_type;
}

void Gamesquare::swap(Gamesquare& other) {
	square::type tmp = other.type();
	other.type(this->m_type);
	type(tmp);
}

void Gamesquare::mousePressEvent(QMouseEvent*) {
	emit pressed(m_id.x(), m_id.y());
}

void Gamesquare::mouseReleaseEvent(QMouseEvent* event) {
	QSize size_ = size();
	int x;
	if (event->pos().x() <= 0) {
		x = (event->pos().x() - size_.width()) / size_.width() + m_id.x();
	} else {
		x = (event->pos().x() + size_.width()) / size_.width() - 1 + m_id.x();
	}

	if (event->pos().y() <= 0) {
		emit released(x, (event->pos().y() - size_.height()) / size_.height() + m_id.y());
	} else {
		emit released(x, (event->pos().y() + size_.height()) / size_.height() - 1 + m_id.y());
	}
}

void Gamesquare::draw() {
	if (m_type == square::type::size    ) {
		qDebug() << "Invalid type!";
	} else {
		setPalette(QPalette(square::COLORS[static_cast<unsigned char>(m_type)]));
	}
}
