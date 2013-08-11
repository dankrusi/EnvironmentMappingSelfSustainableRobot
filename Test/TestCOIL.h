/*
 * TestCore.h
 *
 *  Created on: 12.03.2009
 *      Author: dgrob
 */

#ifndef TESTCOIL_H_
#define TESTCOIL_H_

#include <QtTest/QtTest>

// Forwards declarations
class COIL;


class TestCOIL : public QObject {

	Q_OBJECT

public:
	COIL *coil;

private slots:
	void initTestCase();
	void cleanupTestCase();

	// test enter passive mode
	void testEnterPassiveMode();

	// test enter passive mode
	void testEnterSafeMode();

	// test enter passive mode
	void testEnterFullMode();


	// test method drive(speed, radius) with no radius but various speed
	void testDrive100mmPerS();
	void testDrive100mmPerSBackward();
	void testDrive600mmPerSBackward();
	void testDrive600mmPerS();
	void testDrive0mmPerS();

	// test method drive(speed, radius) with various radius and speed 100mm/s
	void testDriveRadius0();
	void testDriveRadius100();
	void testDriveRadiusMinus100();

	// test direct drive
	void testDirectDrive100mmPerS();
	void testDirectDrive100mmPerSBackward();
	void testDirectDrive600mmPerS();
	void testDirectDrive600mmPerSBackward();
	void testDirectDrive0mmPerS();

	// test direct drive with different wheel speeds
	void testDirectDrive100Left200Right();
	void testDirectDrive200Left100Right();
	void testDirectDrive100Left0Right();
	void testDirectDrive0Left100Right();
	void testDirectDrive0LeftMinus100Right();
	void testDirectDriveMinus100Left0Right();
	void testDirectDriveMinus100LeftMinus200Right();
	void testDirectDriveMinus200LeftMinus100Right();
	void testDirectDriveMinus100Left100Right();
	void testDirectDrive100LeftMinus100Right();

};


#endif /* TESTCOIL_H_ */
