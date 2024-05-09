#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStatusbar>
#include <QJsonArray>

#include "task_browser.h"
#include "menu_bar.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	QJsonArray m_TaskStack;
	QStatusBar* m_statusBar;
	TaskBrowser* m_taskBrowser;
	MenuBar* m_menuBar;
};

#endif // MAIN_WINDOW_H
