#ifndef IOT_SENSOR_NODE_CONFIG_H
#define IOT_SENSOR_NODE_CONFIG_H

#include "../winc/driver/include/m2m_wifi.h"
#include "config/conf_winc.h"

// <h> Application Configuration

// <o> Send Interval <0-100000>
// <i> Send interval in seconds
// <id> application_send_interval
#define CFG_SEND_INTERVAL 1

// <o> Timeout <0-100000>
// <i> Timeout
// <id> application_timeout
#define CFG_TIMEOUT 5000

// </h>

// <h> Cloud Configuration

// <s> project id
// <i> Google Cloud Platform project id
// <id> project_id
#define CFG_PROJECT_ID "avr-iot"

// <s> project region
// <i> Google Cloud Platform project region
// <id> project_region
#define CFG_PROJECT_REGION "us-central1"

// <s> registry id
// <i> Google Cloud Platform registry id
// <id> registry_id
#define CFG_REGISTRY_ID "AVR-IOT"

// <s> mqtt host
// <i> mqtt host address
// <id> mqtt_host
#define CFG_MQTT_HOST "mqtt.googleapis.com"

// </h>

// <h> Debug Configuration

// <q> Enable debug messages:
// <i> Check to enable debug messages
// <id> debug_msg
#define CFG_DEBUG_MSG 0

// </h>

// <h> CLI Support

// <q> Enable CLI:
// <i> Check to enable cli
// <id> enable_cli
#define CFG_ENABLE_CLI 1

// </h>

#endif // IOT_SENSOR_NODE_CONFIG_H
