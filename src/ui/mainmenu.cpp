///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "ui/mainmenu.hpp"
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

QPushButton* MainMenu::getSettingsButton() {
	return m_ui->settingsButton;
}

void MainMenu::on_quitButton_clicked() {
	QCoreApplication::quit();
}
