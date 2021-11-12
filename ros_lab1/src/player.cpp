#include "ros/ros.h"
#include <stdlib.h>
#include "ros_lab1/gambling_table.h"
#include <iostream>
#include <string>
using namespace std;
using namespace ros;

int main(int argc, char **argv)
{
  if (argc != 3)
  {
    ROS_INFO("usage: player <string>color <uint>number");
    return 1;
  }

  init(argc, argv, "house");
  NodeHandle n;
  ServiceClient client = n.serviceClient<ros_lab1::gambling_table>("gambling_table");
  ros_lab1::gambling_table srv;
  srv.request.color = argv[1];
  srv.request.number = atoll(argv[2]);
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
      string new_color;
      int new_number;
      cin >> new_color;
      if(new_color.find("surrender") != std::string::npos)
        return 1;
      cin >> new_number;
      srv.request.color = new_color;
      srv.request.number = new_number;
    }
    else
    {
      ROS_ERROR("How dare you break my casino!");
      return 1;
    }

  }
}
