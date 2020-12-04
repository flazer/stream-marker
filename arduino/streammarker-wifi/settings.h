#define NUM_PIXELS 35 //Amount of leds
#define DATA_PIN D2 //Pin for controlling the strip

#define LED_BRIGHTNESS 250 //Brighness of your leds

int handlePingSecs = 120; //Interval to ping server in seconds

int animationRepeatsFallback = 3; //Fallback amount of animation repeats
int animationSpeedFallback = 20; //Fallback amount of animation speed

//Only change something below, if you really know what you're doing
int loopCnt = 0;
int pingCnt = 0;
uint8_t rCurrent = 0;
uint8_t gCurrent = 0;
uint8_t bCurrent = 0;
bool animationActive = false;
int animationRepeats = 0;
int animationRepeatCnt = 0;
int animationSpeed = 0;
int ledBrightnessCurrent = 0;
int fadeDirection = 1;
