#include <string>
#include <iostream>
#include <sstream>

#include <sys/socket.h>	
#include <arpa/inet.h>	
#include <unistd.h>
#include "LineInfo.h"

using namespace std;

const int MESSAGE_MAX_SIZE = 1024;

const int ERROR = -1;

const int SOCK_PORT = 8888;

const string LOCAL_HOST_ADDRESS = "127.0.0.1";

const int MAX_GUESS_LIMIT = 100;

const int MAX_NO_TRIES = 12;

int main(int argc, char* argv[]){

  int socketServerId;
  sockaddr_in server;
  string messagestr;

  try{

    // create a socket

    if((socketServerId = socket(AF_INET, SOCK_STREAM,0)) == ERROR){

      throw domain_error(LineInfo("Error: Failed to create socket", __FILE__, __LINE__));
    }

    server.sin_addr.s_addr = inet_addr(LOCAL_HOST_ADDRESS.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(SOCK_PORT);

    //connect to the remote server 
    if((connect(socketServerId,(struct sockaddr*) &server, sizeof(server))) == ERROR){

      throw domain_error(LineInfo("Socket Connect failure ", __FILE__, __LINE__));
    }

    cout << endl << endl << "Try to guess the number b/w 1 and " << MAX_GUESS_LIMIT << endl << endl;

    int nooftries = 0;
    int guessno; 

    //keep communicating with the server

    do{

      cout << "Current try number: " << ++nooftries << endl;
      cout << "Enter the number guessed " << endl << endl;
      cin >> guessno;

      stringstream gnoss;
      gnoss << guessno;
      string guessNoStr = gnoss.str();

      if((write(socketServerId,guessNoStr.c_str(),guessNoStr.size()+1)) == ERROR){

        throw domain_error(LineInfo("Socket write failure ", __FILE__, __LINE__));
      }

       // read from socketServerId the receiveMessage
      char receiveMessage[MESSAGE_MAX_SIZE]={0};
      if((read(socketServerId,receiveMessage,MESSAGE_MAX_SIZE)) == ERROR){

        throw domain_error(LineInfo("Socket read failure ", __FILE__, __LINE__));
      }


      string recieveMessageStr = receiveMessage; 
      cout << endl << "Server reply:" << endl << endl << recieveMessageStr << endl << endl;

      // Check if a Win Message
        if (recieveMessageStr.find("Win") != string::npos)
            break;


        if(nooftries == MAX_NO_TRIES){

          cout << endl << "The maximum number of tries " << MAX_NO_TRIES << " is done. " << endl;

          if((write(socketServerId,"Send random number", sizeof("Send random number") + 1)) == ERROR){

            throw domain_error(LineInfo("Socket write failure ", __FILE__, __LINE__));
            
          }

          char randomNoMessage[MESSAGE_MAX_SIZE] = {0};
          if((read(socketServerId,randomNoMessage,MESSAGE_MAX_SIZE )) == ERROR){

                      throw domain_error(LineInfo("Socket read failure ", __FILE__, __LINE__));
          }

          int randomno = stoi(randomNoMessage);
            
            //display the random no and state the game is over 

            cout << "The random number is " << randomno << endl << endl;
            cout << "The game is over" << endl << endl;

            break;

          
        }

      

      
    }while (true);

     if (close(socketServerId) == ERROR)
        throw domain_error(LineInfo("Socket close failure ", __FILE__, __LINE__));
  }

    catch (exception& e) {
      cout << e.what() << endl;
      exit(EXIT_FAILURE);
}

    exit(EXIT_SUCCESS);

}