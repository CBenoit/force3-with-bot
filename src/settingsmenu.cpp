///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <QString>

#include "settingsmenu.hpp"
#include "ui_settingsmenu.h"

SettingsMenu::SettingsMenu(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::SettingsMenu)
{
	m_ui->setupUi(this);

	// populate rules combobox
	m_ui->rulesComboBox->addItem("Default rules");
	m_ui->rulesComboBox->addItem("Alternative rules");

	// populate brains comboboxes
	QString brains[] = {QString("Player"), QString("AI")};
	for (auto brain : brains) {
		m_ui->redBrainComboBox->addItem(brain);
		m_ui->blueBrainComboBox->addItem(brain);
	}
	m_ui->redBrainComboBox->setCurrentIndex(1);

	// populate AI types comboboxes
	QString ai_types[] = {QString("Default"), QString("Improved"), QString("Better"), QString("Lose win")};
	for (auto type : ai_types) {
		m_ui->redAITypeComboBox->addItem(type);
		m_ui->blueAITypeComboBox->addItem(type);
	}
}

SettingsMenu::~SettingsMenu() {
	delete m_ui;
}

QPushButton* SettingsMenu::getBackToMainMenuButton() {
	return m_ui->backToMainMenuButton;
}

void SettingsMenu::on_blueBrainComboBox_currentIndexChanged(int index) {
	if (index == 1) { // AI selected
		m_ui->blueAITypeComboBox->setDisabled(false);
		m_ui->blueAILevelSpinBox->setDisabled(false);
	} else {
		m_ui->blueAITypeComboBox->setDisabled(true);
		m_ui->blueAILevelSpinBox->setDisabled(true);
	}
}

void SettingsMenu::on_redBrainComboBox_currentIndexChanged(int index) {
	if (index == 1) { // AI selected
		m_ui->redAITypeComboBox->setDisabled(false);
		m_ui->redAILevelSpinBox->setDisabled(false);
	} else {
		m_ui->redAITypeComboBox->setDisabled(true);
		m_ui->redAILevelSpinBox->setDisabled(true);
	}
}
