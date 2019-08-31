#include"db.hpp"
#include<iostream>

using namespace std;
int main(){
  using namespace order_system;
  //class dish_table test
  bool ret = false;
  Json::StyledWriter write;
  MYSQL* mysql = MySQLInit();

  //Json::Value dish;
 // dish["name"] = "红烧肉";
 // dish["price"] = 2300;
       //insert
 // DishTable dish_table(mysql);
  //ret = dish_table.Insert(dish);
       //selectall
  //Json::Value dishes;
  //ret = dish_table.SelectAll(&dishes);
      //Update
  //dish["dish_id"] = 2;
//  dish["name"] = "毛家红烧肉";
//  dish["price"] = 3000;
 // Json::Value dish_out;
 // ret = dish_table.Update(dish);
  //ret = dish_table.SelectOne(5,&dish_out);
  
  
  //delete
// int dish_id = 2;
//  ret = dish_table.Delete(dish_id);

// order_table test 
//
   OrderTable order_table(mysql);
//    Json::Value order; 
//    order["table_id"] = "007";
//    order["time"] = "2019/08/01 23:00";
//    order["dish_ids"] = "1,2";
//    order["state"] = 0;
//.autorelabel    ret = order_table.Insert(order);
//


//selectall
  Json::Value orders;
  ret = order_table.SelectAll(&orders);
  std:: cout << "Selectall:"  << ret << endl;

//changestaue
  //Json::Value state;
  //state["order_id"] = 3;
  //state["state"] = 1;
  //ret = order_table.ChangeState(state);


  MySQLRelease(mysql);  
  return 0;
}
