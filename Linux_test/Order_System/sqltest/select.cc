#include<cstdio>
#include<cstdlib>
#include<mysql/mysql.h>

int main(){
  MYSQL* mysql = mysql_init(NULL);

  if(mysql_real_connect(mysql,"127.0.0.1","root","",3306,NULL,0) == NULL){
    printf("connect failed! %s\n",mysql_error(mysql));
    return 1;
  }
  mysql_set_character_set(mysql,"utf8");
  char sql[1024] = {0};
  sprintf(sql,"select * from dish_table");

  int ret = mysql_query(mysql,sql);
  if(ret != 0){
    printf("musql_query failed! %s\n",mysql_error(mysql));
    mysql_close(mysql);
    return 1;
  }
  //获取并遍历结果集合
  //获取结果集合
  MYSQL_RES* result = mysql_store_result(mysql);
  //获取结果种有几行
  int rows = mysql_num_rows(result);
  int cols = mysql_num_fields(result);
  //根据行列遍历结果
  for(int row = 0;row < rows; ++row){
    MYSQL_ROW mysql_row = mysql_fetch_row(result);
    for(int col = 0 ;col < cols ; ++col){
      printf("%s\t",result);
    }
    printf("\n");
  }
  mysql_colse(mysql);

}
