/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - clarifications and/or documentation extension
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stddef.h"
#include "esp8266_init.h"
#include "esp8266_handler.h"
#include "dht11.h"
#include "oled.h"
#include "MQTTPacket.h"
#include "transport.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "timers.h"

#include "mqtt.h"
#include "main.h"

/* This is in order to get an asynchronous signal to stop the sample,
as the code loops waiting for msgs on the subscribed topic.
Your actual code will depend on your hw and approach*/
#include <signal.h>

u32 send_time=10000; //发送一帧的时间 10s
unsigned char BMP2[] ={
0x78,0xFC,0xFE,0xFE,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE,0xFE,0xFE,0xFC,0x78,
0x00,0x00,0x01,0x01,0x0F,0x4F,0xCF,0xCF,0xCF,0xCF,0x4F,0x0F,0x01,0x01,0x00,0x00,/*"C:\Users\BUU\Pictures\off.bmp",11*/
/* (16 X 16 )*/
};
unsigned char BMP1[] ={
0x78,0x78,0x82,0x82,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x82,0x82,0x78,0x78,
0x00,0x00,0x00,0x00,0x07,0xF8,0xF8,0xF8,0xF8,0xF8,0xF8,0x07,0x00,0x00,0x00,0x00,/*"C:\Users\BUU\Pictures\on.bmp",10*/
/* (16 X 16 )*/
};
extern u8 temperature;  	    
extern u8 humidity; 

MqttSta_E mqtt_status;
static uint8_t g_transmit = 0;
MQTT_STATUS_E Mqtt_status_step = MQTT_IDLE;
int toStop = 0;
MQTTString pub_topicString = MQTTString_initializer;
static char MQTT_TOPIC[64] = {0};
static char CLIENT_ID[32]= {0};

xQueueHandle xQueue_MQTT_Recv;  /**< MQTT接收消息队列 */

uint32_t pingRespTickCount;

/**
 ******************************************************************************
 * @brief  Sign接收函数
 * @param  iSignNo sign号
 * @return None.
 ******************************************************************************/
void SignHandler(int iSignNo)
{
    printf("Capture sign no:%d ", iSignNo);
}
/**
 ******************************************************************************
 * @brief  MQTT内部调用
 * @param  sig 无
 * @return None.
 ******************************************************************************/

void cfinish(int sig)
{
    signal(SIGINT, SignHandler);
    toStop = 1;
}

/**
 ******************************************************************************
 * @brief  MQTT内部调用
 * @return None.
 ******************************************************************************/
void stop_init(void)
{
    signal(SIGINT, cfinish);
    signal(SIGTERM, cfinish);
}

/**
 ******************************************************************************
 * @brief  MQTT初始化
 * @return None.
 ******************************************************************************/
void MQTT_Init(void)
{
    ReadChipID(CLIENT_ID, sizeof(CLIENT_ID));
    snprintf(MQTT_TOPIC, sizeof(MQTT_TOPIC), "%s%s", MQTT_TOPIC_BASE, CHIP_ID);
    debug_log("MQTT_TOPIC is: %s", MQTT_TOPIC);
//    MQTT_RB_Init();

    snprintf(MQTT_TOPIC_PUB,sizeof(MQTT_TOPIC_PUB),"%s%s",MQTT_TOPIC_PUBLISH,CHIP_ID);
    pub_topicString.cstring = MQTT_TOPIC_PUB;
    debug_log("publish topic: %s",pub_topicString.cstring);
    stop_init();
}


/**
 ******************************************************************************
 * @brief  MQTT连接服务器
* @return < 0,连接错误，其他 返回socket
 ******************************************************************************/
int MQTT_Connect(void)
{
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    unsigned char buf[200];
    int buflen = sizeof(buf);
    int len = 0;
    char *host = MQTT_SERVER_HOST;
    int port = MQTT_SERVER_PORT;

    debug_log("Sending to hostname:%s,port:%d", host, port);


    data.clientID.cstring = CLIENT_ID;
    data.keepAliveInterval = MQTT_ALIVE_INTERVAL;
    data.cleansession = 1;
    data.username.cstring = MQTT_USER;
    data.password.cstring = MQTT_PASSWORD;

    len = MQTTSerialize_connect(buf, buflen, &data);

//	for(uint8_t i=0;i<len;i++)
//		printf("%02x",buf[i]);
//	printf("\r\n");
    transport_sendPacketBuffer(mqtt_status.socket, buf, len);

    /* wait for connack */
    if (MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
    {
        unsigned char sessionPresent, connack_rc;

        if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
        {
            debug_log("Unable to connect, return code %d", connack_rc);
            goto exit;
        }
    }
    else
        goto exit;

    debug_log("Connected to MQTT server");
    return mqtt_status.socket;

exit:
//    transport_close(mqtt_status.socket);
    return -1;
}

/**
 ******************************************************************************
 * @brief  订阅主题
 * @param  topic_str 主题名
 * @return the length of the serialized data.  <= 0 indicates error
 ******************************************************************************/
int MQTT_Subscribe(char* topic_str)
{
    MQTTString topicString = MQTTString_initializer;

    int len = 0;
    unsigned char buf[200];
    int buflen = sizeof(buf);
    int msgid = 1;
    int req_qos = 0;
    int rc;

    topicString.cstring = topic_str;
//    debug_log("MQTT_SubscribeTopic:%s",topicString.cstring);
    len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);

    transport_sendPacketBuffer(mqtt_status.socket, buf, len);
//    vTaskDelay(2000 / portTICK_RATE_MS);
    if ((rc = MQTTPacket_read(buf, buflen, transport_getdata)) == SUBACK) 	/* wait for suback */
    {
        unsigned short submsgid;
        int subcount;
        int granted_qos;

        rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
        if (granted_qos != 0)
        {
            debug_log("granted qos != 0, %d", granted_qos);
            goto exit;
        }
    }
    else
    {
        debug_log("MQTT_Subscribe Error %d", rc);
        goto exit;
    }
exit:
    return rc;
}


/**
 ******************************************************************************
 * @brief 读取主题，阻塞方式
 * @return the length of the serialized data.  <= 0 indicates error
 ******************************************************************************/
int MQTT_Read(void)
{
    int rc;
    static unsigned char buf[200] = {0};
    int buflen = sizeof(buf);
    while (!toStop)
    {
        /* transport_getdata() has a built-in 1 second timeout, your mileage will vary */
        rc = MQTTPacket_read(buf, buflen, transport_getdata);
//        debug_log("read rc = %d",rc);
        if (rc == PUBLISH)
        {
            debug_log("----------PUBLISH----------");
            MQTT_Recv_t mqtt_recv_t;
            MQTTDeserialize_publish(&mqtt_recv_t.dup, &mqtt_recv_t.qos, &mqtt_recv_t.retained, &mqtt_recv_t.msgid, &mqtt_recv_t.receivedTopic,
                                    &mqtt_recv_t.payload_in, &mqtt_recv_t.payloadlen_in, buf, buflen);
            xQueueSend(xQueue_MQTT_Recv,  &mqtt_recv_t,  5);
//            debug_log("\r\nmessage arrived %d, %s", mqtt_recv_t.payloadlen_in, mqtt_recv_t.payload_in);
        }
        else if(rc == PINGRESP)
        {
            debug_log("Recv MQTT PINGRESP");
            pingRespTickCount = 0;
        }
        else if(rc == DISCONNECT)
        {
            debug_log("MQTT Disconnect");
            goto exit;
        }
        else if(rc == -1)
        {
            debug_log("MQTT Disconnect2");
            goto exit;
        }
        else
        {
            debug_log("MQTT %d", rc);
        }
        memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
        vTaskDelay(20 / portTICK_RATE_MS);
    }
exit:
    return rc;
}

/**
 ******************************************************************************
 * @brief 发布主题
 * @param dup integer - the MQTT dup flag
 * @param qos integer - the MQTT QoS value
 * @param retained integer - the MQTT retained flag
 * @param packetid integer - the MQTT packet identifier
 * @param topicName MQTTString - the MQTT topic in the publish
 * @param payload byte buffer - the MQTT publish payload
 * @param payloadlen integer - the length of the MQTT payload
 * @return the length of the serialized data.  <= 0 indicates error
 ******************************************************************************/
int MQTT_Publish(unsigned char dup, int qos, unsigned char retained, unsigned short packetid,
                 MQTTString topicName, char* payload, int payloadlen)
{
    int len, rc;
    unsigned char buf[256];
    int buflen = 256;
//    debug_log("MQTT_Publish");
    if(MQTT_GetConnected())
    {
        len = MQTTSerialize_publish(buf, buflen, dup, qos, retained, packetid, topicName, payload, payloadlen);
        //len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
        rc  = transport_sendPacketBuffer(mqtt_status.socket, buf, len);
        return rc;
    }
    else
    {
        debug_log("MQTT Disconneted");
        return -1;
    }
}

/**
 ******************************************************************************
 * @brief  MQTT发送心跳，采用定时触发
 * @param  xTimer
 * @retval None.
 ******************************************************************************/
//void MQTT_PingREQ(xTimerHandle xTimer)
void MQTT_PingREQ(void)
{
    unsigned char buf[200];
    int buflen = sizeof(buf);
    int len;

    if(MQTT_GetConnected())
    {
        printf("PING REQ");

        len = MQTTSerialize_pingreq(buf, buflen);
        transport_sendPacketBuffer(mqtt_status.socket, buf, len);
    }
}

/**
 ******************************************************************************
 * @brief  获取连接状态
 * @retval 连接状态
 ******************************************************************************/
uint8_t MQTT_GetConnected(void)
{
    uint8_t i;

    if(g_esp_status_t.esp_net_work_e == ESP_NETWORK_SUCCESS)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    return i;
}
/**
******************************************************************************
* @brief  MQTT
* @retval None.
******************************************************************************/
void MqttXqueueGreat(void)
{
    xQueue_MQTT_Recv = xQueueCreate(5, sizeof(MQTT_Recv_t));
    if(xQueue_MQTT_Recv == NULL)
    {
        debug_log("xQueue_MQTT_Recv ERROR");
        while(1);
    }
}

void MqttHandle(void)
{
    MQTT_Init();
    MqttXqueueGreat();
}
void vMQTT_Handler_Task(void *ptr)
{
    int rc = -1;
    while(1)
    {
        switch(Mqtt_status_step)
        {
            case MQTT_IDLE:
            {
                MqttHandle();
                memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
                Mqtt_status_step = MQTT_CONNECT;
                g_transmit = 0;
                debug_log("mqtt satus is idle");
            }
            break;
            case MQTT_CONNECT:
            {
                if(g_esp_status_t.esp_hw_status_e ==ESP_HW_CONNECT_OK )
                {
                    lastSendOutTime =  xTaskGetTickCount();
                    rc = MQTT_Connect();
                    if(rc == -1)
                    {
                        vTaskDelay(200 / portTICK_RATE_MS);
                        continue;
                    }
                    else
                    {
                        /* 如果连接成功，则进入订阅状态*/
                        g_esp_status_t.esp_net_work_e = ESP_NETWORK_SUCCESS;
                        Mqtt_status_step = MQTT_SUBSCRB;
                        memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
                    }
                }
                vTaskDelay(200 / portTICK_RATE_MS);
            }
            break;
            case MQTT_SUBSCRB:
            {
                lastSendOutTime =  xTaskGetTickCount();
                rc = MQTT_Subscribe(MQTT_TOPIC);
                if(rc != -1)
                {
                    /* 如果订阅成功，则进入发布状态*/
                    Mqtt_status_step = MQTT_PUBLSH;
                    memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
                }
                else
                {
                    debug_log("MQTT Subscribe Error");
                }
                vTaskDelay(200 / portTICK_RATE_MS);
            }
            break;
            case MQTT_PUBLSH:
            {
                g_transmit = 1;
                MQTT_Read();
                if(g_esp_status_t.esp_net_work_e != ESP_NETWORK_SUCCESS)
                {
                    debug_log("mqtt connect is failed");
                }
            }
            break;
            default:
                break;
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


void vMQTT_Recive_Task(void *ptr)//订阅的消息写在这里
{
    MQTT_Recv_t mqtt_recv_t;
    signed portBASE_TYPE pd;
    while(1)
    {
        pd = xQueueReceive(xQueue_MQTT_Recv, &mqtt_recv_t, 20 / portTICK_RATE_MS);
        if(pd != NULL)
        {
            char buff[100] = {0};
            int i = 0;
						int k = 0;
//            printf("mqtt_recv_t:%s\r\n",mqtt_recv_t.payload_in);
            memcpy(buff,mqtt_recv_t.payload_in,mqtt_recv_t.payloadlen_in);
            debug_log("mqtt recv data:");
            for(i = 0; i < mqtt_recv_t.payloadlen_in; i++)
            {
                debug_log("buff[%d] = %c",i,buff[i]);
            }

							if(buff[2]=='n' && buff[3]=='u' && buff[4]=='m')//接收到订阅的消息								
              {
								if(buff[8]=='1')//收到按钮一
								{
									if(buff[k+19]=='1')//按钮一开
										{LED0_ON;
										OLED_DrawBMP(30,6,46,8,BMP2);  } //LEDON
								  if(buff[k+19]=='0')//按钮一关
									  {LED0_OFF;OLED_DrawBMP(30,6,46,8,BMP1); }  //LEDOFF
								}
								if(buff[8]=='2')//收到按钮2
								{
									if(buff[k+19]=='1')//按钮2开
										{LED2_ON;OLED_DrawBMP(50,6,66,8,BMP2); }  //LEDON
								  if(buff[k+19]=='0')//按钮2关
									  {LED2_OFF;OLED_DrawBMP(50,6,66,8,BMP1); }  //LEDOFF
								}
								if(buff[8]=='4')//收到按钮3
								{
									if(buff[k+19]=='1')//按钮3开
										{LED1_ON;OLED_DrawBMP(70,6,86,8,BMP2); }  //LEDON
								  if(buff[k+19]=='0')//按钮3关
									  {LED1_OFF;OLED_DrawBMP(70,6,86,8,BMP1); }  //LEDOFF
								}
								if(buff[8]=='3')//收到按钮4
								{
									if(buff[k+19]=='1')//按钮4开
									{
									//LED3_ON;
									OLED_DrawBMP(90,6,106,8,BMP2); } //LEDON
								  if(buff[k+19]=='0')//按钮4关
									{
										//LED3_OFF;
										OLED_DrawBMP(90,6,106,8,BMP1);}  //LEDOFF}
									
								}
								
						}
						
						memset(buff,0,sizeof(buff));//清空数组

							
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}
static char meassage[200];
void MqttTranmitTask(void *ptr)//发布消息在这里写.把所有消息发布成一帧，留给后面去解析
{
    static uint8_t i = 0;
	  float temp,humi,light,co2;
	  
	  
    while(1)
    {
        if(g_esp_status_t.esp_net_work_e == ESP_NETWORK_SUCCESS)
        {
            if(g_transmit)
            {
                i++;
							  //temp++;humi++;light++;co2++;
							  sprintf(meassage,"{\"temperature\":%d,\"humidity\":%d,\"light\":%.1f,\"pressure\":%.1f}",temperature,humidity,light,co2);
                //MQTT_Publish(0, 0, 0, 0, pub_topicString, &i, sizeof(i));
							  MQTT_Publish(0, 0, 0, 0, pub_topicString, meassage, sizeof(meassage));
                printf("send to server data is %s\r\n",meassage);
							  //LED0_ON;
            }
        }
        vTaskDelay(send_time / portTICK_PERIOD_MS);
    }
}



