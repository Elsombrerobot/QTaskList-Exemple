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
#include "../utils/qt_utils.h"

// A Simple main window to contain the main widget TaskBrowser.
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
	//Resize
	resize(1200, 700);

	// Create TaskBrowser instance.
	m_taskBrowser = new TaskBrowser(this);

	// Widget setup
	m_statusBar = new QStatusBar(this);
	setStatusBar(m_statusBar);
	setCentralWidget(m_taskBrowser);

	// Show message
	m_statusBar->showMessage("Connection as " + QtUtils::CurrentUser::FullName() + " successful !", 4000);
}