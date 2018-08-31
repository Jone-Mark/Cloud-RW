#ifndef __MQTT_H__
#define __MQTT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "MQTTPacket.h"
#include "transport.h"


#define MQTT_SERVER_HOST            "www.mengke.fun"		//ʵ�ʴ������޸ĳ��Լ��ķ���������
#define MQTT_SERVER_PORT            1883				//ʵ�ʴ������޸ĳ��Լ��ķ������˿�
#define MQTT_ALIVE_INTERVAL         30					//ʵ�ʴ������޸ĳ��Լ�������ʱ�䣨�룩
#define MQTT_USER                   "****"				//ʵ�ʴ������޸ĳ��Լ����û���
#define MQTT_PASSWORD               "****"				//ʵ�ʴ������޸ĳ��Լ�������
#define MQTT_TOPIC_BASE             "****"		//ʵ�ʴ������޸ĳ��Լ��Ķ�������
#define MQTT_TOPIC_PUBLISH			    "****"	//ʵ�ʴ������޸ĳ��Լ��ķ�������
	
#define CHIP_ID			"1"	        //���ӵ�ID,��ͬ�İ�����¼��ͬID�����ڲ���EEPROM����

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

/*Mqtt״̬�ṹ��*/
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
