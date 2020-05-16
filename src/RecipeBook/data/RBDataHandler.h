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

		RBDataReadHandle(const RBDataReadHandle&) = delete;
		RBDataReadHandle(RBDataReadHandle&&) = delete;
		RBDataReadHandle& operator=(const RBDataReadHandle&) = delete;

	private:
		QReadLocker m_Locker;
		const RecipeBook& m_rRecipeBook;
	};

	class RBDataWriteHandle
	{
	public:
		RBDataWriteHandle(RBDataHandler& rDataHandler, bool dontSetDataChanged = false);
		RecipeBook& data() { return m_rRecipeBook; }

		RBDataWriteHandle(const RBDataWriteHandle&) = delete;
		RBDataWriteHandle(RBDataWriteHandle&&) = delete;
		RBDataWriteHandle& operator=(const RBDataWriteHandle&) = delete;

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
