///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
// Copyright 2017 TiWinDeTea.                                                                    //
// This file is part of Force3 project which is released under the                               //
// European Union Public License v1.1. If a copy of the EUPL was                                 //
// not distributed with this software, you can obtain one at :                                   //
// https://joinup.ec.europa.eu/community/eupl/og_page/european-union-public-licence-eupl-v11     //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MESSAGEHANDLER_HPP
#define MESSAGEHANDLER_HPP

enum QtMsgType;
class QMessageLogContext;
class QString;

void q_message_handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // MESSAGEHANDLER_HPP
