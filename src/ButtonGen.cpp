#include "ButtonGen.h"
#include <string>
#include <iostream>
#include <raylib.h>

using namespace std;

// Constructor
Button::Button(const char* image1, const char* image2, const char* image3, double x, double y)
{
	imageNoHover = LoadTexture(image1);
	imageHover = LoadTexture(image2);
	imagePressed = LoadTexture(image3);
	imageTemp = imageNoHover;
	xModifier = x;
	yModifier = y;

	Button::buttonFrame = (float)Button::imageTemp.height;
	Button::buttonSource = { 0, 0, (float)Button::imageTemp.width, Button::buttonFrame };
	Button::buttonBounds = { Button::screenWidth / (float)Button::xModifier - Button::imageTemp.width / (float)Button::xModifier,
		Button::screenHeight / (float)Button::yModifier - Button::imageTemp.height / (float)Button::yModifier, (float)Button::imageTemp.width, Button::buttonFrame };
}


bool Button::draw()
{
	Button::mousePos = GetMousePosition();
	Button::buttonPressed = false;

	//Colours image if mouse is within button
	if (CheckCollisionPointRec(Button::mousePos, Button::buttonBounds)) {
		Button::imageTemp = Button::imageHover;
		// Changes image to pressed when button is clicked
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
			Button::imageTemp = Button::imagePressed;
			Button::buttonPressed = true;
		}
	}
	else {
		Button::imageTemp = Button::imageNoHover;
	} 

	DrawTextureRec(Button::imageTemp, Button::buttonSource, { Button::buttonBounds.x, Button::buttonBounds.y }, WHITE);

	return Button::buttonPressed;
}

// Unloads button image
void Button::unload()
{
	UnloadTexture(Button::imageNoHover);
	UnloadTexture(Button::imageHover);
	UnloadTexture(Button::imagePressed);
}
