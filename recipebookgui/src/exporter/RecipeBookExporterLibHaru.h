#pragma once

#include <memory>
#include <QObject>

class UIStringConverter;

namespace recipebook
{
	class RecipeBookConfiguration;

	bool exportRecipeBookLibHaru(const UIStringConverter& converter, const RecipeBookConfiguration& config, QString filename);
}
