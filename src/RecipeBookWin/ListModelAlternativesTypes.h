#ifndef RECIPEBOOK_LISTMODEL_ALTERNATIVES_TYPES_H
#define RECIPEBOOK_LISTMODEL_ALTERNATIVES_TYPES_H

#include <QSortFilterProxyModel>

namespace recipebook::UI
{
    class ListModelAlternativesTypes : public QSortFilterProxyModel
    {
        Q_OBJECT
    public:
        ListModelAlternativesTypes();

    public slots:
        QString name(int row) const;
        int renameType(int row, QString newName);

        QString color(int row) const;
        void setColor(int row, QString strColor);

        int addType(QString strType);
        bool existsType(QString strType) const;

        bool canTypeBeRemoved(int row) const;
        bool removeType(int row);

    protected:
        virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;
    };
}

#endif
