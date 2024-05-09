#include <QMenu>
#include <QRadioButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QWidgetAction>
#include <QIcon>
#include <QMessageBox>

#include "../utils/qt_utils.h"
#include "menu_bar.h"

// A radio button for selected a language, exemple french.
LanguageRadioButton::LanguageRadioButton(const QString& id, const QString& name, QWidget* parent) 
	: QRadioButton(parent), id(id), name(name)
{
	// The id should have the same name as the alias of the flag icon.
	setIcon(QIcon(":/" + id));
	setText(name);

	// Connect toggled to language change.
	connect(this, &QRadioButton::toggled, this, [this]()
		{
			if (this->isChecked())
			{
				QtUtils::Settings::Write(QtUtils::Settings::Keys::AppLangage, QVariant(this->id));
				QMessageBox::information(this, QObject::tr("Langage updated."), QObject::tr("Langage will be updated on next restart."));
			}
		});
}

// The widget responsible for changeing language.
LanguageSelector::LanguageSelector(QWidget* parent)
	: QWidget(parent)
{
	m_layout = new QVBoxLayout(this);

	// Map for supported languages.
	m_languageList = { {tr("French"), "fr_FR"}, {tr("English"), "en_UK"} };

	// Get settings langage
	QString langage = QtUtils::Settings::Read(QtUtils::Settings::Keys::AppLangage).toString();

	// Add options into the widget.
	for (const auto [k, v] : m_languageList.asKeyValueRange())
	{
		LanguageRadioButton* tempButton = new LanguageRadioButton(v, k, this);

		// Set checked if is setting langage.
		if (v == langage)
		{
			tempButton->blockSignals(true);
			tempButton->setChecked(true);
			tempButton->blockSignals(false);
		}

		m_layout->addWidget(tempButton);
	}
}

// The widget responsoble for changeing language.
MenuBar::MenuBar(QWidget* parent)
	: QMenuBar(parent)
{
	m_menu = new QMenu(tr("&Menu"), this);
	addMenu(m_menu);
	m_languageMenu = new QMenu(tr("&Language..."), this);
	m_menu->addMenu(m_languageMenu);
	m_languageSelector = new LanguageSelector(this);
	m_selectorContainer = new QWidgetAction(this);
	m_selectorContainer->setDefaultWidget(m_languageSelector);
	m_languageMenu->addAction(m_selectorContainer);
}