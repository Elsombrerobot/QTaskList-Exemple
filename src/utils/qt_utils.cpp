#include <QMap>
#include <QApplication>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QIcon>
#include <QSettings>

#include "../utils/qt_utils.h"

namespace QtUtils
{
    // Create a QApplication with all needed configuration.
    QApplication* CreateQApp(int& argc, char** argv)
    {
        QApplication* app = new QApplication(argc, argv);

        // Load the icon from the genetared build_windows/qrc_icons.cpp resource file
        QIcon icon(":/app_icon");

        // Set application properties
        app->setWindowIcon(icon);
        app->setOrganizationName("Elsombrerobot");
        app->setApplicationVersion(QString(VERSION_STRING));
        app->setStyle("fusion");
        app->setApplicationName(APPLICATION_NAME);
        return app;
    }

    // Write a session persistent setting that can be retrived with Setting::Read
    void Settings::Write(const QString& group, QVariant value)
    {
        QSettings settings = QSettings(QSettings::UserScope, qApp);
        settings.setValue(group, value);
    }

    // Read a session persistent setting written with Setting::Write
    QVariant Settings::Read(const QString& group, QVariant defaultValue)
    {
        QSettings settings = QSettings(QSettings::UserScope, qApp);
        return settings.value(group, defaultValue);
    }

    // Get the window name with app name, verison, and current user if current user.
    QString GetWindowName()
    {
        // Create version string
        QString version = QString::number(Q_TASKLIST_EXEMPLE_VERSION_MAJOR) + "." +
                          QString::number(Q_TASKLIST_EXEMPLE_VERSION_MINOR) + "." +
                          QString::number(Q_TASKLIST_EXEMPLE_VERSION_PATCH);

        // Create base window name.
        QString windowName = QString(APPLICATION_NAME) + " - " + version;

        // Add username if user is connected.
        if (CurrentUser::Get().connected)
        {
            windowName += " - " + CurrentUser::FullName();
        }
        return windowName;
    };

    // Current user singleton for app.
    CurrentUser::CurrentUser() : connected(false) {}

    // Create and get the instance of the user.
    CurrentUser& CurrentUser::Get()
    {
        static CurrentUser s_Instance;
        return s_Instance;
    }

    // Set a user.
    void CurrentUser::Set(QJsonObject& data)
    {
        Get().connected = true;
        Get().IData() = data;
    }

    // Get data for the current user.
    QJsonObject& CurrentUser::Data()
    {
        return Get().IData();
    }

    // Get the kitsu fullname of the current user.
    QString CurrentUser::FullName()
    {
        return Data()["user"].toObject()["full_name"].toString();
    }

    // Get the kitsu Id of the current user.
    QString CurrentUser::Id()
    { 
        return Data()["user"].toObject()["id"].toString();
    }

    // Get the kitsu Mail of the current user.
    QString CurrentUser::Mail()
    {
        return Data()["user"].toObject()["email"].toString();
    }

    // Get the kitsu access token of the current user for this session.
    QString CurrentUser::AccessToken()
    {
        return Data()["access_token"].toString();
    }

    // Internal data getter.
    QJsonObject& CurrentUser::IData()
    {
        return m_Data;
    }
}