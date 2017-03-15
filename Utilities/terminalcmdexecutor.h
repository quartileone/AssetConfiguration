/*
 * Copyright (c) 2016 Technology Incubator Pty Ltd. All rights reserved.
 *
 * @file:   terminalcmdexecuter.h
 * @brief:  Class that responsible for commands that will be executed for terminal.
 * Such as reboot, lock unlock screen.
 *
 */

#ifndef TERMINALCMDEXECUTOR_H
#define TERMINALCMDEXECUTOR_H

#include <QString>

class TerminalCmdExecutor
{
public:
    static void LockScreen();

    static void UnlockScreen(QString pass);

    static void Reboot();

private:
    TerminalCmdExecutor()
    {
    }
};

#endif // TERMINALCMDEXECUTOR_H
