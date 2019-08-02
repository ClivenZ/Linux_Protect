#include<cstdio>
#include<cstdlib>
//第三方库 MySQL
#include<mysql/mysql.h>

int main(){
  //1、创建一个数据库的操作“句柄”
  MYSQL* mysql = mysql_init(NULL);
  //2、建立句柄和数据库之间的联系（连接服务器）
  if(mysql_real_connect(mysql,"127.0.0.1","root",""
        ,"order_system",3306,NULL,0) == NULL){
    printf("connect failed! %s\n",mysql_error(mysql));
    return 1;
  }
  //3、设置编码格式(避免乱码)
  mysql_set_character_set(mysql,"utf8");
  //4、拼装一个插入数据 SQL
  char sql[1024] = {0};
  int price = 2000;
  sprintf(sql,"insert into dish_table values(null,"",%d)",price);
  //5、执行SQL语句
  int ret = mysql_query(mysql,sql);
  if(ret != 0){
    printf("sql failed! %s\n",mysql_error(mysql));
    mysql_close(mysql);
    return 1;
  }

  //6、关闭连接
  mysql_close(mysql);

  return 0;
}
