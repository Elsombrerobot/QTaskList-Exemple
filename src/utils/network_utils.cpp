#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <QUrl>
#include <QObject>
#include <QUrlQuery>
#include <QString>
#include <QApplication>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "qt_utils.h"

// Singleton for app network access manager. Doc says one should be enough for an entire app.
class AppNetworkManager
{
public:

	AppNetworkManager(const AppNetworkManager&) = delete;

	static AppNetworkManager& Get()
	{
		static AppNetworkManager s_Instance;
		return s_Instance;
	}

	static QNetworkAccessManager& Manager() { return Get().IManager(); }

private:

	QNetworkAccessManager m_Manager;
	AppNetworkManager() {}
	QNetworkAccessManager& IManager() { return m_Manager; }

};

// QtNetwork related utils functions, get, posts...
namespace NetworkUtils
{
	// Create a QNetworkRequest with given url, headers and url data.
	QNetworkRequest CreateRequest(
		const QString& url,
		const QtUtils::QStrMap& headers = {},
		const QtUtils::QStrMap& urldata = {})
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
		request.setTransferTimeout(5000);

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
		const QtUtils::QStrMap& headers = {},
		const QtUtils::QStrMap& urldata = {})
	{
		QNetworkRequest request = CreateRequest(url, headers, urldata);
		QNetworkReply* reply = AppNetworkManager::Manager().get(request);
		return reply;
	}

	// Perform a post onto a given url and return the adress of the reply object.
	QNetworkReply* Post(
		const QString& url,
		const QtUtils::QStrMap& headers = {},
		const QtUtils::QStrMap& urldata = {})
	{
		QNetworkRequest request = CreateRequest(url, headers, urldata);
		QNetworkReply* reply = AppNetworkManager::Manager().post(request, QByteArray{});
		return reply;
	}

	// Make sure an adress has a trailing slash, useful for url manipulation.
	QUrl EnsureTrailingSlash(QUrl url)
	{
		if (!url.path().endsWith("/"))
			url.setPath(url.path() + "/");
		return url;
	}

}

#endif // NETWORK_UTILS_H