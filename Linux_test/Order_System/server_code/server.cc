#include "httplib.h"
#include "db.hpp"
#include <iostream>
#include <jsoncpp/json/json.h>
const char* CONTENT_TYPE = "applicaton/json";

MYSQL* mysql = NULL;

//辅助函数
bool GetDishes(const Json::Value& dish_ids,Json::Value* dishes,
    order_system::DishTable& dish_table){
  for(uint32_t i = 0;i < dish_ids.size();++i){
    int dish_id = dish_ids[i].asInt();
    Json::Value dish_info;
    bool ret = dish_table.SelectOne(dish_id,&dish_info);
    if(!ret){
      std::cout  << "dish_id" << dish_id << "not found!\n";
      continue;
    }
    dishes->append(dish_info);
  }
  return true;
}
int GetConsume(const Json::Value& dishes){
  int consume = 0;
  for(uint32_t i = 0;i < dishes.size();++i){
    consume += dishes[i]["price"].asInt(); 
  }
  return consume;
}
int main(){
  using namespace httplib;
  using namespace order_system;
  Server server;
  //数据库客户端初始化和释放
  mysql = MySQLInit();
  signal(SIGINT,[](int){ MySQLRelease(mysql);exit(0);});
  DishTable dish_table(mysql);
  OrderTable order_table(mysql);
  //设置路由
//新增菜品
  server.Post("/dish",[&dish_table](const Request& req,Response& resp){
      Json::Reader reader;
      Json::FastWriter writer;
      Json::Value req_json;
      Json::Value resp_json;
      //1、请求解析为Json格式
      bool ret = reader.parse(req.body,req_json);
      if( !ret ){
      //请求解析错误，返回400响应
      resp_json["ok"] = false;
      resp_json["reason"] = "parse Request failed!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      //2、进行参数校验
      if(req_json["name"].empty() || req_json["price"].empty()){
      resp_json["ok"] = false;
      resp_json["reson"] = "Request has no name or price filed!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      //3、调用数据库接口进行操作
      ret = dish_table.Insert(req_json);
      if( !ret ){
        resp_json["ok"] = false;
        resp_json["reason"] = "Insert failed!\n";
        resp.status = 500;
        resp.set_content(writer.write(resp_json),CONTENT_TYPE);
        return;
      }
      //4、封装正确的返回结果
      resp_json["ok"] = true;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
  });
  //实时获取所有菜品
  server.Get("/dish",[&dish_table](const Request& req,Response& resp){
      Json::Reader reader;
      Json::FastWriter writer;
      Json::Value resp_json;
      //1、调用数据库接口
      Json::Value dishes;
      bool ret = dish_table.SelectAll(&dishes);
      if(!ret){
      resp_json["ok"] = false;
      resp_json["reson"] = "SelectAll failed!\n";
      resp.status = 500;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      //2、构造响应结果
      resp.set_content(writer.write(dishes),CONTENT_TYPE);
      return;
      });
//实现删除菜品
  server.Delete(R"(/dish/(\d+))",[&dish_table](const Request& req,Response& resp){
      Json::Value resp_json;
      Json::FastWriter writer;
      //1、解析获取 dish_id 
      int dish_id = std::stoi(req.matches[1]);
      //2、调用数据库接口
      bool ret = dish_table.Delete(dish_id);
      if(!ret){
      resp_json["ok"] = false;
      resp_json["reason"] = "Delete failed!\n";
      resp.status = 500;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      } 
      //3、包装正确的结果
      resp_json["ok"] = true;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      });
//实现修改菜品
  server.Put(R"(/dish/(\d+))",[&dish_table](const Request& req,Response& resp){
      Json::Reader reader;
      Json::FastWriter writer;
      Json::Value req_json;
      Json::Value resp_json;
      //1、获取菜品的id
      int dish_id = std::stoi(req.matches[1]);
      //2、解析菜品信息
      bool ret = reader.parse(req.body,req_json);
      if(!ret){
      resp_json["ok"] = false;
      resp_json["reason"] = "Parse Request failed!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      req_json["dish_id"] = dish_id;
      //3、校验菜品信息
      if(req_json["name"].empty() || req_json["price"].empty()){
      resp_json["ok"] = false;
      resp_json["reason"] = "Request has no name or prince!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      //4、调整数据库接口进行修改
      ret = dish_table.Update(req_json);
      if(!ret){
        resp_json["ok"] = false;
        resp_json["reason"] = "Update failed!\n";
        resp.status = 500;
        resp.set_content(writer.write(resp_json),CONTENT_TYPE);
        return;
      }
      //5、封装正确的数据
      resp_json["ok"] = true;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
  });
  //实现新增订单
  server.Post("/order",[&order_table](const Request& req,Response& resp){
      Json::Reader reader;
      Json::FastWriter writer;
      Json::Value req_json;
      Json::Value resp_json;
      //1、请求解析成Json格式
      bool ret = reader.parse(req.body,req_json);
      if(!ret){
      resp_json["ok"] = false;
      resp_json["reason"] = "Parse failed!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      //2、检验订单格式
      if(req_json["table_id"].empty() || req_json["time"].empty()
          || req_json["dish_ids"].empty()){
      resp_json["ok"] = false;
      resp_json["reason"] = "Request has on table_id or time or dish_id! \n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      //3、转换接口
      const Json::Value& dish_ids = req_json["dish_ids"];
      req_json["dish_ids_str"] = writer.write(dish_ids);
      //4、调用数据库接口，插入数据
      ret = order_table.Insert(req_json);
      if(!ret){
        resp_json["ok"] = false; 
        resp_json["reason"] = "OrderTable Insert failed!\n";
        resp.status = 500;
        resp.set_content(writer.write(resp_json),CONTENT_TYPE);
        return;
      }
      //5、返回正确结果
      resp_json["ok"] = true;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
  });
//实现修改订单
  server.Put(R"(/order/(\d+))",[&order_table](const Request& req,Response& resp){
      Json::Reader reader;
      Json::FastWriter writer;
      Json::Value req_json;
      Json::Value resp_json;
      //1、获取订单
      int order_id = std::stoi(req.matches[1]);
      //2、请求解析为Json格式
      bool ret = reader.parse(req.body,req_json);
      if(!ret){
      resp_json["ok"] = false;
      resp_json["reason"] = "parse Request failed!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      req_json["order_id"] = order_id;
      //3、校验订单格式
      if(req_json["order_id"].empty() || req_json["state"].empty()){
      resp_json["ok"] = false;
      resp_json["reason"] = "Request has no order_id or state\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      //4、调用数据库接口
      ret = order_table.ChangeState(req_json);
      if(!ret){
        resp_json["ok"] = false;
        resp_json["reason"] = "ChangeState failed!\n";
        resp.status = 500;
        resp.set_content(writer.write(resp_json),CONTENT_TYPE);
        return;
      }
      resp_json["ok"] = true;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
  });
  //实现获取订单
  server.Get("/order",[&order_table,&dish_table](const Request& req,Response& resp){
      Json::Reader reader;
      Json::FastWriter writer;
      Json::Value resp_json;
      //1、调用数据库接口
      Json::Value orders;
      bool ret = order_table.SelectAll(&orders);
      if(!ret){
      resp_json["ok"] = false;
      resp_json["reason"] = "SelectAll failed!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),CONTENT_TYPE);
      return;
      }
      for(uint32_t order_index = 0;order_index < orders.size();++order_index){
      //循环处理
      Json::Value& order = orders[order_index];
      //2、格式转化
      Json::Value dish_ids;
      ret = reader.parse(order["dish_ids_str"].asString(),dish_ids);
      if(!ret){
        std::cout << "order_id" << order["order_id"].asInt()
          << "has error dish_ids_str!" << std::endl;
        continue;
      }
      //3、获取菜品的详细信息
      GetDishes(dish_ids,&order["dishes"],dish_table);
      //4、构造 cousume 字段，求这个订单的总价
      order["consume"] = GetConsume(order["dishes"]);
      }
      //5、响应结果
      resp.set_content(writer.write(orders),CONTENT_TYPE);
      return;
  });
    //设置静态文件目录
    server.set_base_dir("./wwwroot");
    server.listen("0.0.0.0",9094);
  return 0;
}



