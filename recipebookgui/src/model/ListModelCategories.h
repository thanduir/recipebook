#ifndef RECIPEBOOK_LISTMODEL_CATEGORIES_H
#define RECIPEBOOK_LISTMODEL_CATEGORIES_H

#include <QAbstractListModel>

class UIStringConverter;

namespace recipebook
{
	class RecipeBook;
	class RBDataHandler;
}

namespace recipebook::UI
{
	class ListModelCategories : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class CategoryRoles : int
		{
			NameRole = Qt::UserRole + 1
		};

	public:
		ListModelCategories(RBDataHandler& rRBDataHandler);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	public slots:
		QString name(int row) const;
		int renameCategory(int row, QString newName);

		bool canCategoriesBeAdded() const;
		int addCategory(QString strCategory);
		bool existsCategory(QString strCategory) const;

		bool canCategoryBeRemoved(int row) const;
		bool removeCategory(int row);

		void onDataReset();

	signals:
		void categoryRenamed(quint32 index);

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		RBDataHandler& m_rRBDataHandler;
	};
}

#endif
