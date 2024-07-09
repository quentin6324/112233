
#include "ohos_init.h"
#include "wifi_device.h"
#include "mqtt_client.h"
#include "dht11.h"
#include "iot_adc.h"

#define TEMP_THRESHOLD 50.0
#define HUMIDITY_THRESHOLD 80.0
#define SMOKE_THRESHOLD 300

void WifiConnect(void) {
    WifiDeviceConfig apConfig = {0};
    strcpy(apConfig.ssid, "your_ssid");
    strcpy(apConfig.preSharedKey, "your_password");
    apConfig.securityType = WIFI_SEC_TYPE_PSK;
    WifiConnect(&apConfig);
}

void SendAlert(const char* message) {
    MQTTClient mqttClient = MQTTClientInit("broker_address", "client_id");
    MQTTClientConnect(mqttClient, "username", "password");
    MQTTClientPublish(mqttClient, "alert/topic", message);
    MQTTClientDisconnect(mqttClient);
    MQTTClientDestroy(mqttClient);
}

void MonitorTask(void) {
    float temperature = 0.0;
    float humidity = 0.0;
    uint16_t smoke = 0;

    while (1) {
        if (DHT11_ReadData(&temperature, &humidity) == DHT11_OK) {
            printf("Temperature: %.2f, Humidity: %.2f\n", temperature, humidity);
        }

        smoke = MQ2_ReadData();
        printf("Smoke: %d\n", smoke);

        if (temperature > TEMP_THRESHOLD || humidity > HUMIDITY_THRESHOLD || smoke > SMOKE_THRESHOLD) {
            SendAlert("Fire Alert! Check the system.");
        }

        osDelay(5000);
    }
}

void AppInit(void) {
    WifiConnect();
    osThreadAttr_t attr = {
        .name = "MonitorTask",
        .stack_size = 4096,
        .priority = osPriorityNormal,
    };
    osThreadNew((osThreadFunc_t)MonitorTask, NULL, &attr);
}

SYS_RUN(AppInit);
