//#pragma once 
#include <Setup\Setup.h>

float battery_volts;  
int battery_percentage; 
int adc_value; 
const float v_min = 2.0;
const float v_max = 2.7;          

//Spannung an Batterie in Volt und Umrechnung in Prozent
void getBattery() {
    adc_value = analogRead(V_BAT); 
    battery_volts = (3.3/4096.0)*1.47 * float(adc_value);  // Referenzspannung/ MAX ADC * Spannungsteiler

    //Batterie-Status in % 
    battery_percentage = (battery_volts - v_min)/(v_max - v_min) * 100;
    if (battery_percentage > 100) battery_percentage = 100;
    if (battery_percentage < 0) battery_percentage = 0;

    //Not-Aus 
    if(battery_percentage < 10) {
        display->clearDisplay();
        display->setTextSize(1);
        display->setTextColor(SSD1306_WHITE);
        display->print("Battery low!!!"); 
        Buzzer_beep(4000, 3);
        delay(1000);
        digitalWrite(POWER_ENABLE, LOW);
    }
}

// Function to draw battery status on OLED
void drawBatteryStatus() {
    display->clearDisplay();

    // Draw battery outline
    display->drawRect(0, 0, 34, 16, SSD1306_WHITE);
    display->fillRect(34, 4, 4, 8, SSD1306_WHITE);

    // Draw battery level
    int width = (battery_percentage * 28) / 100;
    display->fillRect(2, 2, width, 12, SSD1306_WHITE);

    // Display battery battery_percentage
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(40, 4);
    display->print(battery_percentage);
    display->print("%   ");
    display->print(battery_volts);
    display->print("V");

    display->display();
    ble->println("Voltage: " + String(battery_volts) + "V");
    ble->println("Battery: " + String(battery_percentage) + "%");
}