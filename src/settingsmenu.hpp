///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FORCE3_SETTINGSMENU_HPP
#define FORCE3_SETTINGSMENU_HPP

#include <QWidget>
#include <QPushButton>

namespace Ui {
class SettingsMenu;
}

class SettingsMenu : public QWidget
{
	Q_OBJECT

public:
	explicit SettingsMenu(QWidget *parent = 0);
	~SettingsMenu();

	QPushButton* getBackToMainMenuButton();

private slots:
	void on_blueBrainComboBox_currentIndexChanged(int index);

	void on_redBrainComboBox_currentIndexChanged(int index);

private:
	Ui::SettingsMenu *m_ui;
};

#endif // FORCE3_SETTINGSMENU_HPP
