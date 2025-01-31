#pragma once

#include <QMenu>
#include <QSystemTrayIcon>

class ClientMonitor;

void SetupSystemTray( QSystemTrayIcon &aSystemTray, std::unique_ptr<ClientMonitor> &aClientMonitor );
