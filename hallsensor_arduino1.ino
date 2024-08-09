#define HALL 3  //pin number for hall sensor input, (2 used by MPU6050?)
#define DpR 2 //distance traveled by one revolution in m

volatile unsigned char tick = 0;
volatile unsigned long tickTime_isr;
unsigned long tickTime=0;
float spd = 0;

void setup() {
  Serial.begin(115200);
  pinMode(HALL, INPUT);
  attachInterrupt(HALL-2, hall_ISR, FALLING); //first number is 0 for pin 2 and 1 for pin 3
}

void loop() {

    getSpeed(); //call once per loop to catch the tick (if occured) and calculate time 

}

void hall_ISR() {
  tickTime_isr = micros();
  tick = 1;
}

void getSpeed() {
  unsigned long old_tickTime;
  if (tick) {
    tick = 0; //reset interupt flag (single bit so no need to stop interrupts)
    old_tickTime=tickTime;  //save old time to calcualte difference (tickTime is GLOBAL)
    noInterrupts();
    tickTime = tickTime_isr;  //get new time
    interrupts();
    spd = 1000000*DpR / (((float)tickTime-(float)old_tickTime));  //calcualte speed (spd is GLOBAL)
    Serial.print("Speed = ");Serial.println(spd);
  }
}
