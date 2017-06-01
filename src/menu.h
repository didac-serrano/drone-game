/*
	Clase para la gestión de las pantallas de selección del juego
	(aka aquellas que no son mundo o niveles jugables)
	
	Dídac Serrano TJE 2017
*/
#ifndef MENU_H
#define MENU_H

#include "utils.h"
#include "framework.h"
#include "includes.h"
#include <vector>


class Button
{
public:
	Button(Vector4 pos, Vector3 buttonC, Vector3 selC); // constructor
	~Button(); // destructor

	// Some attributes
	Vector4 position;
	Vector3 buttonColor;
	Vector3 selectedButtonColor;
};

class Menu
{
public:
	Menu(); // constructor
	Menu(Vector3 buttonC, Vector3 selecC, Vector3 bgC);
	~Menu(); // destructor

	// Some attributes
	Vector3 buttonColor;
	Vector3 selectedButtonColor;
	Vector3 backgroundColor;
	std::vector<Button*> buttons;
	// amb el temps estaria fino posar textures
	// ni que sigui al background

	// Methods
	void addButton(float originX, float originY, float height, float width);
	void draw();

	//window
	/*
	SDL_Window* window;
	int window_width;
	int window_height; */
};



#endif