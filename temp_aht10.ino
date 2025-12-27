#include <Wire.h>
#include <U8glib.h>
#include <Adafruit_AHTX0.h>

// --- AHT10 Sensor Setup ---
Adafruit_AHTX0 aht;

// --- OLED Display Setup (U8glib) ---
// Constructor for I2C OLED (SSD1306 128x64). 
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

// Global variables to store the sensor readings
float temperatureC = 0.0;
float humidity = 0.0;
float feelsLikeC = 0.0;
bool sensorError = false;


// --- UTILITY FUNCTION: Calculate Heat Index (Required for Feels Like) ---
float calculateHeatIndexC(float tempC, float rh) {
  // 1. Convert Celsius to Fahrenheit
  float tempF = (tempC * 9.0 / 5.0) + 32.0;

  // 2. Use a simplified Heat Index formula (accurate for T < 80F)
  float hiF = 0.5 * (tempF + 61.0 + ((tempF - 68.0) * 1.2) + (rh * 0.094));
  
  // 3. Convert the result back to Celsius
  return (hiF - 32.0) * 5.0 / 9.0;
}


// --- Drawing Function (Unit Size Increased) ---
void draw() {
  u8g.setFont(u8g_font_unifont); 

  if (sensorError) {
    u8g.setFont(u8g_font_fub20n);
    u8g.drawStr(0, 30, "SENSOR ERROR");
    u8g.drawStr(0, 50, "CHECK AHT");
    return;
  }
  
  // --- Line 1: Temperature (Top, Largest Font) ---
  u8g.setFont(u8g_font_fub25n); 
  // ONLY use the number string here
  String tempNumStr = String(temperatureC, 1); 
  
  // Center the large temperature number
  u8g_uint_t temp_num_width = u8g.getStrWidth(tempNumStr.c_str());
  u8g_uint_t temp_x = (u8g.getWidth() - temp_num_width) / 2;
  
  u8g.drawStr(temp_x, 30, tempNumStr.c_str());
  
  // --- FIX: Manually draw the °C symbol using the slightly larger unifont ---
  u8g.setFont(u8g_font_10x20);
  u8g_uint_t unit_x = temp_x + temp_num_width;
  u8g_uint_t unit_y = 30;

  // Draw the degree symbol (small circle, manually adjusted position)
  u8g.drawCircle(unit_x + 5, unit_y - 25, 3, U8G_DRAW_ALL); 
  
  // Draw the Celsius 'C'
  u8g.drawStr(unit_x + 10, unit_y - 5, "C"); // Adjusted Y position for better alignment
  
  // ------------------------------------------------------------------

  // --- Line 2: Humidity (Middle, Smaller Font) ---
  u8g.setFont(u8g_font_7x13); 
  String humiStr = "Humidity: " + String(humidity, 1) + " %"; 
  
  u8g.drawStr(2, 48, humiStr.c_str());
  
  // --- Line 3: Feels Like Temperature (Bottom) ---
  String feelsStr = "Feels like: " + String(feelsLikeC, 1) + " C";
  
  u8g.drawStr(2, 63, feelsStr.c_str());
  
}


// --- Setup Function ---
void setup() {
  Serial.begin(115200);

  if (!aht.begin()) {
    Serial.println("AHT10 sensor not found. Check wiring.");
    sensorError = true;
  } else {
    Serial.println("AHT10 sensor found.");
    delay(100); 
  }

  // Initialize OLED and show a startup message
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(0, 20, "Sensor Ready"); 
  } while (u8g.nextPage());
  delay(1000);
}

// --- Loop Function ---
void loop() {
  sensors_event_t humidity_event, temp_event;
  int retries = 0;
  bool validRead = false;

  // --- 1. Read Sensor Data ---
  while (retries < 3 && !validRead) {
    aht.getEvent(&humidity_event, &temp_event);

    if (!isnan(temp_event.temperature) && 
        !isnan(humidity_event.relative_humidity) && 
        humidity_event.relative_humidity >= 0 && 
        humidity_event.relative_humidity <= 100) {
      
      temperatureC = temp_event.temperature;
      humidity = humidity_event.relative_humidity;
      sensorError = false;
      validRead = true;
      
      // --- 2. Calculate Feels Like Temperature ---
      feelsLikeC = calculateHeatIndexC(temperatureC, humidity);

    } else {
      retries++;
      if (retries < 3) delay(100);
    }
  }

  if (!validRead) {
    Serial.println("Failed to get valid sensor data.");
    sensorError = true;
  }

  // --- 3. Update Display ---
  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage());

  delay(1000);
}
