#ifndef TASK_MENU_H
#define TASK_MENU_H

#include <QMenu>

#include "task_utils.h"

// Custom context menu class
class TaskContextMenu : public QMenu
{
    Q_OBJECT

public:
    TaskContextMenu(const TaskUtils::Task& onTask,                      // Task zhere the context menu was requested.
                          TaskUtils::TaskConstRefList selectedTasks,    // All the task selected.
                          QWidget* parent = nullptr);

signals:
    void OpenTaskTriggered(const TaskUtils::Task&);
};

#endif // TASK_MENU_H