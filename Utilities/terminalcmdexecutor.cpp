/*
 * Copyright (c) 2016 Technology Incubator Pty Ltd. All rights reserved.
 *
 * @file:   terminalcmdexecuter.cpp
 * @brief:  TerminalCmdExecuter class implementation.
 *
 */

#include <QProcess>

#include "terminalcmdexecutor.h"

static const QString TERMINAL_CMD_TOOL      = "xdotool ";


void TerminalCmdExecutor::LockScreen()
{
    QProcess* proc = new QProcess();
    proc->start(TERMINAL_CMD_TOOL + "key super+l" );
    proc->waitForFinished();
    proc->close();
}

void TerminalCmdExecutor::UnlockScreen(QString pass)
{
    QProcess* proc = new QProcess();
    proc->start(TERMINAL_CMD_TOOL + "type " + pass);
    proc->waitForFinished();

    proc->start(TERMINAL_CMD_TOOL + "key --delay 50 Return");
    proc->waitForFinished();

    proc->close();
}

void TerminalCmdExecutor::Reboot()
{
    QProcess* proc = new QProcess();
    proc->startDetached("sudo shutdown -r now");

    proc->close();
}

