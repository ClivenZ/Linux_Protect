#pragma once 

#include<cstdio>
#include<cstdlib>
#include<mysql/mysql.h>
#include<jsoncpp/json/json.h>

namespace order_system{
  
  //
static MYSQL* MySQLInit(){
  //创建句柄
  MYSQL* mysql = mysql_init(NULL);
  //建立链接
  if(mysql_real_connect(mysql,"127.0.0.1","root","656260","order_system",3306,NULL,0) == NULL){
    printf("connect failed! %s\n",mysql_error(mysql));
    return NULL;
  }
  //设置编码格式
  mysql_set_character_set(mysql,"utf8");
  return mysql;
}

static void  MySQLRelease(MYSQL* mysql){
  mysql_close(mysql);
}
 

class DishTable{
public:
  DishTable(MYSQL* mysql):
    _mysql(mysql)
  {}
  //使用 JSON 作为函数参数
  bool Insert(const Json::Value& dish){
    char sql[1024 * 4] = {0};
    sprintf(sql,"insert into dish_table values(null,'%s',%d)",dish["name"].asCString(),dish["price"].asInt());
    //
    int ret = mysql_query(_mysql,sql);
    if(ret != 0){
      printf("DishTable Insert failed! %s\n",mysql_error(_mysql));
      return false;
    }
    printf("DishTable Insert finished!\n");
    return true;
  }
  //const & 表示输入型参数
  //* 表示输出型参数
  //& 表示输入输出型参数
  bool SelectAll(Json::Value* dishes){
    char sql[1024 * 4] = {0};
    sprintf(sql,"select dish_id,name,price from dish_table");
    int ret = mysql_query(_mysql,sql);
    if(ret != 0){
      printf("DishTable SelectAll failed! %s\n",mysql_error(_mysql));
      return false;
    }
    printf("DishTable SelectAll OK!\n");
    //构造结果
    MYSQL_RES* result = mysql_store_result(_mysql);
    //获取行数
    int rows = mysql_num_rows(result);
    for(int i = 0;i < rows; ++i){
      MYSQL_ROW row = mysql_fetch_row(result);
      Json::Value dish;
      dish["dish_id"] = atoi(row[0]);
      dish["name"] = row[1];
      dish["price"] = atoi(row[2]);
      dishes->append(dish);
    }
    //注意释放资源
    mysql_free_result(result);
    return true;
  }

  bool SelectOne(int32_t dish_id,Json::Value* dish){
    //1、拼SQL语句 where dish_id = dish_id
    char sql[1024 * 4] = {0};
    sprintf(sql,"select * from dish_table where dish_id = %d",dish_id);
    //2、执行SQL
    int ret = mysql_query(_mysql,sql);
    if(ret != 0){
      printf("Dishtable selectone failed! %s\n ",mysql_error(_mysql));
      return false;
    }
    //3、遍历结果结合
    MYSQL_RES* result = mysql_store_result(_mysql);
    if(result ==  NULL){
       printf("Dishtable seletone result failed! %s\n",mysql_error(_mysql));
       return false;
    }
    //4、取出数据
    int rows = mysql_num_rows(result);
    if(rows != 1){
      printf("The number of result rows is not one! rows = %d\n",rows);
      return false;
    }
    for(int i = 0;i < rows; ++i){
      MYSQL_ROW row = mysql_fetch_row(result);
      (*dish)["dish_id"] = atoi(row[0]);
      (*dish)["name"] = row[1];
      (*dish)["price"] = atoi(row[2]);
      break;
    }
    //5、释放数据
    printf("Dishtale selectone is OK!\n");
    mysql_free_result(result);
    return true;
  }

  bool Update(const Json::Value& dish){
    char sql[1024 * 4] = {0};
    sprintf(sql,"update dish_table SET name='%s',price=%d where dish_id=%d",
        dish["name"].asCString(),
        dish["price"].asInt(),
        dish["dish_id"].asInt());
    int ret = mysql_query(_mysql,sql);
    if(ret != 0){
      printf("Dishtable update failed! %s\n",mysql_error(_mysql));
      return false;
    }
    printf("Dishtable update OK!\n");
    return true;
  }
  bool Delete(int32_t dish_id){
    char sql[1024 * 4] = {0};
    sprintf(sql,"delete from dish_table where dish_id=%d",dish_id);
    int ret = mysql_query(_mysql,sql);
    if(ret != 0){
      printf("Dishtable delete falied! %s\n",mysql_error(_mysql));
      return false;
    }
    printf("Dishtable delete OK!\n");
    return true;
  } 
private:
  MYSQL* _mysql;
};//end class DishTable 

class OrderTable{
public:
  OrderTable(MYSQL* mysql):
    _mysql(mysql)
  {}
  bool Insert(const Json::Value& order){
    char sql[1024 * 4] ={0};
    sprintf(sql,"insert into order_table values(null,'%s','%s','%s',%d)"
        ,order["table_id"].asCString()
        ,order["time"].asCString()
        ,order["dish_ids"].asCString()
        ,order["state"].asInt());
    int ret = mysql_query(_mysql,sql);
    if(ret != 0){
      printf("Ordertable insert failed! %s\n",mysql_error(_mysql));
      return false;
    }
    printf("Ordertable insert OK!\n");
    return true;
  }
  bool SelectAll(Json::Value* orders){
    char sql[1024 * 4] = {0};
    sprintf(sql,"select * from order_table");
    int ret = mysql_query(_mysql,sql);
    if(ret != 0){
      printf("Ordertable selectall failed! %s\n",mysql_error(_mysql));
      return false;
    }
    MYSQL_RES* result = mysql_store_result(_mysql);
    if(result == NULL){
      printf("Ordertable selectall result failed! %s\n",mysql_error(_mysql));
      return false;
    }
    int rows = mysql_num_rows(result);
    for(int i = 0;i < rows; ++i){
      MYSQL_ROW row = mysql_fetch_row(result);
      Json::Value order;
      order["order_id"] = atoi(row[0]);
      order["table_id"] = row[1];
      order["time"] = row[2];
      order["dish_ids_str"] = row[3];
      order["state"] = atoi(row[4]);
      orders->append(order);
    }
    printf("Ordertable selectone OK!\n");
    mysql_free_result(result);
    return true;
  }
//  bool SelectOne(Json::Value* order){
//    char sql[1024 * 4] = {0};
//    sprintf(sql,"select * from order_table where order_id=%d",order["order_id"].asInt);    
//    return true;
//  }
  bool ChangeState(const Json::Value& order){
    char sql[1024 * 4] = {0};
    sprintf(sql,"updata order_table set state=%d where order_id=%d"
        ,order["state"].asInt()
        ,order["order_id"].asInt());
    int ret = mysql_query(_mysql,sql);
    if(ret != 0){
      printf("Ordertable changestate falied! %s\n",mysql_error(_mysql));
      return false;
    }
    printf("Ordertable changestate OK!\n"); 
    return true;
  }
private:
  MYSQL* _mysql;
}; //end class OrderTable 

}//end order_system 

