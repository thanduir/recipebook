#ifndef RECIPEBOOK_FILTERMODEL_RECIPEITEMS_H
#define RECIPEBOOK_FILTERMODEL_RECIPEITEMS_H

#include <QSortFilterProxyModel>

namespace recipebook::UI
{
    class FilterModelIngredients : public QSortFilterProxyModel
    {
        Q_OBJECT
    public:
        FilterModelIngredients();

    public slots:
        void setFilterString(QString strFilter);    

        QString name(int row) const;
        
        // TODO: Extend with other needed methods! (for filtering in ingredientsTab)

    protected:
        virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

    private:
        QString m_FilterString;
    };
}

#endif
