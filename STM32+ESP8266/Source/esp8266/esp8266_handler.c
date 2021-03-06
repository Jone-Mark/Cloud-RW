#include "string.h"
#include "bsp.h"
#include "esp8266_init.h"
#include "esp8266_handler.h"
#include "./delay/bsp_delay.h"
//#define USER_PRINT_BASE( format, args... )  printf( format, ##args )
//#define debug_log( format, args... ) USER_PRINT_BASE( "%s_%d:"format"\r\n",__FILE__,__LINE__, ##args )

#define ESP_ERROR_CNT			10

/* SmartConfigrationTime(MS) */
#define SMART_CONFIG_TIME		10000

/* 串口发送超时(MS) */
#define SEND_TIMEOUT_TIME		200

/* 串口发送间隔(MS) */
#define USART_SEND_INTERVAL		1000

/* 发送数据命令 */
#define SendDataCmd						("AT+CIPSEND=%d\r\n")

/* MQTT订阅数据缓存 */
uint8_t mqttSubscribeData[200] = {0};

/* MCU ID */
static char LONG_CLIENT_ID[32]= {0};
TickType_t lastSendOutTime = 0;

NET_DEVICE_INFO_T netDeviceInfo_t = {{0}, NULL};

/* ESP8266 连接状态初始化 */
ESP_STATUS_T g_esp_status_t = {ESP_HW_RESERVE_0,ESP_NETWORK_FAILED};

/* ESP8266 错误次数初始化 */
static ESP_ERROR_T esp_error_t={0,0,0,0};

/* 串口接收数据缓冲区 */
char gUsartReciveLineBuf[200] = {0};

void printf_buf(const uint8_t *bufAddr, uint16_t bufLen)
{
    uint16_t i;
    for(i = 0; i<bufLen; i++)
    {
        debug_log("buf[%d] = 0x%02x ",i,bufAddr[i]);
    }
}
void printf_buf_char(uint8_t *bufAddr, uint16_t bufLen)
{
    uint16_t i;
    for(i = 0; i<bufLen; i++)
    {
        USART1_Send(bufAddr,bufLen);
    }
}

//==========================================================
//	函数名称：	net_device_send_cmd
//
//	函数功能：	向网络设备发送一条命令，并等待正确的响应
//
//	入口参数：	cmd：需要发送的命令
//				res：需要检索的响应
//
//	返回参数：	返回连接结果
//
//	说明：		0-成功		1-失败
//==========================================================
uint8_t net_device_send_cmd(char *cmd, char *res)
{
    uint8_t timeout = SEND_TIMEOUT_TIME;
    USART2_Send(cmd,strlen(cmd));
    netDeviceInfo_t.cmd_hdl = res;
    if(res == NULL)
    {
        return 0;
    }
    while(netDeviceInfo_t.cmd_hdl != NULL && --timeout != 0)
    {
        vTaskDelay(10 / portTICK_RATE_MS);
    }
    if(timeout > 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//==========================================================
//	函数名称：	ReconfigWIFI
//
//	函数功能：	重新配网
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		成功：立即重启		失败：超时重启
//==========================================================
void ReconfigWIFI(void)
{
    static uint16_t ConfigCnt = 0;
    printf("---------------long------------------\r\n");
    g_esp_status_t.esp_hw_status_e = ESP_HW_RECONFIG;
    g_esp_status_t.esp_net_work_e = ESP_NETWORK_FAILED ;

    LED0_OFF;
    if(net_device_send_cmd("AT+CWSMARTSTART=2\r\n","OK") == 0)
    {
        debug_log("Start smart config configure");
    }

    while(1)
    {
        if(strstr(netDeviceInfo_t.cmd_resp,"SMART SUCCESS") != NULL)//配网成功
        {
            /* 点亮三色灯 */

            debug_log("smart config ok");
            /* 5秒后重启，等待ESP8266给手机反馈配网结果*/
            vTaskDelay(5000 / portTICK_RATE_MS);
            NVIC_SystemReset();//重启
        }
        else
        {
            /* 10秒没成功 */
            if(ConfigCnt++ >= SMART_CONFIG_TIME)
            {
                /* 超时重启 */
                NVIC_SystemReset();
            }
        }
        //NET_LED_TOGGLE;
        vTaskDelay(50 / portTICK_RATE_MS);
    }
}

/**
******************************************************************************
**	函数名称：	ConnectTcp
**
**	函数功能：	连接TCP服务器
**
**	入口参数：	无
**
**	返回参数：	连接结果，成功1，失败0
**
**	说明：		命令处理成功则将指针置NULL
******************************************************************************
**/
uint8_t ConnectTcp(void)
{
    uint8_t tmp = 0;
    if(net_device_send_cmd("AT+CIPSTART=\"TCP\",\"47.106.209.211\",1883\r\n","OK") == 0)
    {
        tmp = 1;
        debug_log("connect server tcp ok!");
    }
    else
        tmp = 0;
    return tmp;
}
/**
******************************************************************************
**	函数名称：	esp8266_cmd_handle
**
**	函数功能：	检查命令返回是否正确
**
**	入口参数：	cmd：需要发送的命令
**
**	返回参数：	无
**
**	说明：		命令处理成功则将指针置NULL
******************************************************************************
**/
void esp8266_cmd_handle(char *cmd)
{
    if(strstr(cmd,netDeviceInfo_t.cmd_hdl) != NULL)
    {
        netDeviceInfo_t.cmd_hdl = NULL;
    }
    memcpy(netDeviceInfo_t.cmd_resp,(const char*)cmd,sizeof(netDeviceInfo_t.cmd_resp));
    memset(gUsartReciveLineBuf,0,sizeof(gUsartReciveLineBuf));
}

/**
 ******************************************************************************
 * @brief  检查ESP8266连接状态
 * @return 状态值
 ******************************************************************************
**/
ESP_HW_STATUS_E check_esp8266_status(void)
{
    uint8_t tmp = 255;

    if(net_device_send_cmd("AT+CIPSTATUS\r\n", "OK") == 0)			//发送状态监测
    {
        if(strstr(netDeviceInfo_t.cmd_resp, "STATUS:2"))				//获得IP
        {
            tmp =  2;
					SetLedConnectNet(LED_NETWORK_NOK);
        }
        else if(strstr(netDeviceInfo_t.cmd_resp, "STATUS:3"))			//建立连接
        {
            tmp = 3;
					  SetLedConnectNet(LED_NETWORK_OK);
        }
        else if(strstr(netDeviceInfo_t.cmd_resp, "STATUS:4"))			//失去连接，断开tcp连接
        {
            tmp = 4;
					SetLedConnectNet(LED_NETWORK_NOK);
        }
        else if(strstr(netDeviceInfo_t.cmd_resp, "STATUS:5"))			//物理掉线，关闭路由器电源，无WIFI信号
        {
            tmp = 5;
	
        }
        else if(strstr(netDeviceInfo_t.cmd_resp, "CLOSE"))
        {
            tmp = 4;
					SetLedConnectNet(LED_NETWORK_NOK);
        }
        else
        {
            //nothing;
					SetLedConnectNet(LED_NETWORK_NOK);
        }
    }
    else
    {
        if(strstr(netDeviceInfo_t.cmd_resp,"busy s...") != NULL)
            tmp = 6;
        else
            tmp = 7;
    }
    return (ESP_HW_STATUS_E)tmp;
}

int MQTT_RB_Read(uint8_t *buf, uint16_t len)
{
    uint8_t i;
    static uint8_t oldlen;
//    debug_log("read data lenth = %d",len);
    while(1)
    {
        if(mqttSubscribeData[0] == 0)
        {
            oldlen = 0;
			
			/* 这里用于tcp掉线,MQTT重连服务器 */
			if(g_esp_status_t.esp_hw_status_e==ESP_HW_CONNECT_OK && g_esp_status_t.esp_net_work_e==ESP_NETWORK_FAILED 
				&& esp_error_t.err_esp_network > 5)
			{
				esp_error_t.err_esp_network = 0;
				return 0;
			}
            vTaskDelay(20 / portTICK_RATE_MS);
        }
        else
        {
            break;
        }
    }
//	debug_log("---------read len = %d",len);
    for(i = 0; i < len; i++)
    {
        *(buf + i) = mqttSubscribeData[i+oldlen];
//		debug_log("%02x",mqttSubscribeData[i+oldlen]);
    }
    oldlen+=len;
    return i;
}


/**
 ******************************************************************************
 * @brief  处理网络返回数据
 * @param  Dataptr 缓冲区指针.
 * @return 状态值,成功0，失败1
 ******************************************************************************
**/
static uint8_t Handle_Internet_Data(char *Dataptr)
{
    uint8_t tmp = 255;
    char* ptrMao = NULL;
    /* 服务器返回成功 */
    ptrMao = strstr(Dataptr,":");
    if(ptrMao != NULL)
    {
        memset(mqttSubscribeData,0,sizeof(mqttSubscribeData));
        memcpy(mqttSubscribeData,(ptrMao+1),sizeof(mqttSubscribeData));
        memset(gUsartReciveLineBuf,0,sizeof(gUsartReciveLineBuf));
    }
    return tmp;
}

/**
 ******************************************************************************
 * @brief  上传数据到服务器
 * @param  无
 * @return 无
 ******************************************************************************
**/
//static void updata_senosr_data(void)
//{
//    char buf[140];
//    float sbuf[2];
//    sbuf[0] = sht20Info.tempreture;
//    sbuf[1] = sht20Info.humidity;
//    char sendData[20];

//    sprintf(sendData,(char *)SendDataCmd,strlen(buf));
////    debug_log("%s",buf);
//    if(netDeviceInfo_t.netWork)
//    {
//        if(net_device_send_cmd(sendData,">") == 0)
//        {
//            USART2_Send(buf,strlen(buf));
//        }
//    }

//}

void SendDataServer(uint8_t *data,int len)
{
    char sendDataCmdBuf[20];
    sprintf(sendDataCmdBuf,(char *)SendDataCmd,len);
//    if(netDeviceInfo_t.netWork)
    if(g_esp_status_t.esp_net_work_e == ESP_NETWORK_SUCCESS)
    {
        if(net_device_send_cmd(sendDataCmdBuf,">") == 0)
        {
            debug_log("Send to esp8266 data len = %d",len);
            USART2_SendData(data,len);
        }
    }
}
/**
 ******************************************************************************
 * @brief  解析ESP8266串口返回数据线程
 ******************************************************************************
**/
void vAnalysisUartData(void *ptr)
{
    signed portBASE_TYPE pd;
//    xSemaphore_CmdLine = xSemaphoreCreateMutex();
    while(1)
    {
        pd =  xQueueReceive(xQueue_USART2_Cmd, gUsartReciveLineBuf, portMAX_DELAY);

        if(pd != pdTRUE)
        {
            printf("pd != pdTRUE\r\n");
            break;
        }

        if(strstr(gUsartReciveLineBuf,"CLOSED") != NULL)
        {
            g_esp_status_t.esp_hw_status_e = ESP_HW_DISCONNECT;
            g_esp_status_t.esp_net_work_e = ESP_NETWORK_FAILED;
            debug_log("TCP CLOSED!");
        }
        else if(strstr(gUsartReciveLineBuf,"+IPD") != NULL)
        {
            /* 处理网络数据 */
            Handle_Internet_Data(gUsartReciveLineBuf);

        }
        else
        {
            if(g_esp_status_t.esp_hw_status_e != ESP_HW_CONNECT_OK || g_esp_status_t.esp_hw_status_e != ESP_HW_RECONFIG)
            {
                /* ESP8266指令数据 */
                esp8266_cmd_handle(gUsartReciveLineBuf);
            }
            else
            {
                debug_log("MCU unknown how prosess!");
                netDeviceInfo_t.cmd_hdl = NULL;
                memset(gUsartReciveLineBuf,0,sizeof(gUsartReciveLineBuf));
            }
        }
        memset(gUsartReciveLineBuf,0,sizeof(gUsartReciveLineBuf));
		vTaskDelay(20 / portTICK_RATE_MS);
    }
}


void getTcpConnect(void)
{
    if(ConnectTcp() == 1)
    {
        g_esp_status_t.esp_hw_status_e = ESP_HW_CONNECT_OK;
		esp_error_t.err_esp_disconnect=0;
		esp_error_t.err_esp_lostwifi = 0;
		esp_error_t.err_esp_notresp = 0;
        debug_log("connect TCP server OK\r\n");
        Mqtt_status_step = MQTT_IDLE;
    }
    else
    {
        debug_log("connect TCP server Fail\r\n");
    }
}
/**
 ******************************************************************************
 * @brief  主处理线程
 ******************************************************************************
**/
void vEsp8266_Main_Task(void *ptr)
{
    ReadChipID(LONG_CLIENT_ID, sizeof(LONG_CLIENT_ID));
	vTaskDelay(3000 / portTICK_RATE_MS);
    while(1)
    {
        if(g_esp_status_t.esp_hw_status_e != ESP_HW_CONNECT_OK || g_esp_status_t.esp_hw_status_e != ESP_HW_RECONFIG)
        {
            g_esp_status_t.esp_hw_status_e = check_esp8266_status();
        }
        switch(g_esp_status_t.esp_hw_status_e)
        {
            case ESP_HW_RESERVE_0:
            case ESP_HW_RESERVE_1:
            case ESP_HW_CONNECT_OK:
                break;

            /* 获得IP */
            case ESP_HW_GET_IPADDR:
                getTcpConnect();
                break;

            /* 失去连接，断开tcp连接 */
            case ESP_HW_DISCONNECT:
                esp_error_t.err_esp_disconnect++;
                g_esp_status_t.esp_net_work_e = ESP_NETWORK_FAILED;
//                SetLedConnectNet(LED_NETWORK_OK);
                debug_log("TCP CLOSEED\r\n");
                vTaskDelay(3000 / portTICK_RATE_MS);//3秒后重新连接服务器
                /* 重新连接TCP服务器 */
                if(g_esp_status_t.esp_hw_status_e != ESP_HW_RECONFIG)	//如果不是配置状态，则断线重新连接
                {
                    getTcpConnect();
                }
                break;
            case ESP_HW_LOST_WIFI:
                esp_error_t.err_esp_lostwifi++;
                debug_log("ESP8266 Lost\r\n");			//设备丢失
								SetLedConnectNet(LED_NETWORK_NOK);
//				  	    ESP8266_EN_INACTIVE;
//						    DelayXms(100);
//                ESP8266_EN_ACTIVE;	
                net_device_send_cmd("AT+RST\r\n","OK");						
						    DelayXms(1000);
						    DelayXms(1000);
						    DelayXms(1000);
						    DelayXms(1000);
							  DelayXms(1000);
						    DelayXms(1000);
					    	//net_device_send_cmd("AT+RST\r\n","OK");
					
                break;
            case ESP_HW_BUSY_STUS:
                debug_log("--%s",netDeviceInfo_t.cmd_resp);
                break;
            case ESP_HW_NOT_RESP:
                esp_error_t.err_esp_notresp++;
                debug_log("USART2 send timeOut!");			//设备丢失
                break;
            default:
                break;

        }
		if(g_esp_status_t.esp_net_work_e == 0 )
		{
			esp_error_t.err_esp_network++;
		}
        if(esp_error_t.err_esp_disconnect > ESP_ERROR_CNT || esp_error_t.err_esp_lostwifi >
                ESP_ERROR_CNT || esp_error_t.err_esp_notresp > ESP_ERROR_CNT || esp_error_t.err_esp_network > ESP_ERROR_CNT )
        {
            //RESTART SYSTEM;
            NVIC_SystemReset();
        }

		debug_log("esp_hw_status_e=%d,esp_net_work_e=%d",g_esp_status_t.esp_hw_status_e,g_esp_status_t.esp_net_work_e);
        vTaskDelay(2000 / portTICK_RATE_MS);
    }
}
/*---------------------------------------------------------------------------*/
int Esp8266_Tcp_Send(int socket, uint8_t *data, uint16_t len)
{
    uint8_t ret;
    char cmd[128] = {0};
    sprintf(cmd, "AT+CIPSEND=%d\r\n", len);
//    debug_log("cmd :%s",cmd);
    if(net_device_send_cmd(cmd,">") == 0)
    {
        USART2_SendData(data,len);
        ret = 0;
    }
    else
        ret = 1;
    return ret;
}

uint8_t Esp8266_GetTcpStatus(void)
{
    uint8_t status = 0;
    if(	g_esp_status_t.esp_hw_status_e  == ESP_HW_CONNECT_OK )
        status = 1;
    return status;
}

