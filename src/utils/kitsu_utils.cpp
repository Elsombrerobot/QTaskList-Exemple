#include <QString>
#include <QObject>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "kitsu_utils.h"
#include "qt_utils.h"
#include "network_utils.h"

// Function and utils related to kitsu, api, tasks..
namespace KitsuUtils
{
    Api::Api(QObject* parent) : QObject(parent) {}

    // Initialize static member
    Api& Api::Get()
    {
        static Api s_Instance;
        return s_Instance;
    }

    // Base url Getter
    QString Api::BaseUrl()
    {
        return Get().m_BaseUrl;
    }

    // Url setter
    void Api::SetUrl(QString url)
    {
        Get().m_BaseUrl = url;
    }

    // Route getter. Formats "{key}" in route wih given value {"key" : "value"} for key in formatData.
    // Routes for route param are defined in header, ex: Api::Route::Auth
    QString Api::GetRoute(QString route, const QtUtils::QStrMap& formatData)
    {
        // Add data to the url
        for (const auto [k, v] : formatData.asKeyValueRange())
        {
            route.replace('{' + k + '}', v);
        }
        QUrl url = QUrl(Api::BaseUrl());
        url.setPath(route);
        return url.toString();
    }

    // Starts the verification of the kitsu api url.
    void Api::Validate() 
    {
	    // Send get requests to the endpoint.
	    QNetworkReply* reply = NetworkUtils::Get(Api::GetRoute(Routes::Api));

	    // Connect request to api response handler.
	    connect(reply, SIGNAL(finished()), &Get(), SLOT(m_HandleValidateResponse()));
    }

    // Check the data received from the url, to make sure that it is a Kitsu instance and access is granted.
    void Api::m_HandleValidateResponse()
    {
        //Extract reply from sender
	    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

        // Check for network errors
	    if (reply->error() != QNetworkReply::NoError)
	    {
            QString message = "Can't connect to " + Api::BaseUrl() + " : " + reply->errorString();
            emit ValidateApiError(message);
            return;

	    }

        // Validate reply json response
        QJsonObject replyData = NetworkUtils::ReadJsonReply(reply);
        if (replyData.isEmpty())
        {
            QString message = "Failed to read server response.";
            emit ValidateApiError(message);
            return;

        }

	    // Extract the "api" value from the JSON response
        QString keyToCheck = "api";
        QString expectedApiValue = "Zou";
	    QJsonValue apiValue = replyData.value(keyToCheck);

	    // Check if the "api" value matches the expected "Zou" value
	    if (apiValue.toString() != expectedApiValue)
	    {
            QString message = "URL is not a Kitsu API instance: " + reply->url().toString();
            emit ValidateApiError(message);
		    return;
	    }

        // Success
        emit ValidateApiSuccess();

        // Clean reply
        reply->deleteLater();
    }

    // Start the user credentials verifications.
    void Api::Auth(QString email, QString password)
    {
        // Setup request.
        QtUtils::QStrMap urlData;
        urlData["email"] = email;
        urlData["password"] = password;

        // Send post request to the endpoint.
        QNetworkReply* reply = NetworkUtils::Post(Api::GetRoute(Routes::Auth), {}, urlData);

        // Connect request to api response handler.
        connect(reply, SIGNAL(finished()), &Get(), SLOT(m_HandleAuthResponse()));
    }
       
    void Api::m_HandleAuthResponse()
    {
        // Extract reply from sender
        QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

        // Remove query parameters from the URL.
        //Passing a value of QString() to query (a null QString) unsets the query completely. However, passing a value of QString("") will set the query to an empty value, as if the original URL had a lone "?".
        QUrl baseUrl = reply->url();
        baseUrl.setQuery(QString());

        // Check for network errors
        if (reply->error() != QNetworkReply::NoError)
        {
            QString message = "Connection to " + baseUrl.toString() + " failed, verify login and password.";
            emit AuthError(message);
            return;
        }

        // Validate reply json response
        QJsonObject replyData = NetworkUtils::ReadJsonReply(reply);
        if (replyData.isEmpty())
        {
            QString message = "Failed to read server response.";
            emit AuthError(message);
            return;
        }

        // Extract the "api" value from the JSON response
        QString keyToCheck = "login";
        bool expectedApiValue = true;
        QJsonValue apiValue = replyData.value(keyToCheck);

        // Check if the "api" value matches the expected "Zou" value
        if (!apiValue.isBool() || apiValue.toBool() != expectedApiValue)
        {
            QString message = "Connection to " + baseUrl.toString() + " failed, unknown reason.";
            emit AuthError(message);
            return;
        }

        // Clean reply
        reply->deleteLater();

        // Connection successful, emit user data
        emit AuthSuccess(replyData);
    }

    // Start the user credentials verifications.
    void Api::GetTasks(bool done)
    {
        // Create route with current user id
        QtUtils::QStrMap formatData;
        formatData["person_id"] = QtUtils::CurrentUser::Id();

        // Create header cookie with current user token (can't work this out with QNetworkCookie and QNetworkCookieJar)
        QtUtils::QStrMap headers;
        headers["Authorization"] = QByteArray("Bearer ") + QtUtils::CurrentUser::AccessToken().toUtf8();

        // Choose done task route or regular task task route. 
        QString route = done ? Routes::UserDoneTasks : Routes::UserTasks;

        // Format route with user id
        QString url = Api::GetRoute(route, formatData);

        // Send post request to the endpoint with formated url and header. Also give 30 seconds timeout.
        QNetworkReply* reply = NetworkUtils::Get(url, headers, {}, NetworkUtils::Timeout::MsLong);

        // Connect request to get tasks response handler.
        connect(reply, SIGNAL(finished()), &Get(), SLOT(m_HandleGetTasksResponse()));
    }

    // Handle success or error for GetTasks
    void Api::m_HandleGetTasksResponse()
    {
        // Extract reply from sender
        QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

        // Check for network errors
        if (reply->error() != QNetworkReply::NoError)
        {
            QString message = "Failed to fetch tasks : " + reply->errorString();
            emit GetTasksError(message);
            return;
        }

        // Validate reply json response
        QJsonArray replyData = NetworkUtils::ReadArrayReply(reply);
        if (replyData.isEmpty())
        {
            QString message = "Failed to read server response.";
            emit GetTasksError(message);
            return;
        }

        // Clean reply
        reply->deleteLater();

        // Get tasks successful, emit tasks data
        emit GetTasksSuccess(replyData);
    }
}
