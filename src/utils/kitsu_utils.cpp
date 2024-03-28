#include <QString>
#include <QObject>
#include <QNetworkReply>

#include "kitsu_utils.h"
#include "qt_utils.h"
#include "network_utils.h"

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

    // Route getter
    QString Api::GetRoute(QString route)
    {
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
        QJsonObject replyData = NetworkUtils::ReadReplyData(reply);
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
        QJsonObject replyData = NetworkUtils::ReadReplyData(reply);
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


}
