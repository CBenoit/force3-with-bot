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
#include "gameboard.hpp"

static heuristic::function_t function_table[] = {&heuristic::easy, &heuristic::normal, &heuristic::hard, &heuristic::legendary};

SettingsMenu::SettingsMenu(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::SettingsMenu)
{
	m_ui->setupUi(this);

	// populate rules combobox
	m_ui->rulesComboBox->addItem("Default rule set");
	m_ui->rulesComboBox->addItem("Alternative rule set");
	m_ui->rulesComboBox->setCurrentIndex(Gameboard::alternative_rules ? 1 : 0);

	// populate brains comboboxes
	QString brains[] = {"Player", "AI"};
	for (auto brain : brains) {
		m_ui->redBrainComboBox->addItem(brain);
		m_ui->blueBrainComboBox->addItem(brain);
	}
	m_ui->redBrainComboBox->setCurrentIndex(Gameboard::red_is_ai);
	m_ui->blueBrainComboBox->setCurrentIndex(Gameboard::blue_is_ai);

	// populate AI types comboboxes
	QString ai_types[] = {"Easy", "Normal", "Hard", "Legendary"};
	for (auto type : ai_types) {
		m_ui->redAITypeComboBox->addItem(type);
		m_ui->blueAITypeComboBox->addItem(type);
	}
	m_ui->redAITypeComboBox->setCurrentIndex(Gameboard::red_brain.first);
	m_ui->blueAITypeComboBox->setCurrentIndex(Gameboard::blue_brain.first);

	m_ui->redAILevelSpinBox->setValue(Gameboard::red_depth);
	m_ui->blueAILevelSpinBox->setValue(Gameboard::blue_depth);
}

SettingsMenu::~SettingsMenu() {
	auto idx = m_ui->redAITypeComboBox->currentIndex();
	Gameboard::red_brain = {static_cast<uint>(idx), function_table[idx]};
	Gameboard::red_depth = static_cast<uint>(m_ui->redAILevelSpinBox->value());
	Gameboard::red_is_ai = (m_ui->redBrainComboBox->currentIndex() == 1);

	idx = m_ui->blueAITypeComboBox->currentIndex();
	Gameboard::blue_brain = {static_cast<uint>(idx), function_table[idx]};
	Gameboard::blue_depth = static_cast<uint>(m_ui->blueAILevelSpinBox->value());
	Gameboard::blue_is_ai = (m_ui->blueBrainComboBox->currentIndex() == 1);

	Gameboard::alternative_rules = (m_ui->rulesComboBox->currentIndex() == 1);
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
