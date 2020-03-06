#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_recipebook.h"

class RecipeBook : public QMainWindow
{
	Q_OBJECT

public:
	RecipeBook(QWidget *parent = Q_NULLPTR);

private:
	Ui::RecipeBookClass ui;
};
