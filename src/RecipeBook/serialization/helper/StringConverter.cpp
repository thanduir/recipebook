#include "StringConverter.h"
#include <QException>
#include "../../data/RecipeBookConfigItem.h"

using namespace recipebook;

static const QString c_strSizeSmall			= "Small";
static const QString c_strSizNormal			= "Normal";
static const QString c_strSizeLarge			= "Large";

static const QString c_strStatusNone		= "None";
static const QString c_strStatusTaken		= "Taken";

static const QString c_strUnitCount			= "Count";
static const QString c_strUnitKilogram		= "Kilogram";
static const QString c_strUnitGram			= "Gram";
static const QString c_strUnitLiter			= "Liter";
static const QString c_strUnitDeciliter		= "Deciliter";
static const QString c_strUnitMilliliter	= "Milliliter";
static const QString c_strUnitDessertspoon	= "Dessertspoon";
static const QString c_strUnitTeaspoon		= "Teaspoon";
static const QString c_strUnitUnitless		= "Unitless";

static const QString c_strConfigTypeHeader	= "Header";
static const QString c_strConfigTypeRecipe	= "Recipe";

QString serialization::helper::convertSize(Size size)
{
	switch(size)
	{
		case Size::Small:
			return c_strSizeSmall;

		case Size::Normal:
			return c_strSizNormal;

		case Size::Large:
			return c_strSizeLarge;

		default:
			throw QException();
	}
}

Size serialization::helper::convertSize(QString strSize)
{
	if(strSize == c_strSizeSmall)
	{
		return Size::Small;
	}
	else if(strSize == c_strSizNormal)
	{
		return Size::Normal;
	}
	else if(strSize == c_strSizeLarge)
	{
		return Size::Large;
	}
	else
	{
		throw QException();
	}
}

QString serialization::helper::convertStatus(Status status)
{
	switch(status)
	{
		case Status::None:
			return c_strStatusNone;

		case Status::Taken:
			return c_strStatusTaken;

		default:
			throw QException();
	}
}

Status serialization::helper::convertStatus(QString strStatus)
{
	if(strStatus == c_strStatusNone)
	{
		return Status::None;
	}
	else if(strStatus == c_strStatusTaken)
	{
		return Status::Taken;
	}
	else
	{
		throw QException();
	}
}

QString serialization::helper::convertUnit(Unit unit)
{
	switch(unit)
	{
		case Unit::Count:
			return c_strUnitCount;

		case Unit::Kilogram:
			return c_strUnitKilogram;

		case Unit::Gram:
			return c_strUnitGram;

		case Unit::Liter:
			return c_strUnitLiter;

		case Unit::Deciliter:
			return c_strUnitDeciliter;

		case Unit::Milliliter:
			return c_strUnitMilliliter;

		case Unit::Dessertspoon:
			return c_strUnitDessertspoon;

		case Unit::Teaspoon:
			return c_strUnitTeaspoon;

		case Unit::Unitless:
			return c_strUnitUnitless;

		default:
			throw QException();
	}       
}

Unit serialization::helper::convertUnit(QString strUnit)
{
	if(strUnit == c_strUnitCount)
	{
		return Unit::Count;
	}
	else if(strUnit == c_strUnitKilogram)
	{
		return Unit::Kilogram;
	}
	else if(strUnit == c_strUnitGram)
	{
		return Unit::Gram;
	}
	else if(strUnit == c_strUnitLiter)
	{
		return Unit::Liter;
	}
	else if(strUnit == c_strUnitDeciliter)
	{
		return Unit::Deciliter;
	}
	else if(strUnit == c_strUnitMilliliter)
	{
		return Unit::Milliliter;
	}
	else if(strUnit == c_strUnitDessertspoon)
	{
		return Unit::Dessertspoon;
	}
	else if(strUnit == c_strUnitTeaspoon)
	{
		return Unit::Teaspoon;
	}
	else if(strUnit == c_strUnitUnitless)
	{
		return Unit::Unitless;
	}
	else
	{
		throw QException();
	}
}

QString serialization::helper::convertRBConfigType(RecipeBookConfigItemType unit)
{
	switch(unit)
	{
	case RecipeBookConfigItemType::Header:
		return c_strConfigTypeHeader;

	case RecipeBookConfigItemType::Recipe:
		return c_strConfigTypeRecipe;

	default:
		throw QException();
	}
}

RecipeBookConfigItemType serialization::helper::convertRBConfigType(QString strType)
{
	if(strType == c_strConfigTypeHeader)
	{
		return RecipeBookConfigItemType::Header;
	}
	else if(strType == c_strConfigTypeRecipe)
	{
		return RecipeBookConfigItemType::Recipe;
	}
	else
	{
		throw QException();
	}
}
