///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "force3.hpp"
#include "ui_force3.h"
#include "mainmenu.hpp"
#include "gameboard.hpp"
#include "settingsmenu.hpp"

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

void Force3::setMainMenu() {
	MainMenu* mainMenu = new MainMenu();
	switchScene(m_currentWidget, mainMenu);
	connect(mainMenu->getStartButton(), SIGNAL(clicked()), this, SLOT(setGameboard()));
	connect(mainMenu->getSettingsButton(), SIGNAL(clicked()), this, SLOT(setSettings()));
}

void Force3::setGameboard() {
	switchScene(m_currentWidget, new Gameboard());
}

void Force3::setSettings() {
	SettingsMenu* settingsMenu = new SettingsMenu();
	switchScene(m_currentWidget, settingsMenu);
	connect(settingsMenu->getBackToMainMenuButton(), SIGNAL(clicked()), this, SLOT(setMainMenu()));
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
