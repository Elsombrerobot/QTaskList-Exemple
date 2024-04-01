#ifndef TASK_BROSWER_H
#define TASK_BROSWER_H

#include <QObject>
#include <QJsonArray>
#include <QWidget>
#include <QSplitter>

#include "task_loader.h"
#include "task_list.h"
#include "task_filter.h"

class TaskBrowser : public QWidget {
    Q_OBJECT

public:
    explicit TaskBrowser(QJsonArray& taskStack, QWidget* parent = nullptr);

private:
    QJsonArray& m_TaskStack;
    QSplitter* m_leftSplitter;
    QSplitter* m_mainSplitter;
    QHBoxLayout* m_layout;
    TaskLoader* m_TaskLoader;

};

#endif // TASK_BROSWER_H
