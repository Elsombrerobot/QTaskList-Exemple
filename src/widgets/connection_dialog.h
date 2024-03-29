#ifndef CONNECTION_DIALOG_H
#define CONNECTION_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget.h>

#include "../utils/network_utils.h"
#include "loading_button.h"

class ConnectionDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ConnectionDialog(QWidget* parent = nullptr);

private slots:
	void m_AttemptApiConnection();
	void m_HandleApiConnectionError(QString response);
	void m_AttemptAuth();
	void m_HandleAuthError(QString response);
	void m_HandleAuthSuccess(QJsonObject userData);

private:
	QLineEdit* m_kitsuApiUrlLineEdit;
	QLineEdit* m_mailLineEdit;
	QLineEdit* m_passwordLineEdit;
	LoadingButton* m_connectButton;
};

#endif // CONNECTION_DIALOG_H
