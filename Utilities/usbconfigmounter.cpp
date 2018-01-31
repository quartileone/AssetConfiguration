/*
 * Copyright (c) 2016 Technology Incubator Pty Ltd. All rights reserved.
 *
 * @file:   usbconfigmounter.cpp
 * @brief:  UsbConfigMounter class implementation.
 *
 */

#include <QDebug>
#include <QProcess>

#include "usbconfigmounter.h"

#define MOUNT_TOOL     "sudo mount "
#define UNMOUNT_TOOL   "sudo umount "

UsbConfigMounter::UsbConfigMounter(QDir usbFlashDir)
    : m_usbFlashDir(usbFlashDir)
{
}

bool UsbConfigMounter::MountConfigUsbFlash(QString mountPath)
{
    m_usbMountedDir = mountPath + QDir::separator() + m_usbFlashDir.dirName();
    if (!m_usbMountedDir.exists()){
      m_usbMountedDir.mkdir(mountPath + QDir::separator() + m_usbFlashDir.dirName());
    }

    QProcess* proc = new QProcess();
    QString strMount = MOUNT_TOOL;
    strMount += "-t auto ";
    strMount += m_usbFlashDir.absolutePath();
    strMount += ' ';
    strMount += m_usbMountedDir.absolutePath();

    proc->start(strMount);
    proc->waitForFinished();

    if (proc->exitStatus() == QProcess::CrashExit) {
        return false;
    }
    proc->close();

    return true;
}

void UsbConfigMounter::UnmountConfigUsbFlash(QString mountPath)
{
    QProcess* proc = new QProcess();
    QString strMount = UNMOUNT_TOOL;
    strMount += m_usbFlashDir.absolutePath();

    proc->start(strMount);
    proc->waitForFinished();
    if (proc->exitStatus() == QProcess::CrashExit) {
        qDebug() << "ERROR" << endl;
        return;
    }
    proc->close();

    if (m_usbMountedDir.exists()){
      m_usbMountedDir.rmdir(mountPath + QDir::separator() + m_usbFlashDir.dirName());
    }

    m_usbMountedDir = QString::null;
}

bool UsbConfigMounter::IsConfigUsbFlash()
{
    if (!m_usbMountedDir.exists()) {
        return false;
    }

    QStringList filters;
    filters << USB_CONFIG_FILE_NAME;
    m_usbMountedDir.setNameFilters(filters);

    return m_usbMountedDir.entryList(QDir::Files).size() > 0;
}

bool UsbConfigMounter::IsMounteDirExist(QString mountPath)
{
    QDir usbMountedDir(mountPath + QDir::separator() + m_usbFlashDir.dirName());

    return usbMountedDir.exists();
}


