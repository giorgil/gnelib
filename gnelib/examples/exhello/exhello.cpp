/* GNE - Game Networking Engine, a portable multithreaded networking library.
 * Copyright (C) 2001 Jason Winnebeck (gillius@webzone.net)
 * Project website: http://www.rit.edu/~jpw9607/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * exhello
 * Everyone knows the "Hello World" program.  Perhaps it's been coded
 * too many times.  However, the GNE coders just plain don't care ;).
 * This is an example to show the minimum code needed to connect and send
 * data, in this case, "Hello Server (or Client)!" to the other end of the
 * connection.
 * Well... perhaps not the minimum.  Lots of errors can happen over network
 * connections that we have to handle for this program to properly execute
 * when conditions aren't perfectly ideal, like when we can't reach the
 * server.
 */

#include "../../src/gnelib.h"
#include <iostream>
#include <cstring>

using namespace std;

void errorExit(const char* error);
int getPort(const char* prompt);
void displayAddress();
string getAddressString(NLaddress& addr);
void doServer(int outRate, int inRate, int port);
void doClient(int outRate, int inRate, int port);

class OurClient : public ClientConnection {
public:
  OurClient(int outRate, int inRate, NLaddress address) 
    : ClientConnection(outRate, inRate, address) {}

  virtual ~OurClient() {}
private:
};

class OurServer : public ServerConnection {
public:
  OurServer(int outRate, int inRate) 
    : ServerConnection(outRate, inRate) {
  }

  virtual ~OurServer() {}

  void onNewConn(Connection* newConn) {
  }

private:
};

int main(int argc, char* argv[]) {
  GNE::init(NL_IP, atexit);
  GNE::setUserVersion(1); //sets our user protocol version number, used in
                          //the connection process by GNE to version check.
  Console::init(atexit);
  Console::setTitle("GNE Basic Connections Example");

  //It's okay to use iostreams instead of the Console functions when we are
  //only accessing the console from one thread.
  displayAddress();
  cout << "Should we act as the server, or the client?" << endl;
  cout << "Type 1 for client, 2 for server: ";
  int type;
  cin >> type;

  int port;
  if (type == 2) {
    Console::setTitle("GNE Basic Connections Example -- Server");
    cout << "Reminder: ports <= 1024 on UNIX can only be used by the superuser." << endl;
    port = getPort("listen on");
    doServer(3000, 3000, port);
  } else {
    Console::setTitle("GNE Basic Connections Example -- Client");
    port = getPort("connect to");
    doClient(3000, 3000, port);
  }

  return 0;
}

void errorExit(const char* error) {
  cout << "Error: " << error << endl;
  exit(1);
}

int getPort(const char* prompt) {
  int port;
  cout << "Which port should we " << prompt << ": ";
  cin >> port;
  while ((port < 1) || (port > 65535)) {
    cout << "Ports range from 1 to 65535, please select one in this range: ";
    cin >> port;
  }
  return port;
}

void displayAddress() {
  NLaddress ourAddr = GNE::getLocalAddress();
  char buf[NL_MAX_STRING_LENGTH];
  nlAddrToString(&ourAddr, (NLbyte*)buf);
  char* temp = strchr(buf, ':');
  if (temp)
    *temp = '\0';
  cout << "Local address: " << buf << endl;
}

string getAddressString(NLaddress& addr) {
  char buf[NL_MAX_STRING_LENGTH];
  nlAddrToString(&addr, (NLbyte*)buf);
  return string(buf);
}

void doServer(int outRate, int inRate, int port) {
  OurServer server(outRate, inRate);
  if (server.open(port))
    errorExit("Cannot open server socket.");
  if (server.listen())
    errorExit("Cannot listen on server socket.");

  cout << "Server is listening on: " << getAddressString(server.getLocalAddress()) << endl;
}

void doClient(int outRate, int inRate, int port) {
  string host;
  cout << "Enter hostname or IP address: ";
  cin >> host;

  NLaddress addr;
  nlGetAddrFromName((NLbyte*)host.c_str(), &addr);
  nlSetAddrPort(&addr, (NLushort)port);
  
  cout << "Connecting to: " << getAddressString(addr) << endl;
  OurClient client(outRate, inRate, addr);
}