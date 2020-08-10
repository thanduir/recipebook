#ifndef RECIPEBOOK_LISTMODEL_RBCONFIG_ITEMS_H
#define RECIPEBOOK_LISTMODEL_RBCONFIG_ITEMS_H

#include <QAbstractListModel>

namespace recipebook
{
	class RecipeBookConfiguration;
	class RBDataHandler;
	class RBDataWriteHandle;
	class RBDataReadHandle;
}

namespace recipebook::UI
{
	class ListModelRBConfigItems : public QAbstractListModel
	{
		Q_OBJECT
	public:
		enum class RBConfigItemsRoles : int
		{
			NameRole = Qt::UserRole + 1,
			HeaderRole,
			HeaderLevelRole,
			MaxHeaderLevelRole,
			EverythingSetRole
		};

	public:
		ListModelRBConfigItems(RBDataHandler& rRBDataHandler);

		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
		virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;

	public slots:
		void setCurrentConfig(int row);

		bool existsRecipe(QString strRecipe) const;

		QString name(int row) const;
		bool renameHeader(int row, QString newName);
		bool isEverythingSet(int row) const;

		bool isHeader(int row) const;

		quint32 headerLevel(int row) const;
		quint32 maxHeaderLevel(int row) const;
		void setHeaderLevel(int row, quint32 level);
		
		bool canRecipesBeAdded() const;
		int addHeader(QString strName, int pos);
		int addRecipe(QString strName, int pos);
		bool removeItem(int row);
		void moveItem(int row, int target);
		
		void onDependentItemRemoved(int index);

		// edit list interface
		void beginEditList();
		bool itemSelected(QString itemName) const;
		void changeState(QString itemName, bool selected);
		void cancelEditList();
		bool applyEditList();

		// Move items
		void beginMove(int row);
		void updateMoveTarget(int target);
		void applyMove();

	protected:
		virtual QHash<int, QByteArray> roleNames() const override;

	private:
		void setDataChanged(int row, RBConfigItemsRoles role);
		qint32 getPreviousHeaderLevel(const RecipeBookConfiguration& rConfig, int row) const;

		recipebook::RecipeBookConfiguration* getConfig(recipebook::RBDataWriteHandle& rHandle);
		const recipebook::RecipeBookConfiguration* getConfig(recipebook::RBDataReadHandle& rHandle) const;

	private:
		RBDataHandler& m_rRBDataHandler;
		int m_CurrentConfig = -1;

		QStringList m_EditListSelectedValues;

		int m_MoveFrom = -1;
		int m_MoveTo = -1;
	};
}

#endif
