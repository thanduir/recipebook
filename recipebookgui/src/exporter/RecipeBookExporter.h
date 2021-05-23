#ifndef RECIPEBOOK_EXPORTER_H
#define RECIPEBOOK_EXPORTER_H

#include <QObject>

class UIStringConverter;

namespace recipebook
{
	class RBDataHandler;
	class RecipeBookSettings;
	class RBDialogInterface;

	class RecipeBookExporter : public QObject
	{
		Q_OBJECT

	public:
		RecipeBookExporter(RBDataHandler& rRBDataHandler,
						   RecipeBookSettings& rSettings, 
						   const UIStringConverter& rConverter,
						   const RBDialogInterface& rDlgInterface);

	public slots:
		QStringList getDlgNameFilters() const;
		void exportRecipeBook(QString strFileURL, quint32 uiConfiguration);

	private:
		RBDataHandler&				m_rRBDataHandler;
		RecipeBookSettings&			m_rSettings;
		const UIStringConverter&	m_rConverter;
		const RBDialogInterface&	m_rDlgInterface;
	};
}

#endif
