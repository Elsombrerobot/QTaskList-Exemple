#ifndef TASK_BROSWER_H
#define TASK_BROSWER_H

#include <QObject>
#include <QJsonArray>
#include <QWidget>
#include <QSplitter>
#include <QTableView>
#include <QContextMenuEvent>

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
    QVBoxLayout* m_tableLayout;
    QLabel* m_taskResumeLabel;
    QWidget* m_tableWidget;
    TaskUtils::TaskList m_taskList;

signals:
     void TaskListReady(TaskUtils::TaskList*);

private slots:
    void m_HandlTaskMenuRequest(const TaskUtils::Task& onTask, TaskUtils::TaskConstRefList selectedTasks, QContextMenuEvent* event);
    void m_HandleAvailableTasks(QJsonArray tasksData);
    void m_UpdateResumeLabel();
};

#endif // TASK_BROSWER_H
