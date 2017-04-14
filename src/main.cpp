#include "force3.hpp"
#include <QApplication>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Force3 w;
	w.show();

	return a.exec();
}
