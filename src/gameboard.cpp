#include "gameboard.hpp"
#include "ui_gameboard.h"

Gameboard::Gameboard(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::Gameboard)
{
	m_ui->setupUi(this);
}

Gameboard::~Gameboard() {
	delete m_ui;
}
