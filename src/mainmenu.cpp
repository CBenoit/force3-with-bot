#include "src/mainmenu.hpp"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::MainMenu)
{
	m_ui->setupUi(this);
}

MainMenu::~MainMenu() {
	delete m_ui;
}

QPushButton* MainMenu::getStartButton() {
	return m_ui->startButton;
}

void MainMenu::on_quitButton_clicked() {
	QCoreApplication::quit();
}
