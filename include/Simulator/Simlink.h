#ifndef __Simulator__Simlink__
#define __Simulator__Simlink__

#include "../../include/Simulator/Robby.h"
#include "../../include/Simulator/Emitter.h"
#include "../../include/Simulator/Line.h"

#include <thread>
#include <SFML/Graphics.hpp>
#include <math.h>

// Three dimensional vector (used with IMU).
struct TDVect {
	// Components of 3d vector
	double xComp, yComp, zComp;

	// Get 3d vector as radius, xy angle, and z angle.
	void getRTP(double *r, double *xy, double *z) {
		*r = sqrt(pow(xComp, 2) + pow(yComp, 2) + pow(zComp, 2));
		*xy = atan2(xComp, yComp);
		*z = acos(zComp / *r);
	}
};

/** A robotic simulator */
class Simlink {
public:
	// Initializes the simulator
	Simlink();

	// Deinitializes the simulator
	~Simlink();

	// Closes the window of the simulator
	void closeWindow();

	/* Set the RGB LED to a certain color and intensity.
	 *
	 * Integers r, g, and b are from 0-255, and the intensity is from 0-100.
	 */
	void setLED(int r, int g, int b, int i);

	// Return sound pressure from 0-100.
	int getMicrophone();

	// Return IR intensity from 0-100.
	int getIR();

	// Return visible light intensity from 0-100.
	int getVis();

	/* Buzz at a volume and frequency from 0-100 for an abritrary duration.
	 *
	 * The frequency is mapped where 0=261Hz (middle C)
	 * and 100 is an octave higher at 523Hz. The duration
	 * is currently 200ms.
	 */
	void chirp(int volume, int frequency);

	/* Set motor values from -100 to 100
	 *
	 * Positive is forwards, negative is reverse,
	 * zero is stopped.
	 */
	void setMotors(int motorOne, int motorTwo, double speed, double deltaTime);

	// Stop the motors.
	void stop();

	// Get the current battery level (0-100).
	int getBattery();

	// Returns the 3D acceleration vector in m/s^2.
	TDVect getAccel();

	// Returns the 3D magnetic field vector.
	TDVect getCompass();

	/* Returns the 3D rotational velocity vector.
	 *
	 * The TDVect math here is a little sketchy, but
	 * I trust you'll understand what's going on. The
	 * components of TDVect are xyz rad/s.
	 */
	TDVect getGyro();

	bool isLeftOfLine();

	void getBallDisplacement(double *x, double *z);

	double distanceFromLine();

	// Gets the current temperature value from 0-100
	int getTemperature();

	// Places the robot in a random position
	void placeRobotInRandomPosition();

	// Places the emitter in a random position
	void placeEmitterInRandomPosition();

	// Gets the distance of the robot from the emitter
	double getDistanceOfRobotFromEmitter();

	// Gets the displacement of the robot from the emitter in terms of x and y components
	void getRobotDisplacementFromEmitter(double *x, double *y);

	// Places a line on the simulator. Can be detected by getLineVal()
	void placeLine(sf::Vector2f p1, sf::Vector2f p2);

	Robby robot;
	Emitter emitter;
	sf::CircleShape ball;
	int visVal;
private:
	void mainWindowHandler();

	// Updates the main simulator window
	void updateMainWindow();

	// Main window.
	sf::RenderWindow mainWindow;

	// Background of main window
	sf::Sprite background;

	// Window thread
	std::thread mainWindowThread;

	// LED with values r, g, and b from 0-255.
	struct LED { int r,g,b; };

	// IMU with 3D vectors for accel, gyro, and compass.
	struct IMU { TDVect accel; TDVect gyro; TDVect compass; };

	// Motor container with two motor values from -255 to +255.
	struct Motors { int motorOne, motorTwo; };

	struct Piezo { int volume, frequency; };

	// Sensor values, all ranging from 0-100.
	int irVal, micVal, batVal, tempVal;

	bool click;
	int cx,cy;
	LED led;
	Motors motors;
	IMU imu;
	Piezo piezo;
	Line line;

	bool keepWindowsOpen;
};

#endif /* defined(__Simulator__Simlink__) */
