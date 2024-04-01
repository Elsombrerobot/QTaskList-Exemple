#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QStatusbar>
#include <QJsonArray>

#include "task_browser.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	QJsonArray m_TaskStack;
	QStatusBar* m_StatusBar;
	TaskBrowser* m_TaskBrowser;

};

#endif // MAIN_WINDOW_H
