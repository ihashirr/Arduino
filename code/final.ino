#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FastLED.h>

// Wi-Fi credentials
const char* ssid = "Hashir1";
const char* password = "12345678";

// Static IP address configuration
IPAddress local_IP(192, 168, 82, 184); // Desired IP address
IPAddress gateway(192, 168, 209, 155);  // Your router's gateway IP
IPAddress subnet(255, 255, 255, 0);     // Subnet mask
IPAddress dns(192, 168, 209, 155);      // DNS server

// FastLED settings
#define LED_PIN     D8
#define NUM_LEDS    150
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

// Ultrasonic sensor settings
#define TRIG_PIN D1
#define ECHO_PIN D2

ESP8266WebServer server(80);

enum Effect {
  NONE,
  LIGHTNING,
  LIGHTNING_STORM,
  THUNDER_STORM,
  ANIMATION1,
  ANIMATION2,
  ANIMATION3
};

volatile Effect currentEffect = NONE;
bool ultrasonicFeatureActive = false; // Variable to track ultrasonic feature status

void handleRoot() {
  Serial.println("Received request for /");
  String html = R"rawliteral(
    <!doctype html>
    <html lang="en">
      <head>
        <!-- Meta tags -->
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css">
        <title>Arduino</title>
      </head>
      <style>
        @keyframes rotate624 {
          0% { transform: rotate(0deg) translate3d(0, 0, 0); }
          25% { transform: rotate(3deg) translate3d(0, 0, 0); }
          50% { transform: rotate(-3deg) translate3d(0, 0, 0); }
          75% { transform: rotate(1deg) translate3d(0, 0, 0); }
          100% { transform: rotate(0deg) translate3d(0, 0, 0); }
        }
        @keyframes storm1261 {
          0% { transform: translate3d(0, 0, 0) translateZ(0); }
          25% { transform: translate3d(4px, 0, 0) translateZ(0); }
          50% { transform: translate3d(-3px, 0, 0) translateZ(0); }
          75% { transform: translate3d(2px, 0, 0) translateZ(0); }
          100% { transform: translate3d(0, 0, 0) translateZ(0); }
        }
        button {
          position: relative;
          margin: 0;
          padding: 17px 35px;
          outline: none;
          text-decoration: none;
          display: flex;
          justify-content: center;
          align-items: center;
          cursor: pointer;
          text-transform: uppercase;
          background-color: #fff;
          border: 1px solid rgba(22, 76, 167, 0.6);
          border-radius: 10px;
          color: #000000;
          font-weight: 400;
          font-family: inherit;
          z-index: 0;
          overflow: hidden;
          transition: all 0.3s cubic-bezier(0.02, 0.01, 0.47, 1);
        }
        button span {
          color: #000000;
          font-size: 14px;
          font-weight: 500;
          letter-spacing: 0.7px;
        }
        button:hover {
          animation: rotate624 0.7s ease-in-out both;
        }
        button:hover span {
          animation: storm1261 0.7s ease-in-out both;
          animation-delay: 0.06s;
        }
        .btn-click:after {
          top: -50px; 
          height: 155px;
          left: -75px;
          opacity: 0.4;
          position: absolute;
          background: #ffffff;
          content: "";
          transform: rotate(35deg);
          width: 50px;
          z-index: -10;
          transition: all 550ms cubic-bezier(0.19, 1, 0.22, 1);
        }
        .btn-click {
          border: 1px solid;
          overflow: hidden;
          position: relative;
        }
        .btn-click:hover:after {
          left: 120%;
          transition: all 550ms cubic-bezier(0.19, 1, 0.22, 1);
        }
        .btn-click span {
          z-index: 20;
        }
      </style>
      <body>
        <section id="services">
          <div class="container" style="max-width: 100%; margin: 0 auto; padding: 0;">
            <div class="row justify-content-center">
              <div class="col-md-12">
                <div class="section-intro" style="text-align: center;">
                  <h1 class="shadow-sm p-3 mb-10 bg-black rounded" style="display: flex; justify-content: center; color: white;">Project Badal</h1>
                  <div class="hline"></div>
                </div>
                <div class="container text-center">
                  <div class="row">
                    <h4 class="col order-5">
                      Lighting effect
                      <button class="btn-click" onclick="startAnimation('lightning')" style="display: block; margin: 0% auto; margin-top: 20px; background-color: rgb(255, 225, 118); border-radius: 25px;">Click me!</button>
                    </h4>
                    <h4 class="col order-5">
                      Lighting effect
                      <button class="btn-click" onclick="startAnimation('lightningStorm')" style="display: block; margin: 0% auto; margin-top: 20px; background-color: tan; border-radius: 25px;">Click me!</button>
                    </h4>
                    <h4 class="col order-5">
                      Lighting effect
                      <button class="btn-click" onclick="startAnimation('thunderStorm')" style="display: block; margin: 0% auto; margin-top: 20px; background-color: rgb(107, 107, 255); border-radius: 25px;">Click me!</button>
                    </h4>
                  </div>
                </div>
                <div class="container text-center">
                  <div class="row">
                    <h4 class="col order-5" style="margin-top: 30px;">
                      Lighting effect
                      <button class="btn-click" onclick="startAnimation('animation1')" style="display: block; margin: 0% auto; margin-top: 20px; background-color: #69e971; border-radius: 25px;">Click me!</button>
                    </h4>
                    <h4 class="col order-5" style="margin-top: 30px;">
                      Lighting effect
                      <button class="btn-click" onclick="startAnimation('animation2')" style="display: block; margin: 0% auto; margin-top: 20px; background-color: #bd5df4; border-radius: 25px;">Click me!</button>
                    </h4>
                    <h4 class="col order-5" style="margin-top: 30px;">
                      Lighting effect
                      <button class="btn-click" onclick="startAnimation('animation3')" style="display: block; margin: 0% auto; margin-top: 20px; background-color: #84aacc; border-radius: 25px;">Click me!</button>
                    </h4>
                  </div>
                </div>
                <div class="container text-center">
                  <div class="row">
                    <h4 class="col order-5" style="margin-top: 30px;">
                      Stop
                      <button class="btn-click" onclick="startAnimation('stop')" style="display: block; margin: 0% auto; margin-top: 20px; background-color: #e5f589; border-radius: 25px;">Click me!</button>
                    </h4>
                    <h4 class="col order-5" style="margin-top: 30px;">
                      Toggle Ultrasonic
                      <button class="btn-click" onclick="startAnimation('toggleUltrasonic')" style="display: block; margin: 0% auto; margin-top: 20px; background-color: #ff8c00; border-radius: 25px;">Toggle</button>
                    </h4>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </section>
        <script>
          function startAnimation(animation) {
            var xhr = new XMLHttpRequest();
            xhr.open('GET', '/' + animation, true);
            xhr.send();
          }
        </script>
      </body>
    </html>
  )rawliteral";
  server.send(200, "text/html", html);
}

void handleLightning();
void handleLightningStorm();
void handleThunderStorm();

void handleAnimation1();
void handleAnimation2();
void handleAnimation3();
void handleStop();
void handleToggleUltrasonic(); // New handler for toggling ultrasonic feature
void lightningStormEffect();
void thunderStormEffect();
void lightningFlash(int flashIntensity, int flashDuration, int colorHue);
void preLightningBuildUp();
void rollingThunder();
void postLightningGlow();
void sparkleEffect(int sparkleCount, int sparkleDuration);
void animation1();
void animation2();
void animation3();

// Ultrasonic sensor functions
long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // Distance in cm
  return distance;
}

void handleLightning() {
  Serial.println("Received request for /lightning");
  currentEffect = LIGHTNING;
  server.send(200, "text/plain", "Lightning Effect Started");
}

void handleLightningStorm() {
  Serial.println("Received request for /lightningStorm");
  currentEffect = LIGHTNING_STORM;
  server.send(200, "text/plain", "Lightning Storm Effect Started");
}

void handleThunderStorm() {
  Serial.println("Received request for /thunderStorm");
  currentEffect = THUNDER_STORM;
  server.send(200, "text/plain", "Thunder Storm Effect Started");
}

void handleAnimation1() {
  Serial.println("Received request for /animation1");
  currentEffect = ANIMATION1;
  server.send(200, "text/plain", "Animation 1 Started");
}

void handleAnimation2() {
  Serial.println("Received request for /animation2");
  currentEffect = ANIMATION2;
  server.send(200, "text/plain", "Animation 2 Started");
}

void handleAnimation3() {
  Serial.println("Received request for /animation3");
  currentEffect = ANIMATION3;
  server.send(200, "text/plain", "Animation 3 Started");
}

void handleStop() {
  Serial.println("Received request for /stop");
  currentEffect = NONE;
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  server.send(200, "text/plain", "Animation Stopped");
}

void handleToggleUltrasonic() {
  ultrasonicFeatureActive = !ultrasonicFeatureActive;
  Serial.println(ultrasonicFeatureActive ? "Ultrasonic Feature Activated" : "Ultrasonic Feature Deactivated");
  server.send(200, "text/plain", ultrasonicFeatureActive ? "Ultrasonic Feature Activated" : "Ultrasonic Feature Deactivated");
}

void lightningEffect() {
  int flashCount = random(3, 6);  // Random number of flashes per lightning
  for (int i = 0; i < flashCount; i++) {
    if (currentEffect != LIGHTNING) return;  // Interrupt effect if changed
    int flashDuration = random(50, 200);  // Flash duration
    int flashIntensity = random(150, 255);  // Flash brightness
    fill_solid(leds, NUM_LEDS, CHSV(0, 0, flashIntensity));  // White flash
    FastLED.show();
    delay(flashDuration);
    fill_solid(leds, NUM_LEDS, CRGB::Black);  // Turn off
    FastLED.show();
    delay(flashDuration);
  }
  delay(random(2000, 5000));  // Delay between lightning
}

void lightningStormEffect() {
  preLightningBuildUp();

  int flashCount = random(3, 8);  // Random number of flashes per lightning
  for (int i = 0; i < flashCount; i++) {
    if (currentEffect != LIGHTNING_STORM) return;  // Interrupt effect if changed
    int flashIntensity = random(200, 255);  // Flash brightness
    int flashDuration = random(50, 200);  // Flash duration
    int colorHue = random(160, 200);  // Color hue for lightning (cool colors)
    lightningFlash(flashIntensity, flashDuration, colorHue);
  }

  rollingThunder();
  postLightningGlow();
  sparkleEffect(random(5, 15), random(20, 100));  // Add random sparkles
  delay(random(3000, 8000));  // Delay between lightning storms
}

// Continuation of thunderStormEffect function
void thunderStormEffect() {
  for (int j = 0; j < 5; j++) {  // Loop for multiple thunder flashes
    int flashCount = random(5, 10);  // Random number of flashes per thunder
    for (int i = 0; i < flashCount; i++) {
      if (currentEffect != THUNDER_STORM) return;  // Interrupt effect if changed
      int flashDuration = random(50, 200);  // Flash duration
      int flashIntensity = random(150, 255);  // Flash brightness
      fill_solid(leds, NUM_LEDS, CHSV(160, 255, flashIntensity));  // Vibrant blue flash
      FastLED.show();
      delay(flashDuration);
      fill_solid(leds, NUM_LEDS, CRGB::Black);  // Turn off
      FastLED.show();
      delay(flashDuration);
    }
    delay(random(1000, 3000));  // Delay between thunder sets

    // Add light traveling effect
    for (int k = 0; k < 2; k++) {  // Number of traveling lights
      int travelStart = random(0, NUM_LEDS - 30);  // Random starting point
      for (int l = 0; l < 30; l++) {  // Length of the traveling light
        if (currentEffect != THUNDER_STORM) return;  // Interrupt effect if changed
        leds[travelStart + l] = CHSV(160, 255, 255);
        FastLED.show();
        delay(20);
        leds[travelStart + l] = CRGB::Black;
      }
    }
  }
}

void lightningFlash(int flashIntensity, int flashDuration, int colorHue) {
  fill_solid(leds, NUM_LEDS, CHSV(colorHue, 255, flashIntensity));  // Flash color
  FastLED.show();
  delay(flashDuration);
  fill_solid(leds, NUM_LEDS, CRGB::Black);  // Turn off
  FastLED.show();
  delay(flashDuration / 2);
}

void preLightningBuildUp() {
  int buildUpDuration = random(1000, 3000);
  for (int i = 0; i < buildUpDuration; i += 50) {
    if (currentEffect != LIGHTNING_STORM) return;  // Interrupt effect if changed
    int intensity = map(i, 0, buildUpDuration, 0, 255);
    fill_solid(leds, NUM_LEDS, CHSV(0, 0, intensity));  // Gradual build-up color
    FastLED.show();
    delay(50);
  }
  fill_solid(leds, NUM_LEDS, CRGB::Black);  // Ensure all off before main flash
  FastLED.show();
}

void rollingThunder() {
  int numSegments = random(3, 6);
  for (int i = 0; i < numSegments; i++) {
    if (currentEffect != LIGHTNING_STORM) return;  // Interrupt effect if changed
    int segmentLength = NUM_LEDS / numSegments;
    int startLED = i * segmentLength;
    int endLED = startLED + segmentLength;
    int intensity = random(100, 255);
    for (int j = startLED; j < endLED; j++) {
      leds[j] = CHSV(160, 255, intensity);
    }
    FastLED.show();
    delay(random(100, 300));
    for (int j = startLED; j < endLED; j++) {
      leds[j] = CRGB::Black;
    }
    FastLED.show();
  }
}

void postLightningGlow() {
  int glowDuration = random(1000, 3000);
  for (int i = 0; i < glowDuration; i += 50) {
    if (currentEffect != LIGHTNING_STORM) return;  // Interrupt effect if changed
    int intensity = map(i, 0, glowDuration, 255, 0);
    fill_solid(leds, NUM_LEDS, CHSV(160, 255, intensity));  // Gradual glow color
    FastLED.show();
    delay(50);
  }
  fill_solid(leds, NUM_LEDS, CRGB::Black);  // Ensure all off
  FastLED.show();
}

void sparkleEffect(int sparkleCount, int sparkleDuration) {
  for (int i = 0; i < sparkleCount; i++) {
    if (currentEffect != LIGHTNING_STORM) return;  // Interrupt effect if changed
    int sparklePos = random(NUM_LEDS);
    leds[sparklePos] = CRGB::White;  // Sparkle color
    FastLED.show();
    delay(sparkleDuration);
    leds[sparklePos] = CRGB::Black;  // Turn off sparkle
    FastLED.show();
  }
}

void animation1() {
  // Example animation 1: Chase effect
  for (int i = 0; i < NUM_LEDS; i++) {
    if (currentEffect != ANIMATION1) return;  // Interrupt effect if changed
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(50);
    leds[i] = CRGB::Black;
  }
}

void animation2() {
  // Example animation 2: Rainbow effect
  for (int i = 0; i < 256; i++) {
    if (currentEffect != ANIMATION2) return;  // Interrupt effect if changed
    fill_rainbow(leds, NUM_LEDS, i);
    FastLED.show();
    delay(20);
  }
}

void animation3() {
  // Example animation 3: Theater chase effect
  static uint8_t hue = 0;
  for (int i = 0; i < NUM_LEDS; i += 3) {
    if (currentEffect != ANIMATION3) return;  // Interrupt effect if changed
    leds[i] = CHSV(hue, 255, 192);
  }
  FastLED.show();
  delay(50);
  for (int i = 0; i < NUM_LEDS; i += 3) {
    if (currentEffect != ANIMATION3) return;  // Interrupt effect if changed
    leds[i] = CRGB::Black;
  }
  hue += 1;
}

void setup() {
  Serial.begin(115200);

  // Configure static IP
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // FastLED initialization
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  // Ultrasonic sensor initialization
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Set up server routes
  server.on("/", handleRoot);
  server.on("/lightning", handleLightning);
  server.on("/lightningStorm", handleLightningStorm);
  server.on("/thunderStorm", handleThunderStorm);
  server.on("/animation1", handleAnimation1);
  server.on("/animation2", handleAnimation2);
  server.on("/animation3", handleAnimation3);
  server.on("/stop", handleStop);
  server.on("/toggleUltrasonic", handleToggleUltrasonic); // Route for toggling ultrasonic feature

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (ultrasonicFeatureActive) {
    long distance = readUltrasonicDistance();
    if (distance <= 10) {
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      FastLED.show();
    } else {
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
    }
  }

  switch (currentEffect) {
    case LIGHTNING:
      lightningEffect();
      break;
    case LIGHTNING_STORM:
      lightningStormEffect();
      break;
    case THUNDER_STORM:
      thunderStormEffect();
      break;
    case ANIMATION1:
      animation1();
      break;
    case ANIMATION2:
      animation2();
      break;
    case ANIMATION3:
      animation3();
      break;
    default:
      break;
  }
}