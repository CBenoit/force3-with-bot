#ifndef MAINMENU_HPP
#define MAINMENU_HPP

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

private slots:
	void on_quitButton_clicked();

private:
	Ui::MainMenu* m_ui;
};

#endif // MAINMENU_HPP
