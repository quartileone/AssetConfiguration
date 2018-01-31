/*
 * Copyright (c) 2016 Technology Incubator Pty Ltd. All rights reserved.
 *
 * @file:   usbconfigmounter.h
 * @brief:  Class that work with attached USB flash that contains configuration file.
 *
 */

#ifndef USBCONFIGMOUNTER_H
#define USBCONFIGMOUNTER_H

#include <QString>
#include <QDir>

#define USB_CONFIG_FILE_NAME "Config.json"

class UsbConfigMounter
{
public:
    UsbConfigMounter(QDir usbFlashDir);

    bool MountConfigUsbFlash(QString mountPath);

    void UnmountConfigUsbFlash(QString mountPath);

    bool IsConfigUsbFlash();

    bool IsMounteDirExist(QString mountPath);

    QDir usbMountedDir() const { return m_usbMountedDir; }

private:
    QDir m_usbFlashDir;
    QDir m_usbMountedDir;
};

#endif // USBCONFIGMOUNTER_H
