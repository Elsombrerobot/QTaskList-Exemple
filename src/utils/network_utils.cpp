#include <QUrl>
#include <QObject>
#include <QUrlQuery>
#include <QString>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "qt_utils.h"
#include "network_utils.h"



// QtNetwork related utils functions, get, posts...
namespace NetworkUtils
{

	// Singleton for app network access manager. Doc says one should be enough for an entire app.
	AppNetworkManager::AppNetworkManager()
	{
		m_Manager = new QNetworkAccessManager(this);
	};

	AppNetworkManager* AppNetworkManager::Get()
	{
		static AppNetworkManager* s_Instance = new AppNetworkManager();
		return s_Instance;
	}

	QNetworkAccessManager* AppNetworkManager::Manager() { return Get()->m_Manager; };

	// Create a QNetworkRequest with given url, headers and url data.
	QNetworkRequest CreateRequest(
		const QString& url,
		const QtUtils::QStrMap& headers,
		const QtUtils::QStrMap& urldata,
		quint16 msTimeout
		)
	{
		// Create QUrl 
		QUrl qurl{ url };
		QUrlQuery qqurl;

		// Add data to the url
		for (const auto [k, v] : urldata.asKeyValueRange())
		{
			qqurl.addQueryItem(k, v);
		}
		// Set the content type.
		qurl.setQuery(qqurl);

		// Create a request
		QNetworkRequest request{ QUrl(qurl) };
		request.setTransferTimeout(msTimeout);

		// Add headers to the request
		for (const auto [k, v] : headers.asKeyValueRange())
		{
			request.setRawHeader(k.toUtf8(), v.toUtf8());
		}
		request.setRawHeader("content-type", "application/x-www-form-urlencoded");

		return request;
	}

	// Perform a get onto a given url and return the adress of the reply object.
	QNetworkReply* Get(
		const QString& url,
		const QtUtils::QStrMap& headers,
		const QtUtils::QStrMap& urldata,
		quint16 msTimeout
		)
	{

		QNetworkRequest request = CreateRequest(url, headers, urldata, msTimeout);
		QNetworkReply* reply = AppNetworkManager::Manager()->get(request);
		return reply;
	}

	// Perform a post onto a given url and return the adress of the reply object.
	QNetworkReply* Post(
		const QString& url,
		const QtUtils::QStrMap& headers,
		const QtUtils::QStrMap& urldata,
		quint16 msTimeout 
		)
	{
		QNetworkRequest request = CreateRequest(url, headers, urldata, msTimeout);
		QNetworkReply* reply = AppNetworkManager::Manager()->post(request, QByteArray{});
		return reply;
	}

	// Make sure an adress has a trailing slash, useful for url manipulation.
	QString EnsureTrailingSlash(QString url)
	{
		if (!url.endsWith("/"))
			url += "/";
		return url;
	}

	// Read the data from a network reply as json, Return empty object if parsing fails.
	QJsonObject ReadJsonReply(QNetworkReply* reply)
	{
		// Read the response data
		QByteArray responseData = reply->readAll();
		QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
		QJsonParseError jsonError;
		if (jsonError.error != QJsonParseError::NoError)
		{
			return QJsonObject();
		}
		return jsonResponse.object();
	}

	// Read the data from a network reply as array, Return empty object if parsing fails.
	QJsonArray ReadArrayReply(QNetworkReply* reply)
	{
		// Read the response data
		QByteArray responseData = reply->readAll();
		QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
		QJsonParseError jsonError;
		if (jsonError.error != QJsonParseError::NoError)
		{
			return QJsonArray();
		}
		return jsonResponse.array();
	}

}
