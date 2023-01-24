#include <Arduino.h>
#include "teensy_can.h"
#include "GWP-Charger.h"
#include "virtualTimer.h"

#if defined(ARDUINO_TEENSY40) || defined(ARDUINO_TEENSY41)
#include "teensy_can.h"
// The bus number is a template argument for Teensy: TeensyCAN<bus_num>
TeensyCAN<1> can_bus{};
#endif

GWPCharger charger(can_bus);

// Make a VirtualTimerGroup to add your timers to
VirtualTimerGroup timer_group{};

void ten_ms_task()
{
  /**
   * You can use and set the CANSignals as if they were SignalType
   *
   */
  // float_tx_signal = 5.195f;
  // uint8_t_tx_signal = 132;
  // bool_tx_signal = true;
  // millis_tx_signal = millis();

  // The CANRXMessage automatically gets updated on message reception from the interrupt. Not: in order for this to
  // work, you must periodically call Tick() on the can_bus
  charger.Tick(millis());
  can_bus.Tick();
  Serial.print("Received output voltage: ");
  Serial.println(charger.GetOutputVoltage());
  Serial.print("Received output current: ");
  Serial.println(charger.GetOutputCurrent());
  Serial.print("Received primary temperature: ");
  Serial.println(charger.GetPrimaryTemperature());
  Serial.print("Received secondary temperature: ");
  Serial.println(charger.GetSecondaryTemperature());
  Serial.print("Received input voltage: ");
  Serial.println(charger.GetInputVoltage());
  Serial.print("Received input current: ");
  Serial.println(charger.GetInputCurrent());
  Serial.print("Received power reference: ");
  Serial.println(charger.GetPowerReference());
  Serial.print("Received available power: ");
  Serial.println(charger.GetAvailablePower());
}

void setup()
{
  can_bus.Initialize(ICAN::BaudRate::kBaud500K);

  // You can create a new timer in a VirtualTimerGroup using the AddTimer(function, time) function
  timer_group.AddTimer(10, ten_ms_task);

  Serial.begin(9600);
  Serial.println("Started");
}

void loop()
{
  timer_group.Tick(millis());
}