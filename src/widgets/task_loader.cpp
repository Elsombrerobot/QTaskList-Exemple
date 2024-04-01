#include <QWidget>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>

#include "../utils/kitsu_utils.h"
#include "task_loader.h"
#include "loading_button.h"

// Gui widget to query user tasks
TaskLoader::TaskLoader(QWidget* parent) : QWidget(parent)
{
    // Create button and checkbox
    m_reloadButton = new LoadingButton("Reload", this);
    m_reloadButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_doneCheckBox = new QCheckBox("Done", this);

    // Create layout
    m_layout = new QHBoxLayout(this);
    m_layout->addWidget(m_reloadButton);
    m_layout->addWidget(m_doneCheckBox);

    // Connect button to load task function.
    connect(
        m_reloadButton, SIGNAL(clicked()),
        this, SLOT(m_LoadTasks()));

    // Connect the the get task error to the handler.
    connect(
        &KitsuUtils::Api::Get(),
        SIGNAL(GetTasksError(QString)),
        this,
        SLOT(m_HandleLoadTasksError(QString)));

    // Connect the the get task success to the handler.
    connect(
        &KitsuUtils::Api::Get(),
        SIGNAL(GetTasksSuccess(QJsonArray)),
        this,
        SLOT(m_HandleLoadTasksSuccess(QJsonArray)));
}

// Make the qury to get user tasks
void TaskLoader::m_LoadTasks()
{
    setDisabled(true);
    m_reloadButton->StartLoading();
    emit ReloadStarted();
    KitsuUtils::Api::GetTasks(m_doneCheckBox->isChecked());
}

// Handles errors of get tasks
void TaskLoader::m_HandleLoadTasksError(QString response)
{
    m_reloadButton->StopLoading();
    emit ReloadFinished();
    emit TasksAvailable(QJsonArray());
    QMessageBox::warning(this, "Error", response);
    setDisabled(false);
}

// Handles success of get tasks
void TaskLoader::m_HandleLoadTasksSuccess(QJsonArray tasksData)
{
    m_reloadButton->StopLoading();
    emit ReloadFinished();
    emit TasksAvailable(tasksData);
    setDisabled(false);
}
