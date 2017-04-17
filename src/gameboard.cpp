#include "gameboard.hpp"
#include "ui_gameboard.h"
#include "gamesquare.hpp"

#include <QResizeEvent>
#include <QGridLayout>

Gameboard::Gameboard(QWidget *parent) :
	QWidget(parent),
	m_layout(new QGridLayout),
	m_ui(new Ui::Gameboard)
{
	m_ui->setupUi(this);
	for (unsigned char i{3} ; i-- ;) {
		for (unsigned char j{3} ; j-- ;) {
			m_squares[i][j] = new Gamesquare();
			m_layout->addWidget(m_squares[i][j], i, j);
		}
	}
	m_squares[1][1]->type(square::type::empty_square);
	this->setLayout(m_layout);
	resizeEvent(nullptr);
	show();
}

Gameboard::~Gameboard() {
	for (unsigned char i{3} ; i-- ;) {
		for (unsigned char j{3} ; j-- ;) {
			delete m_squares[i][j];
		}
	}
	delete m_ui;
	delete m_layout;
}

void Gameboard::square(unsigned char x, unsigned char y, square::type pawn_type) {
	m_squares[x][y]->type(pawn_type);
}

square::type Gameboard::square(unsigned char x, unsigned char y) const {
	return m_squares[x][y]->type();
}

void Gameboard::swap(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2) {
	m_squares[x1][y1]->swap(*m_squares[x2][y2]);
}

void Gameboard::resizeEvent(QResizeEvent*) {
	const QSize current_size(size().width() / 3, size().height() / 3);
	for (unsigned char i{3} ; i-- ;) {
		for (unsigned char j{3} ; j-- ;) {
			m_squares[i][j]->resize(current_size);
		}
	}
}

void Gameboard::draw() {

}
