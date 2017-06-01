#ifndef MENU_CPP
#define MENU_CPP

#include "menu.h"
#include "includes.h"
#include "framework.h"

#include <iostream>
#include <iterator>

// Menu
Menu::Menu()
{
	//default build
	buttonColor = { 81, 84, 89 };
	selectedButtonColor = { 239, 237, 81 };
	backgroundColor = { 162, 173, 191 };
}
Menu::Menu(Vector3 buttonC, Vector3 selecC, Vector3 bgC)
{
	buttonColor = buttonC;
	selectedButtonColor = selecC;
	backgroundColor = bgC;
}
Menu::~Menu() {}

// Methods
void Menu::addButton(float originX, float originY, float height, float width)
{
	Vector4 position = { originX, originY, height, width };
	Button* button = new Button(position, buttonColor, backgroundColor);
	buttons.push_back(button);
}

void Menu::draw()
{
	/*
	for (int i = 0; i < 50; i++)
		glDrawColor(backgroundColor);
	*/
}

// Button
Button::Button(Vector4 pos, Vector3 buttonC, Vector3 selC)
{
	position = pos;
	buttonColor = buttonC;
	selectedButtonColor = selC;
}
Button::~Button() {}

#endif // !MENU_CPP

/*
void addButton(float originX, float originY, float height, float width)
{
float newXmin = originX - (width / 2);
float newXmax = originX + (width / 2);
float newYmin = originY - (height / 2);
float newYmax = originY + (height / 2);
if (newXmin < 0 || newXmax > )
{
std::cout << "Button coordinates out of range" << std::endl;
return;
}

for (std::vector<Button>iterator it = buttonSs.begin(); it != buttons.end(); it++)
{
float xmin = it.position.x - (it.position.w / 2);
float xmax = it.position.x + (it.position.w / 2);
float ymin = it.position.y - (it.position.z / 2);
float ymax = it.position.y + (it.position.z / 2);
if (it.position.x)
}
} */