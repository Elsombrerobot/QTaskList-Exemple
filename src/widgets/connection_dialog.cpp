#include <QVBoxLayout>
#include <QMessageBox>
#include <QObject>
#include <QApplication>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QUrl>
#include <QString>
#include <QSizePolicy>

#include "connection_dialog.h"
#include "loading_button.h"
#include "../utils/qt_utils.h"
#include "../utils/network_utils.h"
#include "../utils/kitsu_utils.h"

// Widget to get kitsu api url and logs from user, and try connection.
ConnectionDialog::ConnectionDialog(QWidget* parent) : QDialog(parent)
{
	// Widget setup
	setWindowTitle("Connect to kitsu instance.");

	m_layout = new QVBoxLayout(this);

	m_kitsuApiUrlLineEdit = new QLineEdit(this);
	m_kitsuApiUrlLineEdit->setPlaceholderText("https://kitsu.my_studio.com");
	m_kitsuApiUrlLineEdit->setClearButtonEnabled(true);
	m_layout->addWidget(m_kitsuApiUrlLineEdit);

	m_mailLineEdit = new QLineEdit(this);
	m_mailLineEdit->setPlaceholderText("name@company.com");
	m_mailLineEdit->setClearButtonEnabled(true);
	m_layout->addWidget(m_mailLineEdit);

	m_passwordLineEdit = new QLineEdit(this);
	m_passwordLineEdit->setPlaceholderText("Password");
	m_passwordLineEdit->setEchoMode(QLineEdit::Password);
	m_passwordLineEdit->setClearButtonEnabled(true);
	m_layout->addWidget(m_passwordLineEdit);

	m_connectButton = new LoadingButton("Connect", this);
	//m_connectButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	m_layout->addWidget(m_connectButton);

	setMinimumWidth(400);

	// TO REMOVE
	m_kitsuApiUrlLineEdit->setText("http://172.19.80.178");
	m_mailLineEdit->setText("iammarcferrer@gmail.com");
	m_passwordLineEdit->setText("password");

	// Connections

	// Attempt connection on click.
	connect(
		m_connectButton,
		SIGNAL(clicked()), 
		this, SLOT(m_AttemptApiConnection()));
	
	// Connect api error connection error to api connection error handler.
	connect(
		&KitsuUtils::Api::Get(),
		SIGNAL(ValidateApiError(QString)),
		this,
		SLOT(m_HandleApiConnectionError(QString)));

	// If api validates, try to connect with the password and mail on api auth route.
	connect(
		&KitsuUtils::Api::Get(),
		SIGNAL(ValidateApiSuccess()),
		this,
		SLOT(m_AttemptAuth()));

	// Connect auth error to auth error handler.
	connect(
		&KitsuUtils::Api::Get(),
		SIGNAL(AuthError(QString)),
		this,
		SLOT(m_HandleAuthError(QString)));

	// Connect auth success to auth success handler.
	connect(
		&KitsuUtils::Api::Get(),
		SIGNAL(AuthSuccess(QJsonObject)),
		this,
		SLOT(m_HandleAuthSuccess(QJsonObject)));
}

// Test the api connection
void ConnectionDialog::m_AttemptApiConnection()
{
	setDisabled(true);
	m_connectButton->StartLoading();

	//Set api
	QString urlFixed = NetworkUtils::EnsureTrailingSlash(m_kitsuApiUrlLineEdit->text());
	KitsuUtils::Api::SetUrl(urlFixed);
	KitsuUtils::Api::Validate();
}

// If api connection failed show dialog and retry
void ConnectionDialog::m_HandleApiConnectionError(QString response)
{
	m_connectButton->StopLoading();
	QMessageBox::warning(this, "Error", response);
	setDisabled(false);
}

// Attempt auth, not in a lamba for readability + we don't want the two signals of Api to be connected together.
void ConnectionDialog::m_AttemptAuth()
{
	setDisabled(true);
	m_connectButton->StartLoading();
	KitsuUtils::Api::Auth(m_mailLineEdit->text(), m_passwordLineEdit->text());
}

// If failed show dialog and retry
void ConnectionDialog::m_HandleAuthError(QString response)
{
	m_connectButton->StopLoading();
	QMessageBox::warning(this, "Error", response);
	m_passwordLineEdit->clear();
	setDisabled(false);
}

// On auth success, store user data, and accept dialog, that will trigger the main window.
void ConnectionDialog::m_HandleAuthSuccess(QJsonObject userData)
{
	// Set user for application
	QtUtils::CurrentUser::Set(userData);

	setDisabled(false);
	m_connectButton->StopLoading();
	accept();
}
