#ifndef RECIPEBOOK_DATA_HANDLER_H
#define RECIPEBOOK_DATA_HANDLER_H

#include <QReadWriteLock>
#include <data/RecipeBook.h>

namespace recipebook
{
	class RBDataHandler;

	class RBDataReadHandle
	{
	public:
		RBDataReadHandle(const RBDataHandler& rDataHandler);
		const RecipeBook& data() { return m_rRecipeBook; }

	private:
		QReadLocker m_Locker;
		const RecipeBook& m_rRecipeBook;
	};

	class RBDataWriteHandle
	{
	public:
		RBDataWriteHandle(RBDataHandler& rDataHandler);
		RecipeBook& data() { return m_rRecipeBook; }

	private:
		QWriteLocker m_Locker;
		RecipeBook& m_rRecipeBook;
	};

	class RBDataHandler
	{
	public:
		RBDataHandler();

		bool hasDataChanged() { return m_bDataChanged; }
		void setDataUnchanged() { m_bDataChanged = false; }

	private:
		mutable QReadWriteLock m_Lock;
		RecipeBook m_RecipeBook;
		bool m_bDataChanged;

		friend class RBDataReadHandle;
		friend class RBDataWriteHandle;
	};
}

#endif
