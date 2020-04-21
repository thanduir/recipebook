#ifndef RECIPEBOOK_LISTMODEL_ALTERNATIVESGROUPS_H
#define RECIPEBOOK_LISTMODEL_ALTERNATIVESGROUPS_H

#include <QAbstractListModel>

namespace recipebook
{
    class RecipeBook;
}

class UIStringConverter;

namespace recipebook::UI
{
    class ListModelAlternativesGroups : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum class AlternativesGroupsRoles : int
        {
            NameRole = Qt::UserRole + 1,
            ColorRole
        };

    public:
        ListModelAlternativesGroups(RecipeBook& rRecipeBook, const UIStringConverter& rConverter);

        virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    public slots:
        QString stringNoAlternativesGroup() const;

        QString name(int row) const;
        int renameType(int row, QString newName);

        QString color(int row) const;

        void setColor(int row, QString strColor);

        int addType(QString strType);
        bool existsType(QString strType) const;

        bool canTypeBeRemoved(int row) const;
        bool removeType(int row);

        int indexOf(QString strName) const;

        void onDataReset();

    signals:
        void alternativesGroupChanged(quint32 index);

    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        RecipeBook& m_rRecipeBook;
        const UIStringConverter& m_rConverter;
    };
}

#endif
