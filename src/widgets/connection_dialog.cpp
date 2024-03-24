
#include <QVBoxLayout>
#include <QMessageBox>
#include <QObject>
#include <QApplication>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "connection_dialog.h"
#include "../utils/qt_utils.h"
#include "../utils/network_utils.h"

ConnectionDialog::ConnectionDialog(QWidget* parent) : QDialog(parent)
{
	// Widget setup
	setWindowTitle("Connect to kitsu instance.");

	QVBoxLayout* layout = new QVBoxLayout(this);

	m_kitsuApiUrlLineEdit = new QLineEdit(this);
	m_kitsuApiUrlLineEdit->setPlaceholderText("https://kitsu.my_studio/api/");
	layout->addWidget(m_kitsuApiUrlLineEdit);

	m_mailLineEdit = new QLineEdit(this);
	m_mailLineEdit->setPlaceholderText("name@company.com");
	layout->addWidget(m_mailLineEdit);

	m_passwordLineEdit = new QLineEdit(this);
	m_passwordLineEdit->setPlaceholderText("Password");
	m_passwordLineEdit->setEchoMode(QLineEdit::Password);
	layout->addWidget(m_passwordLineEdit);

	m_connectButton = new QPushButton("Connect", this);
	layout->addWidget(m_connectButton);

	setMinimumWidth(400);

	// Connections
	connect(m_connectButton, &QPushButton::clicked, this, &ConnectionDialog::m_AttemptApiConnection);
	connect(this, &ConnectionDialog::accepted, qApp, &QApplication::quit); 

}

// Attempt to connect to the kitsu api with the current password and login. Called if m_HandleApiReponse is a success.
void ConnectionDialog::m_AttemptAuth()
{
	m_SetInputsDisabled(true);

	// Create auth url.
	QUrl baseUrl = NetworkUtils::EnsureTrailingSlash(QUrl(m_kitsuApiUrlLineEdit->text()));
	QUrl authRoute = QUrl(baseUrl).resolved(QUrl("auth/login"));

	// Setup request.
	QtUtils::QStrMap urlData;
	urlData["email"] = m_mailLineEdit->text();
	urlData["password"] = m_passwordLineEdit->text();
	QNetworkReply* reply = NetworkUtils::Post(authRoute.toString(), {}, urlData);

	// Connect request to auth response handler.
	connect(reply, SIGNAL(finished()), this, SLOT(m_HandleAuthResponse()));
}

// Handle the response from /api/auth/login post request.
void ConnectionDialog::m_HandleAuthResponse()
{
	// Extract reply from sender
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	// Remove query parameters from the URL
	//Passing a value of QString() to query (a null QString) unsets the query completely. However, passing a value of QString("") will set the query to an empty value, as if the original URL had a lone "?".
	QUrl baseUrl = reply->url();
	baseUrl.setQuery(QString());

	// Check for network errors
	if (reply->error() != QNetworkReply::NoError)
	{
		QMessageBox::warning(this, "Error " + baseUrl.toString(), "Connection failed, verify login and password.");
		m_clearCredentialFields();
		m_SetInputsDisabled(false);
		return;
	}

	// Read the response data
	QByteArray responseData = reply->readAll();
	QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

	// Check if the response is a valid JSON object
	if (!jsonResponse.isObject())
	{
		QMessageBox::warning(this, "Error", "Invalid JSON response from server: " + baseUrl.toString());
		m_clearCredentialFields();
		m_SetInputsDisabled(false);
		return;
	}

	// Extract the API key from the JSON response
	QJsonObject jsonObject = jsonResponse.object();
	QString keyToCheck = "login";
	QJsonValue apiValue = jsonObject.value(keyToCheck);

	// Check if the API key exists and has a true value
	if (!apiValue.isBool() || !apiValue.toBool())
	{
		QMessageBox::warning(this, "Error", "Invalid login response from server: " + baseUrl.toString());
		m_clearCredentialFields();
		m_SetInputsDisabled(false);
		return;
	}

	// Connection successful
	QMessageBox::information(this, "Success", "Connection successful.");
	accept();
}

// Attempt get the kitsu /api endpoint {"api" : "Zou"} to ensure instance is up, and the adress is actually a kitsu api.
void ConnectionDialog::m_AttemptApiConnection()
{
	m_SetInputsDisabled(true);

	// Setup request.
	QString kitsuApiUrl = m_kitsuApiUrlLineEdit->text();
	QNetworkReply* reply = NetworkUtils::Get(kitsuApiUrl);

	// Connect request to api response handler.
	connect(reply, SIGNAL(finished()), this, SLOT(m_HandleApiReponse()));
}

// Handle the response from /api/ get request, if everything works, calls m_AttemptAuth.
void ConnectionDialog::m_HandleApiReponse()
{
	// Extract reply from sender
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	QString apiUrl = m_kitsuApiUrlLineEdit->text();

	// Check for network errors
	if (reply->error() != QNetworkReply::NoError)
	{
		QMessageBox::warning(this, "Error", "Can't connect to " + apiUrl + ": " + reply->errorString());
		m_kitsuApiUrlLineEdit->clear();
		return;
	}

	// Read the response data
	QByteArray responseData = reply->readAll();
	QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);

	// Check if the response is a valid JSON object
	if (!jsonResponse.isObject())
	{
		QMessageBox::warning(this, "Error", "Invalid JSON response from " + apiUrl);
		m_kitsuApiUrlLineEdit->clear();
		return;
	}

	// Extract the "api" value from the JSON response
	QString keyToCheck = "api";
	QJsonObject jsonObject = jsonResponse.object();
	QJsonValue apiValue = jsonObject.value(keyToCheck);

	// Check if the "api" value matches the expected value
	if (apiValue.toString() != "Zou")
	{
		QMessageBox::warning(this, "Error", "URL is not a Kitsu API instance: " + apiUrl);
		m_kitsuApiUrlLineEdit->clear();
		return;
	}

	// Attempt authentication
	m_AttemptAuth();
}

// Disable or enable all field inputs, used while waiting for api responses.
void ConnectionDialog::m_SetInputsDisabled(bool state)
{
	m_kitsuApiUrlLineEdit->setDisabled(state);
	m_mailLineEdit->setDisabled(state);
	m_passwordLineEdit->setDisabled(state);
	m_connectButton->setDisabled(state);
}

// Clear the credentials inputs, used when login failed.
void ConnectionDialog::m_clearCredentialFields()
{
	m_mailLineEdit->clear();
	m_passwordLineEdit->clear();
}

