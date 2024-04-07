#ifndef TASK_BROSWER_H
#define TASK_BROSWER_H

#include <QObject>
#include <QJsonArray>
#include <QWidget>
#include <QSplitter>
#include <QTableView>

#include "task_loader.h"
#include "task_list.h"
#include "task_filter.h"
#include "task_utils.h"

class TaskBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit TaskBrowser(QWidget* parent = nullptr);

private:
    QSplitter* m_leftSplitter;
    QSplitter* m_mainSplitter;
    QHBoxLayout* m_layout;
    TaskLoader* m_TaskLoader;
    TaskFilter* m_taskFilter;
    TaskTable* m_taskTable;
    TaskTableModel* m_taskModel;
    TaskTableFilterProxy* m_filterProxy;
    TaskUtils::TaskList m_taskList;

signals:
     void TaskListReady(TaskUtils::TaskList*);

private slots:
    void m_HandleAvailableTasks(QJsonArray tasksData);
};

#endif // TASK_BROSWER_H
