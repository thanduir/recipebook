#include "RBDataHandler.h"

using namespace recipebook;

RBDataReadHandle::RBDataReadHandle(const RBDataHandler& rDataHandler)
:	m_Locker(&rDataHandler.m_Lock), m_rRecipeBook(rDataHandler.m_RecipeBook)
{
}

RBDataWriteHandle::RBDataWriteHandle(RBDataHandler& rDataHandler)
: m_Locker(&rDataHandler.m_Lock), m_rRecipeBook(rDataHandler.m_RecipeBook)
{
}

RBDataHandler::RBDataHandler()
{
}
