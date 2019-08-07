#include"db.hpp"

int main(){
  using namespace order_system;
  //class dish_table test
  bool ret = false;
  Json::StyledWriter write;
  MYSQL* mysql = MySQLInit();

  Json::Value dish;
  dish["name"] = "红烧肉";
  dish["price"] = 2300;
  //insert
  DishTable dish_table(mysql);
  ret = dish_table.Insert(dish);
  return 0;
}
