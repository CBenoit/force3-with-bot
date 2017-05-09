#include "settingsmenu.hpp"
#include "ui_settingsmenu.h"

SettingsMenu::SettingsMenu(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::SettingsMenu)
{
	m_ui->setupUi(this);

	m_ui->modeComboBox->addItem("Player vs AI");
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
