#pragma once

#include <raylib.h>

//Class which allows for the creation of a button which has 3 stages of action, normal, hover and pressed.
//This class will return a boolean true if the button is pressed
class Button
{
public:
	Texture2D imageNoHover;
	Texture2D imageHover;
	Texture2D imagePressed;
	double xModifier;
	double yModifier;

	//Instatiates a button which has 3 images to diffrentiate between, no hover, hover and a pressed button.
	Button(const char* image1, const char* image2, const char* image3, double x, double y);
	//Draws the button to the screen and displays the certain image depending on the circumstances
	bool draw();
	//Unloads all textures that are instialised with this button
	void unload();


private:
	Texture2D imageTemp;
	bool buttonPressed{ false };
	float buttonFrame;
	Rectangle buttonSource;
	Rectangle buttonBounds;
	Vector2 mousePos{ 0, 0 };
	float screenWidth = GetScreenWidth();
	float screenHeight = GetScreenHeight();

};
