#ifndef MENU_BAR_H
#define MENU_BAR_H

#include <QMenu>
#include <QRadioButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QWidgetAction>

#include "../utils/qt_utils.h"

// --- LanguageRadioButton ---
class LanguageRadioButton : public QRadioButton
{
    Q_OBJECT

public:
    explicit LanguageRadioButton(const QString& id, const QString& name, QWidget* parent = nullptr);
    
    QString id;
    QString name;
};

// --- LanguageSelector ---
class LanguageSelector : public QWidget
{
    Q_OBJECT

public:
    explicit LanguageSelector(QWidget* parent = nullptr);

//private slots:
    //void m_HandleLanguageChanged();

private:
    QVBoxLayout* m_layout;
    QtUtils::QStrMap m_languageList;
};

// --- MenuBar ---
class MenuBar : public QMenuBar
{
    Q_OBJECT

public:
    explicit MenuBar(QWidget* parent = nullptr);

private:
    QMenu* m_menu;
    QMenu* m_languageMenu;
    LanguageSelector* m_languageSelector;
    QWidgetAction* m_selectorContainer;
};

#endif // MENU_BAR_H