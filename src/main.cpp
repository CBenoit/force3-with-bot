#include <QApplication>
#include <QtGlobal> // qInstallMessageHandler

#include "force3.hpp"
#include "messagehandler.hpp"

int main(int argc, char* argv[]) {
	qInstallMessageHandler(q_message_handler);

	QApplication a(argc, argv);
	Force3 w;
	w.show();

	return a.exec();
}
