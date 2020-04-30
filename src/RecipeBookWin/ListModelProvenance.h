#ifndef RECIPEBOOK_LISTMODEL_PROVENANCE_H
#define RECIPEBOOK_LISTMODEL_PROVENANCE_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
	class RecipeBook;
	class RBDataHandler;
}

namespace recipebook::UI
{
	class ListModelProvenance : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class ProvenanceRoles : int
		{
			NameRole = Qt::UserRole + 1
		};
    
	public:
		ListModelProvenance(RBDataHandler& rRBDataHandler, const UIStringConverter& rConverter);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	public slots:
		QString provenanceEverywhere() const;

		QString name(int row) const;
		int renameProvenance(int row, QString newName);

		int addProvenance(QString strSortOrder);
		bool existsProvenance(QString strSortOrder) const;

		QString listUsedInIngredients(int row) const;
		bool canProvenanceBeRemoved(int row) const;
		bool removeProvenance(int row);

		void onDataReset();
        
	signals:
		void provenanceRenamed(quint32 index);

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		RBDataHandler& m_rRBDataHandler;
		const UIStringConverter& m_rConverter;
	};
}

#endif