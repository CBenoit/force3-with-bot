#ifndef SETTINGSMENU_HPP
#define SETTINGSMENU_HPP

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

private:
	Ui::SettingsMenu *m_ui;
};

#endif // SETTINGSMENU_HPP
