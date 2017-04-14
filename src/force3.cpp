#include "force3.hpp"
#include "ui_force3.h"
#include "mainmenu.hpp"
#include "gameboard.hpp"

Force3::Force3(QWidget* parent) :
	QMainWindow(parent),
	m_ui(new Ui::Force3),
	m_currentWidget(nullptr)
{
	m_ui->setupUi(this);
	setMainMenu();
}

Force3::~Force3() {
	delete m_ui;
	if (m_currentWidget != nullptr) {
		delete m_currentWidget;
	}
}

void Force3::on_actionQuit_triggered() {
	QCoreApplication::quit();
}

void Force3::setMainMenu() {
	MainMenu* mainMenu = new MainMenu();
	switchScene(m_currentWidget, mainMenu);
	connect(mainMenu->getStartButton(), SIGNAL(clicked()), this, SLOT(setGameboard()));
}

void Force3::setGameboard() {
	switchScene(m_currentWidget, new Gameboard());
}

void Force3::switchScene(QWidget* oldWidget, QWidget* newWidget) {
	if (oldWidget != nullptr) {
		delete oldWidget;
		oldWidget = nullptr;
	}

	if (newWidget != nullptr) {
		m_ui->centralWidget->layout()->addWidget(newWidget);
	}

	m_currentWidget = newWidget;
}
