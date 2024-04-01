#ifndef TASK_LOADER_H
#define TASK_LOADER_H

#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>

#include "loading_button.h"

class TaskLoader : public QWidget {
    Q_OBJECT
public:
    explicit TaskLoader(QWidget* parent = nullptr);

private:
    LoadingButton* m_reloadButton;
    QCheckBox* m_doneCheckBox;
    QHBoxLayout* m_layout;

signals:
    void ReloadFinished();
    void ReloadStarted();
    void TasksAvailable(QJsonArray);

private slots:
    void m_HandleLoadTasksError(QString response);
    void m_HandleLoadTasksSuccess(QJsonArray tasksData);
    void m_LoadTasks();
};


#endif // TASK_LOADER_H
