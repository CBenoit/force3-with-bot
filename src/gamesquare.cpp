#include "gamesquare.hpp"

#include <QPalette>
#include <QDebug>

Gamesquare::Gamesquare(QWidget *parent) :
	QWidget(parent),
	m_type(square::type::available)
{
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

void Gamesquare::resizeEvent(QResizeEvent*) {
	draw();
}

void Gamesquare::draw() {
	if (m_type == square::type::type_number) {
		qDebug() << "Invalid type!";
	} else {
		setPalette(QPalette(square::COLORS[static_cast<unsigned char>(m_type)]));
	}
}
