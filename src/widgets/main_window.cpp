#include <QMainWindow>
#include <QStatusBar>
#include <QJsonDocument>
#include <QString>
#include <QLabel>
#include <QJsonArray>
#include <QSplitter>
#include <QSizePolicy>

#include "main_window.h"
#include "task_browser.h"
#include "menu_bar.h"
#include "../utils/qt_utils.h"

// A Simple main window to contain the main widget TaskBrowser.
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	// Set window title
	setWindowTitle(QtUtils::GetWindowName());

	//Resize
	resize(1200, 700);

	// Create TaskBrowser instance.
	m_taskBrowser = new TaskBrowser(this);

	// Widget setup
	m_statusBar = new QStatusBar(this);
	m_menuBar = new MenuBar(this);
	setStatusBar(m_statusBar);
	setMenuBar(m_menuBar);
	setCentralWidget(m_taskBrowser);

	// Show connection message
	m_statusBar->showMessage(QString(tr("Connection as %1 successfull!")).arg(QtUtils::CurrentUser::FullName()), 4000);
}