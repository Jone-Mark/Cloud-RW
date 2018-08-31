# Cloud RW

<br> 本项目主要功能:\<br>
<br> 1>：单片机采集信息，发布信息通过ESP8266 发送到MQTT服务器\<br>
<br> 2>：采用Python监听订阅MQTT数据，数据存到Mysql服务器\<br>
<br> 3>：PHP读取服务器数据存储为Json，\<br>
<br> 4>：H5采用Highchart读取PHPjson数据显示在H5网页上；\<br>
 <br>   下行控制不需要进行服务器操作，直接利用webMqtt进行发布消息，单片机即时订阅，网络延时经测试可以忽略不计。\<br>

## 1.单片机下位机部分：
  <br> 主控芯片：STM32F103VET6,通信模块：ESP8266 WIFI模块，程序：FreeROTS\<br>
  <br> 功能：每10S发布一次传感器信息（可修改），随时订阅来自服务器下发的消息控制LED灯等设备，OLED显示等...\<br>

## 2.Python 

<br>include python files  that ctrl Mysql\<br>
<br>function:\<br>
<br>a:sub mqtt data\<br>
<br>b:DROP Mysql   \<br>
<br>c:Create Mysql\<br>
<br>d:Get Mqtt Data to mysql \<br>
<br>使用方法：双击相应bat文件，\<br>


## 3.PHP
<br>Get Data form mysql encode json data\<br>

## 4.H5 HighChart show PHP data By line chart\<br>
    <br>发布LED消息
[![baidu]](http://baidu.com)  
[baidu]:http://47.106.209.211/mqtt/main.php

    



