#include "ros/ros.h"
#include <stdlib.h>
#include "ros_lab1/gambling_table.h"
#include <iostream>
#include <string>
#include "std_msgs/String.h"
using namespace std;
using namespace ros;
Publisher g_casino_logger;

bool casino(ros_lab1::gambling_table::Request  &req, ros_lab1::gambling_table::Response &res)
{
  int32_t user_number = req.number;
  string user_bet = req.color;
  int32_t user_money_bet = req.bet;

  ROS_INFO("All bets accepted!");

  //Сначала определяю выигрышные число и цвет, rand наше всё
  int32_t winning_number = rand()%37;
  string winning_color;

  if(rand()%2 == 0)
    winning_color = "black";
  else if(winning_number == 0)
    winning_color = "green";
  else
    winning_color = "red";

  //Это поможет определять победу и ставку, если коэф-т минус, значит проиграл
  bool is_win = false;
  int32_t winning_mult = -1;

  ROS_INFO("You're bet:");

  //Сначала определю победу для простых случаев- когда шансы 50 на 50
  if(user_number > 36)
  {
    switch (user_number) {
      //Ставка просто на число
      case 37:
        is_win = user_bet.find(winning_color) != std::string::npos;
        break;
      //Ставка четное-нечетное
      case 38:
        is_win = ((winning_number != 0) &&
                 (winning_number % 2 == 0 && user_bet.find("even") != std::string::npos) ||
                 (winning_number % 2 == 1 && user_bet.find("odd") != std::string::npos));
        break;
      //Ставка больше-меньше
      case 39:
        is_win = ((winning_number != 0) &&
                 (winning_number > 18 && user_bet.find("passe") != std::string::npos) ||
                 (winning_number < 19 && user_bet.find("manque") != std::string::npos));
        break;
    }
    if(is_win)
      winning_mult = 1;

    //Ну и заодно выведу, на что поставил
    ROS_INFO("%s %i \n", user_bet.c_str(), user_money_bet);
  }

  //Этот случай- цвет и число, просто сравниваю
  else
  {
    is_win = (winning_number == user_number) && (winning_color.find(user_number));
    if(is_win)
      winning_mult = 35;

    ROS_INFO("%s %i %i \n", user_bet.c_str(), user_number, user_money_bet);
  }

  std_msgs::String str;

  //Победа определена, осталосьнего просто вывести всё в сервис, топик и консоль
  if (is_win)
  {
    string msg = "Unbelievable! You won!";
    ROS_INFO(msg.c_str());
    str.data = msg;
  }
  else
  {
    string msg = "You lose...";
    ROS_INFO(msg.c_str());
    str.data = msg;
  }

  //Если вот это вот ЧЕРТ ЕГО ПОБЕРИ НЕОБХОДИМУЮ штуку не делать, то сообщение просто отправится в никуда,
  //потому что паблишер еще не подцепит всех подписчиков
  while(g_casino_logger.getNumSubscribers() <= 0){}
  //По заданию- дополнительно отправляю результат в топик
  g_casino_logger.publish(str);

  ROS_INFO("Winning combination was %s color, %i \n\n", winning_color.c_str(), winning_number);

  res.result = is_win;
  res.prize = winning_mult * user_money_bet;

  return true;
}

int main(int argc, char **argv)
{
  init(argc, argv, "house");
  NodeHandle n;
  string service_name;
  n.getParam("/casino_service_name", service_name);
  ServiceServer service = n.advertiseService(service_name, casino);

  string topic_name;
  n.getParam("/casino_topic_name", topic_name);
  g_casino_logger = n.advertise<std_msgs::String>(topic_name, 1000);

  ROS_INFO("Welcome to the fabulous ROS casino!");
  spin();

  return 0;
}
