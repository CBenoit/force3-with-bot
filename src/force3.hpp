#ifndef FORCE3_HPP
#define FORCE3_HPP

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
	void on_actionQuit_triggered();
	void setMainMenu();
	void setGameboard();

private:
	void switchScene(QWidget* oldWidget, QWidget* newWidget);

	Ui::Force3* m_ui;
	QWidget* m_currentWidget;
};

#endif // FORCE3_HPP
