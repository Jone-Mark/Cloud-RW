#!/usr/bin/python
# -*- coding: UTF-8 -*-

import MySQLdb
import datetime

MYSQLHOST = "127.0.0.1"
MYSQLUSER = "root"
MYSQLPW = "123456"
DBNAME = "mqtt"

today_date = datetime.datetime.now().strftime('%Y-%m-%d')
today_time = datetime.datetime.now().strftime('%H:%M:%S')

print (today_date)
print (today_time)

# 打开数据库连接
db = MySQLdb.connect(MYSQLHOST, MYSQLUSER, MYSQLPW, DBNAME, charset='utf8' )

# 使用cursor()方法获取操作游标 
cursor = db.cursor()

# 如果数据表已经存在使用 execute() 方法删除表。
cursor.execute("DROP TABLE IF EXISTS TEMP")

# 创建数据表SQL语句
sql_creat = """CREATE TABLE TEMP(
         SENSER varchar(20) NOT NULL,
         DATA TINYTEXT,
         TIME TINYTEXT)"""

# SQL 插入语句
sql_insert = "INSERT INTO TEMP(SENSER,DATA,TIME) VALUES (12,'"+today_date+"','"+today_time+"')"
#sql_insert = "INSERT INTO TEMP(SENSER,DATA,TIME) VALUES (12,'12:2',12)"
print (sql_insert)
try:
   
   # 执行sql语句
   cursor.execute(sql_creat)
   cursor.execute(sql_insert)
   # 提交到数据库执行
   db.commit()
   print("OK")
except: 
   # Rollback in case there is any error
   db.rollback()
   print("ERR")
# 关闭数据库连接
#db.close()
