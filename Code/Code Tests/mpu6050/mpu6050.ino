#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	#include "Wire.h"
#endif

MPU6050 mpu;

bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

/*	 For counting program cycles	*/
uint16_t ticker = 0ul;

/*	 Pins for LED output tests	*/
/*	 9, 10, 5, 6	*/
enum {LED1 = 9, LED2, LED3 = 5, LED4, LED_BOARD = 13};

/*	 MPU Acc value to trigger saber racket wooshing effect	*/
#define MPU_ACC_TRIGGER 1500;

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

void woosh(){
	Serial.print("WOOSH!\n");
}
void humm(){
	Serial.print("HUMM!\n");
}

	/*	 Set leds to brightness	*/
void ledControl(int brightness){
	analogWrite(LED1, brightness);
	analogWrite(LED2, brightness);
	analogWrite(LED3, brightness);
	analogWrite(LED4, brightness);
}

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
	
	/*	 Initialize MPU	*/
	Serial.println(F("Initializing MPU.."));
	mpu.initialize();
	Serial.println(F("Testing device connections..."));
	Serial.println(mpu.testConnection() ? 
		F("MPU6050 connection successful") : 
		F("MPU6050 connection failed"));
	Serial.println(F("Initializing DMP..."));
	devStatus = mpu.dmpInitialize();
	
	/*	 Offsets	*/
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
	
	/*	 LED Pins	*/
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED_BOARD, OUTPUT);
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
		
		mpu.dmpGetQuaternion(&q, fifoBuffer);
		mpu.dmpGetAccel(&aa, fifoBuffer);
		mpu.dmpGetGravity(&gravity, &q);
		mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
		mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
		
		/*	 Trigger if MPU unit is moved fast enough	*/
		if((abs(aaWorld.x) > MPU_ACC_TRIGGER 
			|| abs(aaWorld.y) > MPU_ACC_TRIGGER)){
				ledControl(255);
				if(ticker % 50 == 0){
					woosh();
					Serial.print("X: ");
					Serial.print(aaWorld.x);
					Serial.print(" Y: ");
					Serial.println(aaWorld.y);
			}
		}
		else{
			ledControl(0);
		}
		
		if(ticker % 100 == 0){
			humm();
		}

		ticker++;

		/*	 Board LED, indicate activity	*/
		blinkState = !blinkState;
		digitalWrite(LED_BOARD, blinkState);
	}
}


