#ifndef RECIPEBOOK_LISTMODEL_RECIPEBOOK_CONFIGURATIONS_H
#define RECIPEBOOK_LISTMODEL_RECIPEBOOK_CONFIGURATIONS_H

#include <QAbstractListModel>

namespace recipebook
{
	class RecipeBook;
	class RBDataHandler;
}

namespace recipebook::UI
{
	class ListModelRecipeBookConfigurations : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class RBConfigurationsRoles : int
		{
			NameRole = Qt::UserRole + 1,
			TitleRole,
			SubtitleRole,
			FontSizeRole
		};
    
	public:
		ListModelRecipeBookConfigurations(RBDataHandler& rRBDataHandler);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	public slots:
		QString name(int row) const;
		int renameConfiguration(int row, QString newName);

		QString title(int row) const;
		QString subtitle(int row) const;
		quint32 fontSize(int row) const;

		void setTitle(int row, QString strTitle);
		void setSubtitle(int row, QString strSubtitle);
		void setFontSize(int row, quint32 fontSize);

		bool canConfigurationBeAdded() const;
		int addConfiguration(QString strConfig);
		bool existsConfiguration(QString strConfig) const;

		bool removeConfiguration(int row);

		void onDataReset();
        
	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		void setDataChanged(int row, RBConfigurationsRoles role);

	private:
		RBDataHandler& m_rRBDataHandler;
	};
}

#endif
