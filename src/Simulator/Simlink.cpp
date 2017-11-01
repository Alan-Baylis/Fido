#include "../../include/Simulator/Simlink.h"

#define _USE_MATH_DEFINES

#include <SFML/Audio.hpp>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <ctime>
#include <math.h>
#include <functional>

Simlink::Simlink() : emitter(20), robot(850, 250, 50, 40), line(sf::Vector2f(0, 0), sf::Vector2f(1000, 1000)), ball(15, 100) {
	irVal = motors.motorOne = motors.motorTwo = 0;
	micVal = 30;
	visVal = batVal = 50;
	tempVal = 50;

	imu.accel.xComp = imu.accel.yComp = imu.accel.zComp = 0;
	imu.compass.xComp = imu.compass.yComp = imu.compass.zComp = 0;
	imu.gyro.xComp = imu.gyro.yComp = imu.gyro.zComp = 0;

	click = false;
	keepWindowsOpen = true;

	ball.setFillColor(sf::Color::Red);
	ball.setOutlineThickness(-3);
	ball.setOutlineColor(sf::Color::Black);
	ball.setOrigin(15,15);
	ball.setPosition(840, 100);

	mainWindowThread = std::thread(&Simlink::mainWindowHandler, this);
}

Simlink::~Simlink() {
	closeWindow();
}

void Simlink::closeWindow() {
	keepWindowsOpen = false;

	if(mainWindowThread.joinable() == true) mainWindowThread.join();
}

void Simlink::mainWindowHandler() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	// Initialize main window
	mainWindow.create(sf::VideoMode(1200, 900), "Fido Simulator", sf::Style::Default, settings);

	sf::Texture texture;
	if (!texture.loadFromFile("resources/background.png")) {
		exit(EXIT_FAILURE);
	} background = sf::Sprite(texture);

	sf::Font font;
	if (!font.loadFromFile("resources/sansation.ttf")) {
		exit(EXIT_FAILURE);
	} sf::Text text("Hello SFML", font, 50);
	text.setColor(sf::Color::Black);

	while (keepWindowsOpen == true) {
		updateMainWindow();
	}

	mainWindow.close();
}

void Simlink::updateMainWindow() {
	if (mainWindow.isOpen() == false) return;

	sf::Event event;
	while (mainWindow.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			mainWindow.close();
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) mainWindow.close();
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				click = true;
				cx = event.mouseButton.x;
				cy = event.mouseButton.y;
			} break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				click = false;
			break;
		case sf::Event::MouseMoved:
			if (click) {
				cx = event.mouseMove.x;
				cy = event.mouseMove.y;
			} break;
		default:
			break;
		}
	}

	if (click) {
		if (cx > 160 && cx < 220 && cy > 20 && cy < 280)
			imu.accel.xComp = -((cy - 20) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 20 && cy < 280)
			imu.accel.yComp = -((cy - 20) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 20 && cy < 280)
			imu.accel.zComp = -((cy - 20) - 130) / 1.28;

		if (cx > 160 && cx < 220 && cy > 320 && cy < 580)
			imu.gyro.xComp = -((cy - 320) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 320 && cy < 580)
			imu.gyro.yComp = -((cy - 320) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 320 && cy < 580)
			imu.gyro.zComp = -((cy - 320) - 130) / 1.28;

		if (cx > 160 && cx < 220 && cy > 620 && cy < 880)
			imu.compass.xComp = -((cy - 620) - 130) / 1.28;
		else if (cx > 270 && cx < 330 && cy > 620 && cy < 880)
			imu.compass.yComp = -((cy - 620) - 130) / 1.28;
		else if (cx > 380 && cx < 440 && cy > 620 && cy < 880)
			imu.compass.zComp = -((cy - 620) - 130) / 1.28;

		if (cx > 780 && cx < 850 && cy > 620 && cy < 880)
			batVal = (int)((880 - cy) / 2.6);
		else if (cx > 890 && cx < 960 && cy > 620 && cy < 880)
			irVal = (int)((880 - cy) / 2.6);
		else if (cx > 1000 && cx < 1070 && cy > 620 && cy < 880)
			visVal = (int)((880 - cy) / 2.6);
		else if (cx > 1110 && cx < 1180 && cy > 620 && cy < 880)
			micVal = (int)((880 - cy) / 2.6);
		else if (cx > 500 && cx < 700 && cy > 800 && cy < 880)
			tempVal = (int)((cx - 500) / 2);

	}

	sf::CircleShape ledCirc(40);
	ledCirc.setFillColor(sf::Color(led.r, led.g, led.b));
	ledCirc.setPosition(675, 625);
	ledCirc.setOutlineThickness(5);
	ledCirc.setOutlineColor(sf::Color(0, 0, 0));

	sf::RectangleShape slide(sf::Vector2f(40, 30));
	slide.setFillColor(sf::Color(0, 0, 0));

	mainWindow.clear();
	mainWindow.draw(background);
	mainWindow.draw(ledCirc);

	slide.setPosition(170, 133 - imu.accel.xComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(280, 133 - imu.accel.yComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(390, 133 - imu.accel.zComp*1.2);
	mainWindow.draw(slide);

	slide.setPosition(168, 435 - imu.gyro.xComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(278, 435 - imu.gyro.yComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(388, 435 - imu.gyro.zComp*1.2);
	mainWindow.draw(slide);

	slide.setPosition(166, 732 - imu.compass.xComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(276, 732 - imu.compass.yComp*1.2);
	mainWindow.draw(slide);
	slide.setPosition(386, 732 - imu.compass.zComp*1.2);
	mainWindow.draw(slide);

	slide.setPosition(797, 858 - batVal*2.5);
	mainWindow.draw(slide);
	slide.setPosition(906, 858 - irVal*2.5);
	mainWindow.draw(slide);
	slide.setPosition(1016, 858 - visVal*2.5);
	mainWindow.draw(slide);
	slide.setPosition(1126, 858 - micVal*2.5);
	mainWindow.draw(slide);

	sf::RectangleShape horizontalSlide(sf::Vector2f(30, 40));
	horizontalSlide.setFillColor(sf::Color(0, 0, 0));

	horizontalSlide.setPosition(500 + tempVal * 2, 835);
	mainWindow.draw(horizontalSlide);

	/// Kinematics
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(mainWindow);
		if(mousePosition.x > 496 + emitter.getRadius() && mousePosition.x < 1200 - emitter.getRadius() && mousePosition.y > 0 + emitter.getRadius() && mousePosition.y < 595 - emitter.getRadius())
			emitter.set(sf::Mouse::getPosition(mainWindow));
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		emitter.bye();
	}

	mainWindow.draw(robot);
	mainWindow.draw(emitter);

	//mainWindow.draw(line);
	//mainWindow.draw(ball);

	mainWindow.display();

	sf::sleep(sf::milliseconds(25));
}

TDVect Simlink::getAccel() {
	return imu.accel;
}

TDVect Simlink::getGyro() {
	return imu.gyro;
}

TDVect Simlink::getCompass() {
	TDVect emitSense = emitter.sense(robot, 50);
	imu.compass = { emitSense.xComp / 6.4, emitSense.yComp / 6.4, emitSense.zComp / 6.4 };
	return imu.compass;
}

bool Simlink::isLeftOfLine() {
	return line.isLeft(robot.getPosition());
}

double Simlink::distanceFromLine() {
	return line.distance(robot.getPosition());
}

void Simlink::getBallDisplacement(double *x, double *z) {
	double lineLength = 100;
	double rotationX = robot.getRotation() * M_PI / 180.0;
	Line xLine = Line(robot.getPosition(), robot.getPosition() + sf::Vector2f(cos(rotationX)*lineLength, sin(rotationX)*lineLength));
	*x = xLine.distance(ball.getPosition());
	if(!xLine.isLeft(ball.getPosition())) *x *= -1;

	double rotationZ = (robot.getRotation() - 90) * M_PI / 180.0;
	Line zLine = Line(robot.getPosition(), robot.getPosition() + sf::Vector2f(cos(rotationZ)*lineLength, sin(rotationZ)*lineLength));
	*z = zLine.distance(ball.getPosition());
	if(zLine.isLeft(ball.getPosition())) *z *= -1;
	std::cout << *x << " " << *z << "\n"; std::cout.flush();
}

int Simlink::getMicrophone() {
	return micVal;
}

int Simlink::getIR() {
	return irVal;
}

int Simlink::getVis() {
	return visVal;
}

int Simlink::getBattery() {
	return batVal;
}

void Simlink::setLED(int r, int g, int b, int i) {
	led.r = r * i/100;
	led.g = g * i/100;
	led.b = b * i/100;
}

void Simlink::setMotors(int motorOne, int motorTwo, double speed, double deltaTime) {
	motors.motorOne = motorOne;
	motors.motorTwo = motorTwo;

	sf::Vector2f previousRobotPosition = robot.getPosition();

	robot.go(motors.motorOne, motors.motorTwo, speed, deltaTime);

	sf::Vector2f newRobotPosition = robot.getPosition();
	if (newRobotPosition.x < 500 + robot.getGlobalBounds().height / 2
		|| newRobotPosition.x > 1200 - robot.getGlobalBounds().height / 2
		|| newRobotPosition.y < 0 + robot.getGlobalBounds().height / 2
		|| newRobotPosition.y > 595 - robot.getGlobalBounds().height / 2) {
		robot.setPosition(previousRobotPosition);
	}
}

void Simlink::chirp(int volume, int frequency) {
	piezo.frequency = frequency;
	piezo.volume = volume;
	int endTime = 200 + clock();
	while(clock() != endTime);
	piezo.volume = 0;
}

int Simlink::getTemperature() {
	return tempVal;
}

void Simlink::placeRobotInRandomPosition() {
	double randX = (((double)rand() / (double)RAND_MAX) * (1150 - 550)) + 550;
	double randY = ((double)rand() / (double)RAND_MAX) * (530 - 50) + 50;

	robot.setPosition(randX, randY);
}

void Simlink::placeEmitterInRandomPosition() {
	double randX = ((double)rand() / (double)RAND_MAX) * (1150 - 550) + 550;
	double randY = ((double)rand() / (double)RAND_MAX) * (530 - 50) + 50;

	emitter.set(sf::Vector2i((int)randX, (int)randY));
}

double Simlink::getDistanceOfRobotFromEmitter() {
	double difX = emitter.getPosition().x - robot.getPosition().x;
	double difY = emitter.getPosition().y - robot.getPosition().y;

	return sqrt(pow(difX, 2) + pow(difY, 2));
}

void Simlink::getRobotDisplacementFromEmitter(double *x, double *y) {
	*x = emitter.getPosition().x - robot.getPosition().x;
	*y = emitter.getPosition().y - robot.getPosition().y;
}

void Simlink::placeLine(sf::Vector2f p1, sf::Vector2f p2) {
	line = Line(p1, p2);
}
