/*
 * TestCOIL.cpp
 *
 *  Created on: 12.03.2009
 *      Author: dgrob
 */

#include "TestCOIL.h"

#include "../Core/COIL/COIL.h"
#include "../Core/COIL/EmulatedCOIL.h"

#include "../Core/Core.h"

#include "../Core/Library/SleeperThread.h"

QTEST_MAIN(TestCOIL)

void TestCOIL::initTestCase(){
	Core *core = new Core();
	QString controller = "emss Controller";
	QString movementrTracker = "Raw Movement Tracker";
	QString navigation = "Spline Navigation";
	QString serialPort = "0";
	bool emulation = true;

	core->connect(controller, movementrTracker, navigation, serialPort, emulation);

	coil = new EmulatedCOIL(core);
	coil->startOI();
}

void TestCOIL::cleanupTestCase(){
	delete coil;
}

void TestCOIL::testEnterPassiveMode(){
	coil->enterPassiveMode();
	QCOMPARE(coil->readSensor(COIL::SENSOR_OI_MODE), (int)COIL::OI_MODE_PASSIVE);
}

void TestCOIL::testEnterSafeMode(){
	coil->enterSafeMode();
	QCOMPARE(coil->readSensor(COIL::SENSOR_OI_MODE), (int)COIL::OI_MODE_SAFE);
}

void TestCOIL::testEnterFullMode(){
	coil->enterFullMode();
	QCOMPARE(coil->readSensor(COIL::SENSOR_OI_MODE), (int)COIL::OI_MODE_FULL);
}

void TestCOIL::testDrive100mmPerS(){
	coil->drive(100,0);
	QCOMPARE(coil->getVelocity(), 100);
	QCOMPARE(coil->getAngle(), 0);
}

void TestCOIL::testDrive100mmPerSBackward(){
	coil->drive(-100,0);
	QCOMPARE(coil->getVelocity(), -100);
	QCOMPARE(coil->getAngle(), 0);
}

void TestCOIL::testDrive600mmPerSBackward(){
	coil->drive(-600,0);
	QCOMPARE(coil->getVelocity(), -500);
	QCOMPARE(coil->getAngle(), 0);
}

void TestCOIL::testDrive600mmPerS(){
	coil->drive(600,0);
	QCOMPARE(coil->getVelocity(), 500);
	QCOMPARE(coil->getAngle(), 0);
}

void TestCOIL::testDrive0mmPerS(){
	coil->drive(0,0);
	QCOMPARE(coil->getVelocity(), 0);
	QCOMPARE(coil->getAngle(), 0);
}

void TestCOIL::testDriveRadius0(){
	coil->drive(100,0);
	QCOMPARE(coil->getTurningRadius(), 0);
}

void TestCOIL::testDriveRadius100(){
	coil->drive(100,100);
	QCOMPARE(coil->getTurningRadius(), 100);
}

void TestCOIL::testDriveRadiusMinus100(){
	coil->drive(100,-100);
	QCOMPARE(coil->getTurningRadius(), 0);
}

void TestCOIL::testDirectDrive100mmPerS(){
	coil->directDrive(100,100);
	QCOMPARE(coil->getVelocity(), 100);
}

void TestCOIL::testDirectDrive100mmPerSBackward(){
	coil->directDrive(-100,-100);
	QCOMPARE(coil->getVelocity(), -100);
}

void TestCOIL::testDirectDrive600mmPerS(){
	coil->directDrive(600,600);

	QCOMPARE(coil->getVelocity(), 500);

	// 1sec * 500mm/s = 500mm
	coil->getDistance();

	QTest::qWait(1000);

	int distance = coil->getDistance();

	QVERIFY2( distance < 510 && distance > 490, "Actual distance isn't in the expected range" );
}

void TestCOIL::testDirectDrive600mmPerSBackward(){
	coil->directDrive(-600,-600);
	QCOMPARE(coil->getVelocity(), -500);

	// 1sec * -500mm/s = -500mm
	coil->getDistance();

	QTest::qWait(1000);

	int distance = coil->getDistance();

	QVERIFY2( distance > -510 && distance < -490, "Actual distance isn't in the expected range" );

}

void TestCOIL::testDirectDrive0mmPerS(){
	coil->directDrive(0,0);
	QCOMPARE(coil->getVelocity(), 0);

	// 1sec * 0mm/s = 0mm
	coil->getDistance();

	QTest::qWait(1000);

	int distance = coil->getDistance();

	QVERIFY2( distance < 10 && distance > -10, "Actual distance isn't in the expected range" );
}

void TestCOIL::testDirectDrive100Left200Right(){
	coil->directDrive(100,200);
}

void TestCOIL::testDirectDrive200Left100Right(){
	coil->directDrive(200,100);
}

void TestCOIL::testDirectDrive100Left0Right(){
	coil->directDrive(100,0);
}

void TestCOIL::testDirectDrive0Left100Right(){
	coil->directDrive(0,100);
}

void TestCOIL::testDirectDrive0LeftMinus100Right(){
	coil->directDrive(0,-100);
}

void TestCOIL::testDirectDriveMinus100Left0Right(){
	coil->directDrive(-100,0);
}

void TestCOIL::testDirectDriveMinus100LeftMinus200Right(){
	coil->directDrive(-100,-200);
	// test angle and distance
	coil->getDistance();
	coil->getAngle();
	QTest::qWait(1000);

	//QCOMPARE(coil->getDistance(), 0);

	int angle = coil->getAngle();
	// According to KrusiGrob08 pg. 36 the angle should be -71.6197244 degrees
	QCOMPARE(angle, 0);
	//QVERIFY2( angle > -75 && angle < -70, "Actual distance isn't in the expected range" );
}

void TestCOIL::testDirectDriveMinus200LeftMinus100Right(){
	coil->directDrive(-200,-100);


}

void TestCOIL::testDirectDriveMinus100Left100Right(){
	coil->directDrive(-100,100);

	// test angle and distance
	coil->getDistance();
	coil->getAngle();
	QTest::qWait(1000);

	QCOMPARE(coil->getDistance(), 0);

	int angle = coil->getAngle();
	// According to KrusiGrob08 pg. 36 the angle should be 47.7464829 degrees
	QVERIFY2( angle < 50 && angle > 45, "Actual distance isn't in the expected range" );
}

void TestCOIL::testDirectDrive100LeftMinus100Right(){
	coil->directDrive(100,-100);

	// test angle and distance
	coil->getDistance();
	coil->getAngle();
	QTest::qWait(1000);
	QCOMPARE(coil->getDistance(), 0);

	int angle = coil->getAngle();
	// According to KrusiGrob08 pg. 36 the angle should be -47.7464829 degrees
	QVERIFY2( angle > -50 && angle < -45, "Actual distance isn't in the expected range" );
}
