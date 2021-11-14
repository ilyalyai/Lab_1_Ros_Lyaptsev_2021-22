#include "ros/ros.h"
#include <stdlib.h>
#include "ros_lab1/gambling_table.h"
#include <iostream>
#include <string>
using namespace std;
using namespace ros;

bool add(ros_lab1::gambling_table::Request  &req, ros_lab1::gambling_table::Response &res)
{
  int user_number = req.number;
  string user_bet = req.color;
  ROS_INFO("All bets are off!");
  int winning_number = rand()%37;
  string winning_color;
  if(rand()%2 == 0)
    winning_color = "black";
  else if(winning_number == 0)
    winning_color = "green";
  else
    winning_color = "red";

  bool is_win = false;

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
  }

  else
    is_win = (winning_number == user_number) && (winning_color.find(user_number));

  if (is_win)
    ROS_INFO("Inbelievable! You won!");
  else
    ROS_INFO("You lose...");

  ROS_INFO("Winning combination was %s color, %i", winning_color.c_str(), winning_number);

  res.result = is_win;
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
