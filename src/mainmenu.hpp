#ifndef MAINMENU_HPP
#define MAINMENU_HPP

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Breep project which is released under the                                //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <QWidget>
#include <QPushButton>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
	Q_OBJECT

public:
	explicit MainMenu(QWidget* parent = 0);
	~MainMenu();

	QPushButton* getStartButton(); // FIXME: better way to connect to this button's click signal?

	QPushButton* getSettingsButton(); // FIXME: same here.

private slots:
	void on_quitButton_clicked();

private:
	Ui::MainMenu* m_ui;
};

#endif // MAINMENU_HPP
