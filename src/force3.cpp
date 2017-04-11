#include "force3.hpp"
#include "ui_force3.h"

Force3::Force3(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::Force3)
{
	ui->setupUi(this);
}

Force3::~Force3()
{
	delete ui;
}
