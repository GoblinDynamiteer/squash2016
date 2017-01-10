#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	#include "Wire.h"
#endif

MPU6050 mpu;
#define OUTPUT_WOOSH

#define LED_PIN 13
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
void woosh(){
	Serial.print("WOOSH!\n");
}
void humm(){
	Serial.print("HUMM!\n");
}
int wooshDelay = 0;
int hummDelay = 300;
volatile bool mpuInterrupt = false;
void dmpDataReady() {
	mpuInterrupt = true;
}

void setup() {

	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
		Wire.begin();
		TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
	#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
		Fastwire::setup(400, true);
	#endif
	Serial.begin(115200);
	while (!Serial);
	Serial.println(F("Initializing I2C devices..."));
	mpu.initialize();
	Serial.println(F("Testing device connections..."));
	Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
	Serial.println(F("Initializing DMP..."));
	devStatus = mpu.dmpInitialize();
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788-1330);
	if (devStatus == 0) {
		Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);
		Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
		attachInterrupt(0, dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
		dmpReady = true;
		packetSize = mpu.dmpGetFIFOPacketSize();
	} 
	else {
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}
	pinMode(LED_PIN, OUTPUT);
	}

void loop() {
	if (!dmpReady) return;
	while (!mpuInterrupt && fifoCount < packetSize) {
	}
	mpuInterrupt = false;
	mpuIntStatus = mpu.getIntStatus();
	fifoCount = mpu.getFIFOCount();
	if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
		mpu.resetFIFO();
		Serial.println(F("FIFO overflow!"));
	} else if (mpuIntStatus & 0x02) {
		while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
		mpu.getFIFOBytes(fifoBuffer, packetSize);
		fifoCount -= packetSize;
		
		#ifdef OUTPUT_WOOSH
			mpu.dmpGetQuaternion(&q, fifoBuffer);
			mpu.dmpGetAccel(&aa, fifoBuffer);
			mpu.dmpGetGravity(&gravity, &q);
			mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
			mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
			if((abs(aaWorld.x) > 1500 || abs(aaWorld.y) > 1500) && !wooshDelay){
				woosh();
				Serial.print("X: ");
				Serial.print(aaWorld.x);
				Serial.print(" Y: ");
				Serial.println(aaWorld.y);
				wooshDelay = 50;
			}
			if(!hummDelay){
				humm();
				hummDelay = 50;
			}
			 delay(1);
			if(wooshDelay > 0){
				wooshDelay--;
			}
			hummDelay--;
		#endif
		
		// blink LED to indicate activity
		blinkState = !blinkState;
		digitalWrite(LED_PIN, blinkState);
	}
}


