#ifndef __MQTT_H__
#define __MQTT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "MQTTPacket.h"
#include "transport.h"


#define MQTT_SERVER_HOST            "www.mengke.fun"		//实际代码请修改成自己的服务器域名
#define MQTT_SERVER_PORT            1883				//实际代码请修改成自己的服务器端口
#define MQTT_ALIVE_INTERVAL         30					//实际代码请修改成自己的心跳时间（秒）
#define MQTT_USER                   "****"				//实际代码请修改成自己的用户名
#define MQTT_PASSWORD               "****"				//实际代码请修改成自己的密码
#define MQTT_TOPIC_BASE             "****"		//实际代码请修改成自己的订阅主题
#define MQTT_TOPIC_PUBLISH			    "****"	//实际代码请修改成自己的发布主题
	
#define CHIP_ID			"1"	        //板子的ID,不同的板子烧录不同ID，后期采用EEPROM控制

static char MQTT_TOPIC_PUB[64] = {0};
typedef struct
{
    unsigned char dup;
    int qos;
    unsigned char retained; 
    unsigned short msgid;
    int payloadlen_in;
    unsigned char* payload_in;
    MQTTString receivedTopic;
} MQTT_Recv_t;

extern xQueueHandle xQueue_MQTT_Recv;

/*Mqtt状态结构体*/
typedef struct
{
	uint8_t connect;
	int socket;
}MqttSta_E;
extern MqttSta_E mqtt_status;

extern uint32_t pingRespTickCount;

void MQTT_Init(void);
int MQTT_Connect(void);
void vMQTT_Handler_Task(void *ptr);
void vMQTT_Recive_Task(void *ptr);
void MqttTranmitTask(void *ptr);


#ifdef __cplusplus
}
#endif

#endif /*__MQTT_CONFIG_H__*/
