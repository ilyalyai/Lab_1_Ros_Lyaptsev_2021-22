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
  string user_color = req.color;
  ROS_INFO("All bets are off!");
  int winning_number = rand()%37;
  int winning_color_number = rand()%2;
  bool is_win = false;
  switch (user_color[0])
  {
    case 'b':
      is_win = (winning_number == user_number) && (winning_color_number == 0);
      break;
    case 'r':
      is_win = (winning_number == user_number) && (winning_color_number == 1);
      break;
    case 'w':
      is_win = winning_number == 0;
      break;
    default:
      ROS_INFO("Are you joking with me?!?");
      ROS_INFO("Of cource you lose, with such a combination!");
      break;
  }
  if (is_win)
    ROS_INFO("Inbelievable! You won!");
  else
  {
    string winning_color;
    if(winning_number == 0)
      winning_color = "white";
    else if(winning_color_number == 0)
      winning_color = "black";
    else
      winning_color = "red";
    ROS_INFO("You lose...");
    ROS_INFO("Winning combination was %s color, %i", winning_color.c_str(), winning_number);
  }

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
