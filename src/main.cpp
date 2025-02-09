#include "main.h"

void initPeripherals()
{
    SystemLog.addEvent(F("Initializing peripherals"));
    EEPROM.begin(EEPROM_SIZE);

    sd_card.initCard();

    if (sd_card.getCardHealthy())
    {
        SystemLog.connectSdCard();
        SensorLog.connectSdCard();
    }
    SystemCamera.init();
    network.init();
}

void initSensors()
{
    i2c.begin();
    i2c.scan();
    i2c.initMPU6050();
    i2c.initHMC5883L();
    i2c.initMS5611();
}

void initTasks()
{
    tasks.init();
    tasks.createTimelapseTask();
    tasks.createTelemetryTask();
    tasks.createSdCardHealthCheckTask();

    i2c.createHMC5883LTask();
    i2c.createMPU6050Task();
    i2c.createMS5611Task();
}

void setup()
{
    SystemLog.addEvent(F("Setup started"));
#if ENABLE_SERIAL_LOGS
    Serial.begin(SERIAL_BAUD);
    Serial.setDebugOutput(true);
    SystemLog.addEvent(F("Serial logs enabled"));
#endif
#if DISABLE_BROWNOUT
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
    SystemLog.addEvent(F("Brownout detector disabled"));
#endif
    initPeripherals();
    initSensors();
    initTasks();
    SystemLog.addEvent(F("Setup complete"));
}

void loop()
{
    esp_task_wdt_reset();

    delay(1000);
}
