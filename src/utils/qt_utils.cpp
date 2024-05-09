#include <QMap>
#include <QApplication>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QIcon>
#include <QDir>
#include <QLocale>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QString>
#include <QTranslator>
#include <QCoreApplication>

#include "../utils/qt_utils.h"
#include "../../src/version.h"

namespace QtUtils
{
    // Define application name.
    #define APPLICATION_NAME "QTaskList-Exemple"

    // Define version string
    #define VERSION_STRING  "v" + \
        QString::number(Q_TASKLIST_EXEMPLE_VERSION_MAJOR) + "." + \
        QString::number(Q_TASKLIST_EXEMPLE_VERSION_MINOR) + "." + \
        QString::number(Q_TASKLIST_EXEMPLE_VERSION_PATCH)
   
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

        // Set user setting langage if not set, and load translation
        QString langage = QtUtils::Settings::Read(QtUtils::Settings::Keys::AppLangage).toString();
        if (langage.isEmpty())
        {
            QtUtils::Settings::Write(QtUtils::Settings::Keys::AppLangage, QVariant("en_UK"));
        }
        QtUtils::SetLanguage(QtUtils::Settings::Read(QtUtils::Settings::Keys::AppLangage).toString());

        return app;
    }

    // Get the window name with app name, verison, and current user if current user.
    QString GetWindowName()
    {
        // Create base window name.
        QString windowName = QString(APPLICATION_NAME) + " - " + QString(VERSION_STRING);

        // Add username if user is connected.
        if (CurrentUser::Get().connected)
        {
            windowName += " - " + CurrentUser::FullName();
        }
        return windowName;
    };

    // Change the language of the application. Not found qm files for langId reset the langage to default en_UK tr() values.
    void SetLanguage(const QString& langId)
    {
        // Translator for the app.
        QTranslator* translator = new QTranslator(qApp);

        // Construct the filepath based on langId, .qm will be a relative file in ../translations wheter we run in debug mode, or we are installing.
        QString filePath = QString("%1/../translations/%2.qm")
            .arg(QCoreApplication::applicationDirPath())
            .arg(langId);

        // Load translation
        Q_UNUSED(translator->load(filePath));
        qApp->installTranslator(translator);
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