#ifndef RECIPEBOOK_SETTINGS_H
#define RECIPEBOOK_SETTINGS_H

#include <QSettings>
#include "data/Unit.h"

namespace recipebook
{
    class RecipeBookSettings : public QObject
    {
        Q_OBJECT

    public:
        RecipeBookSettings();

    public slots:
        QString getApplicationInstanceUID();

        quint32 getDefaultUnit() const;
        void setDefaultUnit(quint32 defaultUnit);

        quint32 getDefaultRecipeNrPersons() const;
        void setDefaultRecipeNrPersons(quint32 nrPersons);

        QString getDefaultCategory() const;
        void setDefaultCategory(QString strCategory);

        QString getDefaultSortOrder() const;
        void setDefaultSortOrder(QString strSortOrder);

    signals:
        void resetAllData() const;
    };
}

#endif
