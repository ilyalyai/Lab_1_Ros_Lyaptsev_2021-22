#include "ros/ros.h"
#include <stdlib.h>
#include "ros_lab1/gambling_table.h"
#include <iostream>
#include <string>
#include "std_msgs/String.h"
using namespace std;
using namespace ros;

void printMessage(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("By the way, look what i've found in logs: [%s]", msg->data.c_str());
  cout<<msg->data;
}

pair<string, int32_t> calculateInput(int32_t argc, char **argv)
{
  pair<string, int32_t> input;
  string user_bet = argv[1];
  if(argc == 3)
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

pair<string, int32_t> calculateInputString(int32_t argc, string*argv)
{
  char* argv_new[4];
  for(int i = 0; i<argc; i++)
  {
    argv_new[i] = &(argv[i])[0];
  }
  return calculateInput(argc, argv_new);
}

int main(int argc, char **argv)
{
  //Дадим игроку немного денег, чтобы было на что играть
  int32_t money = 1000;
  //3- цвет и номер, 2- чет/нечет, только цвет
  if (argc != 4 && argc != 3)
  {
    ROS_INFO("usage: player <string>color <uint32_t>number <uint32_t>bet");
    ROS_INFO("player <string>color <uint32_t>number <uint32_t>bet");
    ROS_INFO("player <string>color <uint32_t>bet");
    ROS_INFO("player <string>\"odd/even/passe/manque\" <uint32_t>bet");
    return 1;
  }

  //Параметров у нас 3 или 4, но третий- всегда число
  if(argc == 4 && atoll(argv[2]) > 36)
  {
    ROS_INFO("Wrong number: 0 <= number <= 36\n");
    return 1;
  }

  //Это позволит определить, на что поставил игрок, и правильно ли он вообще написал
  pair<string, int32_t> input = calculateInput(argc, argv);

  if(input.first.empty())
  {
    ROS_INFO("Sorry, couldn't recognise you're input\n");
    return 1;
  }

  ROS_INFO("All bets are off!");
  ROS_INFO("Ow, you're new here! Welcome bonus for beginners - 1000 coins!\n");

  int32_t request_number = input.second;
  string request_color = input.first;

  //Запуск всех полезных штук, инициализация сервиса для записи значений
  init(argc, argv, "player");
  NodeHandle n;

  //Ловим также сообщения из топика
  string topic_name;
  n.getParam("/casino_topic_name", topic_name);
  Subscriber sub = n.subscribe(topic_name, 1000, printMessage);

  string service_name;
  n.getParam("/casino_service_name", service_name);
  ServiceClient client = n.serviceClient<ros_lab1::gambling_table>(service_name);
  ros_lab1::gambling_table srv;

  srv.request.color = request_color;
  srv.request.number = request_number;
  srv.request.bet = atoll(argv[argc-1]);

  //Аддин, чтобы играть можно было бесконечно
  while(ok())
  {
    while(sub.getNumPublishers() <= 0){}
    if (client.call(srv))
    {
      money += srv.response.prize;
      //Победили
      if(srv.response.result)
      {
        int32_t response = rand()%2;
        string responce_text[3] = {"Remember: the house always wins, maybe it's time to leave?",
                                   "Veni, Vidi, Vici",
                                   "Viva Ros Vegas!"};
        cout << responce_text[response] << endl;
        cout << "You now have " << money << endl;
      }
      //Проиграли
      else
      {
        int32_t response = rand()%2;
        string responce_text[3] = {"The game was rigged from the start!",
                                   "How can a sealed deck be laid out in a different order?!?",
                                   "That's the truth about Las Vegas: we're the only winners. The players don't stand a chance."};
        cout << responce_text[response] << endl;
        cout << "You now have " << money << endl<< endl;

      }
      //Поймаем ка мы callback от subscriber'a
      spinOnce();

      if(money > 2000 || money <= 0)
      {
        cout << "NOW GET OUT OF HERE!!!" << endl;
        return 1;
      }

      //Определю новую ставку
      cout << "If you want to play again, input" << endl << "<uint32_t>bet, enter, <string>color, enter, <uint32_t>number" << endl;
      cout << "If you want to throw up you're cards, type \"surrender\"" << endl;
      string bet;
      cin >> bet;

      if(bet.find("surrender") != std::string::npos)
        return 1;

      if(atoll(&bet[0]) == 0)
      {
        cout << "Hey, what are you trying to do?!?" << endl;
        return 1;
      }

      cout << "Well, on what do you want to bet?" << endl;

      string input;
      cin >> input;

      string new_bet[4];

      new_bet[0] = "Ilya's casino";
      new_bet[1] = input;

      cout << "If you want to bet on number, input it, otherwise input \"bet\"" << endl;
      cin >> input;

      if(input.find("bet") != std::string::npos)
        argc = 3;
      else
      {
        argc = 4;
        new_bet[2] = input;
      }

      pair<string, int32_t> input_pair = calculateInputString(argc, new_bet);

      srv.request.color = input_pair.first;
      srv.request.number = input_pair.second;
      srv.request.bet = atoll(&bet[0]);

      ROS_INFO("All bets are off!");
    }
    else
    {
      ROS_ERROR("How dare you break my casino!");
      return 1;
    }
  }
}
