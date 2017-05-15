///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FORCE3_FORCE3_HPP
#define FORCE3_FORCE3_HPP

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class Force3;
}

class Force3 : public QMainWindow
{
	Q_OBJECT

public:
	explicit Force3(QWidget* parent = 0);
	~Force3();

private slots:
	void setMainMenu();
	void setGameboard();
	void setSettings();

private:
	void switchScene(QWidget* oldWidget, QWidget* newWidget);

	Ui::Force3* m_ui;
	QWidget* m_currentWidget;
};

#endif // FORCE3_FORCE3_HPP
