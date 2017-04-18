///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Breep project which is released under the                                //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

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
