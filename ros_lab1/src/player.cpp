#include "ros/ros.h"
#include <stdlib.h>
#include "ros_lab1/gambling_table.h"
#include <iostream>
#include <string>
using namespace std;
using namespace ros;

pair<string, int> calculateInput(int argc, char **argv)
{
  pair<string, int> input;
  string user_bet = argv[1];
  if(argc == 2)
  {
    //Первый случай: ставим чисто на цвет, число тогда вернем 37, потом это поймаем
    if(user_bet.find("black") != std::string::npos ||
       user_bet.find("green") != std::string::npos ||
       user_bet.find("red") != std::string::npos)
    {
      input.first = user_bet;
      input.second = 37;
    }
    //Второй- чет/нечет
    else if(user_bet.find("odd") != std::string::npos ||
            user_bet.find("even") != std::string::npos)
    {
      input.first = user_bet;
      input.second = 38;
    }
    //Третий- от 1 до 18 или от 19 до 36
    else if(user_bet.find("passe") != std::string::npos ||
            user_bet.find("manque") != std::string::npos)
    {
      input.first = user_bet;
      input.second = 39;
    }
    else
    {
      return input;
    }

  }
  else if(user_bet.find("black") != std::string::npos ||
          user_bet.find("red") != std::string::npos ||
          (user_bet.find("green") != std::string::npos && atoll(argv[2]) == 0))
  {
    input.first = user_bet;
    input.second = atoll(argv[2]);
  }
  return input;
}

int main(int argc, char **argv)
{
  //3- цвет и номер, 2- чет/нечет, только цвет
  if (argc != 3 && argc != 2)
  {
    ROS_INFO("usage: player <string>color <uint>number");
    ROS_INFO("player <string>color <uint>number");
    ROS_INFO("player <string>color");
    ROS_INFO("player <string>\"odd/even/passe/manque\"");
    return 1;
  }

  if(argc == 3 && atoll(argv[2]) > 36)
  {
    ROS_INFO("Wrong number: 0 <= number <= 36");
    return 1;
  }

  pair<string, int> input = calculateInput(argc, argv);

  if(input.first.empty())
  {
    ROS_INFO("Sorry, couldn't recognise you're input");
    return 1;
  }

  int request_number = input.second;
  string request_color = input.first;

  //Запуск всех полезных штук, инициализация сервиса для записи значений
  init(argc, argv, "house");
  NodeHandle n;
  ServiceClient client = n.serviceClient<ros_lab1::gambling_table>("gambling_table");
  ros_lab1::gambling_table srv;

  srv.request.color = request_color;
  srv.request.number = request_number;

  while(ok())
  {
    if (client.call(srv))
    {
      if(srv.response.result)
      {
        int response = rand()%2;
        string responce_text[3] = {"Remember: the house always wins, maybe it's time to leave?",
                                   "Veni, Vidi, Vici",
                                   "Viva Ros Vegas!"};
        cout << responce_text[response] << endl;
      }
      else
      {
        int response = rand()%2;
        string responce_text[3] = {"The game was rigged from the start!",
                                   "How can a sealed deck be laid out in a different order?!?",
                                   "That's the truth about Las Vegas: we're the only winners. The players don't stand a chance."};
        cout << responce_text[response] << endl;
      }
      cout << "If you want to play again, input <string>color, enter, <uint>number" << endl;
      cout << "If you want to throw up you're cards, type \"surrender\"" << endl;
      string input;
      cin >> input;

      if(input.find("surrender") != std::string::npos)
        return 1;

      argv[1] = &input[0];

      cout << "If you want to bet on number, input it, otherwise input \"bet\"" << endl;

      cin >> input;

      if(input.find("bet") != std::string::npos)
        argc = 2;
      else
      {
        argc = 3;
        argv[2] = &input[0];
      }

      pair<string, int> input_pair = calculateInput(argc, argv);

      srv.request.color = input_pair.first;
      srv.request.number = input_pair.second;
    }
    else
    {
      ROS_ERROR("How dare you break my casino!");
      return 1;
    }

  }
}
