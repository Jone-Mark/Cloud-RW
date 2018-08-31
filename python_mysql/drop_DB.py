import MySQLdb

MYSQLHOST = "127.0.0.1"  #mySQL IP
MYSQLUSER = "root"       #用户名 
MYSQLPW = "123456"       #自己设置的密码 
DBNAME = "mqtt"          #数据库名称
TABLE = 'MQTT'           #表名

db = MySQLdb.connect(MYSQLHOST, MYSQLUSER, MYSQLPW, DBNAME, charset='utf8' )
cursor = db.cursor()

 # 如果数据表已经存在使用 execute() 方法删除表。
sql = 'DROP TABLE IF EXISTS '+TABLE
try:
   # 执行SQL语句
   cursor.execute(sql)
   # 提交修改
   db.commit()
   print ('Drop Mysql'+TABLE+'OK')
except:
   # 发生错误时回滚
   db.rollback()

# 关闭连接
db.close()


