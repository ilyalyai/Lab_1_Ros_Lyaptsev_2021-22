#include "ros/ros.h"
#include <stdlib.h>
#include "ros_lab1/gambling_table.h"
#include <iostream>
#include <string>
using namespace std;
using namespace ros;

bool add(ros_lab1::gambling_table::Request  &req, ros_lab1::gambling_table::Response &res)
{
  int32_t user_number = req.number;
  string user_bet = req.color;
  int32_t user_money_bet = req.bet;
  ROS_INFO("All bets accepted!");
  int32_t winning_number = rand()%37;
  string winning_color;
  if(rand()%2 == 0)
    winning_color = "black";
  else if(winning_number == 0)
    winning_color = "green";
  else
    winning_color = "red";

  bool is_win = false;
  int32_t winning_mult = -1;

    ROS_INFO("You're bet:");

  if(user_number > 36)
  {
    switch (user_number) {
      case 37:
        is_win = user_bet.find(winning_color) != std::string::npos;
        break;
      case 38:
        is_win = ((winning_number != 0) &&
                 (winning_number % 2 == 0 && user_bet.find("even") != std::string::npos) ||
                 (winning_number % 2 == 1 && user_bet.find("odd") != std::string::npos));
        break;
      case 39:
        is_win = ((winning_number != 0) &&
                 (winning_number > 18 && user_bet.find("passe") != std::string::npos) ||
                 (winning_number < 19 && user_bet.find("manque") != std::string::npos));
        break;
    }
    winning_mult = 1;
    ROS_INFO("%s %i \n", user_bet.c_str(), user_money_bet);
  }

  else
  {
    is_win = (winning_number == user_number) && (winning_color.find(user_number));
    if(is_win)
      winning_mult = 35;

    ROS_INFO("%s %i %i \n", user_bet.c_str(), user_number, user_money_bet);
  }

  if (is_win)
    ROS_INFO("Inbelievable! You won!");
  else
    ROS_INFO("You lose...");

  ROS_INFO("Winning combination was %s color, %i \n\n", winning_color.c_str(), winning_number);

  res.result = is_win;
  res.prize = winning_mult * user_money_bet;
  return true;
}

int main(int argc, char **argv)
{
  init(argc, argv, "casino");
  NodeHandle n;

  ServiceServer service = n.advertiseService("gambling_table", add);
  ROS_INFO("Welcome to the fabulous ROS casino!");
  spin();

  return 0;
}
