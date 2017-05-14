///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "settingsmenu.hpp"
#include "ui_settingsmenu.h"

SettingsMenu::SettingsMenu(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::SettingsMenu)
{
	m_ui->setupUi(this);

	m_ui->modeComboBox->addItem("Player vs AI");
	m_ui->modeComboBox->addItem("Player vs Player");
	m_ui->modeComboBox->addItem("AI vs AI");

	m_ui->rulesComboBox->addItem("Default rules");
	m_ui->rulesComboBox->addItem("Alternative rules");
}

SettingsMenu::~SettingsMenu() {
	delete m_ui;
}

QPushButton* SettingsMenu::getBackToMainMenuButton() {
	return m_ui->backToMainMenuButton;
}
