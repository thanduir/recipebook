#ifndef RECIPEBOOK_SERIALIZATION_SERIALIZERFACTORY_H
#define RECIPEBOOK_RECIPEBOOKSERIALIZER_H

#include "IRecipeBookSerializer.h"
#include <QSharedPointer>

namespace recipebook::serialization
{
    enum class FileFormat
    {
        Json,
        JsonForApp      // Json format but with striped recipe descriptions to minimize space usage
    };

    class SerializerFactory
    {
    public:
        static QSharedPointer<IRBWriter> getWriter(FileFormat format, QString strUID);
        static QSharedPointer<IRBReader> getReader(FileFormat format);
    };
}

#endif
