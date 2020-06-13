#include "data/RBDataHandler.h"

using namespace recipebook;

RBDataReadHandle::RBDataReadHandle(const RBDataHandler& rDataHandler)
:	m_Locker(&rDataHandler.m_Lock), m_rRecipeBook(rDataHandler.m_RecipeBook)
{
}

RBDataWriteHandle::RBDataWriteHandle(RBDataHandler& rDataHandler, bool dontSetDataChanged)
: m_Locker(&rDataHandler.m_Lock), m_rRecipeBook(rDataHandler.m_RecipeBook)
{
	if(!dontSetDataChanged)
	{
		rDataHandler.m_bDataChanged = true;
	}
}

RBDataHandler::RBDataHandler()
:	m_bDataChanged(false)
{
}
