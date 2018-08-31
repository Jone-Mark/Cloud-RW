# -*- coding: utf-8 -*-
# August 2018
# by jiaomengke，@jiaomengke.xin

#simple Python (v3.7) that hat subscribes to a MQTT broker 
#topic and inserts the topic into a mysql database
#pip install mysqlclient-1.3.13-cp37-cp37m-win_amd64.whl
#pip install paho-mqtt

import paho.mqtt.client as mqtt
import MySQLdb
import datetime 
import time
import sys  
import re 

MQTTHOST = "127.0.0.1"#配置MQTT IP 
MQTTPORT = 1883       #端口
MQTTSUB  = 'send1'     #订阅主题
mqttClient = mqtt.Client()

MYSQLHOST = "127.0.0.1"  #mySQL IP
MYSQLUSER = "root"       #用户名 
MYSQLPW = "123456"       #自己设置的密码 
DBNAME = "mqtt"          #数据库名称
TABLE = 'MQTT'           #表名

db = MySQLdb.connect(MYSQLHOST, MYSQLUSER, MYSQLPW, DBNAME, charset='utf8' )
cursor = db.cursor()

 # 如果数据表已经存在使用 execute() 方法删除表。
#cursor.execute('DROP TABLE IF EXISTS '+TABLE)
#print ('Drop Mysql'+TABLE+'OK')


# 创建Mysql表
sql_creat = 'CREATE TABLE IF NOT EXISTS '+TABLE+'(TEMP TINYTEXT NOT NULL,HUMI TINYTEXT NOT NULL,LIGHT TINYTEXT NOT NULL,PRESSURE TINYTEXT NOT NULL,DATA TINYTEXT,TIME TINYTEXT)'

# 使用cursor()方法获取操作游标 
cursor.execute(sql_creat)
print ('CREATE Mysql'+TABLE+'OK')

# 连接MQTT服务器
def on_mqtt_connect():
    mqttClient.connect(MQTTHOST, MQTTPORT, 60)
    mqttClient.loop_start()

# publish 消息
# def on_publish(topic, payload, qos):
#    mqttClient.publish(topic, payload, qos)

# 消息处理函数
def on_message_come(lient, userdata, msg):

    #data = str(msg.payload).strip('b\'')#去除字符串中   b‘’
    #print (data)
    
    #print(msg.topic+ ":" + str(msg.payload))
    #b'{"temperature":30,"humidity":55,"light":-0.0,"pressure":-0.0
    data = (str(msg.payload).split('}',1)[0]+'}').strip('b\'')#去除字符串中\x00
    print (data)
    
    #正则表达式提取字符串
    temp = (str(re.findall(r"temperature\":(.+?),", data))).strip('[]\'')
    humi  =  (str(re.findall(r"humidity\":(.+?),", data))).strip('[]\'')
    light = (str(re.findall(r"light\":(.+?),", data))).strip('[]\'')
    pressure = (str(re.findall(r"pressure\":(.+?)}", data))).strip('[]\'')

    
    today_date = datetime.datetime.now().strftime('%Y-%m-%d') #获取日期
    today_time = datetime.datetime.now().strftime('%H:%M:%S') #获取时间
    #MySQL插入语句
    sql_insert = 'INSERT INTO '+TABLE+'(TEMP,HUMI,LIGHT,PRESSURE,DATA,TIME) VALUES (\'' +str(temp)+'\',\''+str(humi)+'\',\''+str(light)+'\',\''+str(pressure)+'\',\''+today_date+'\',\''+today_time +'\')'
    print (sql_insert)
    try:
   
       # 执行sql语句
       
       cursor.execute(sql_insert)
       # 提交到数据库执行
       db.commit()
       print("OK")
    except:
       # Rollback in case there is any error
       db.rollback()
       print("ERR")

# subscribe 消息
def on_subscribe():
    mqttClient.subscribe(MQTTSUB, 1)
    mqttClient.on_message = on_message_come # 消息到来处理函数


def main():

    on_mqtt_connect()
    #on_publish("/test/server", "Hello Python!", 1)
    on_subscribe()
    while True:
        time.sleep(0.1) # 休眠1MS  非常重要，没有的话，主线程一直在跑，CPU100%，会卡死MQTT服务器
        pass

if __name__ == '__main__':
    main()
