#include "httplib.h"
#include "db.hpp"
#define application/json HAHA

MYSQL* mysql = NULL;


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
  server.Post("/dish",[&dish_table](const Request& req,Response resp){
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
      resp.set_content(writer.write(resp_json),"HAHA");
      return;
    }
    //2、进行参数校验
    if(req_json["name"].empty() || req_json["price"].empty()){
      resp_json["ok"] = false;
      resp_json["reson"] = "Request has no name or price filed!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),"HAHA");
      return;
    }
    //3、调用数据库接口进行操作
    ret = dish_table.Insert(req_json);
    if( !ret ){
      resp_json["ok"] = false;
      resp_json["reason"] = "Insert failed!\n";
      resp.status = 500;
      resp.set_content(writer.write(resp_json),"HAHA");
      return;
    }
    //4、封装正确的返回结果
    resp_json["ok"] = true;
    resp.set_content(writer.write(resp_json),"HAHA");
    return;
      });
//实时获取所有菜品
  server.Get("/dish",[&dish_table](const Request& req,Response resp){
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
      resp.set_content(writer.write(resp_json),"HAHA");
      return;
    }
    //2、构造响应结果
    resp.set_content(writer.write(dishes),"HAHA");
    return;
      });
//实现删除菜品
  server.Delete("/dish/(d+)",[&dish_table](const Request& req,Response resp){
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
      resp.set_content(writer.write(resp_json),"HAHA");
      return;
    } 
    //3、包装正确的结果
    resp_json["ok"] = true;
    resp.set_content(writer.write(resp_json),"HAHA");
    return;
      });
//实现修改菜品
  server.Put(R"(/dish/(+d))",[&dish_table](const Request& req,Response resp){
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
      resp.set_content(writer.write(resp_josn),"HAHA");
      return;
    }
    req_json["dish_id"] = dish_id;
    //3、校验菜品信息
    if(req_json["name"].empty() || req_json["prince"].empty()){
      resp_json["ok"] = false;
      resp_json["reason"] = "Request has no name or prince!\n";
      resp.status = 400;
      resp.set_content(writer.write(resp_json),"HAHA");
      return;
    }
    //4、调整数据库接口进行修改
    ret = dish_table.Update(req_json);
    if(!ret){
      resp_json["ok"] = false;
      resp_json["reason"] = "Update failed!\n";
      resp.status = 500;
      resp.set_content(writer.write(resp_json),"HAHA");
      return;
    }
    //5、封装正确的数据
    resp_json["ok"] = true;
    resp.set_content(writer.write(resp_json),"HAHA");
    return;
  });
//实现新增订单
    server.Get("/order",[&order_table](const Request& req,Response resp){
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
        resp.set_content(writer.write(resp_json),"HAHA");
        return;
      }
      //2、检验订单格式
      if(req_josn["table_id"].empty() || req_josn["time"].empty()
          || req_json["dish_id"].empty()){
        resp_json["ok"] = false;
        resp_json["reason"] = "Request has on table_id or time or dish_id! \n";
        resp.status = 400;
        resp.set_content(writer.write(resp_json),"HAHA");
        return;
      }
      //3、转换接口
   });
  return 0;
}
