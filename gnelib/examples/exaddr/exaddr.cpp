/* GNE - Game Networking Engine, a portable multithreaded networking library.
 * Copyright (C) 2001 Jason Winnebeck (gillius@mail.rit.edu)
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
 * exaddr
 * Small test/example using the Address class.
 */

#include "../../include/gnelib.h"
#include <iostream>

using namespace std;
using namespace GNE;
using namespace GNE::Console;

void displayAddress(const Address& addr);

int main(int argc, char* argv[]) {
  if (initGNE(NL_IP, atexit)) {
    exit(1);
  }
  initConsole(atexit);
  setTitle("GNE Address Class Test/Example");

  string url = "www.microsoft.com:80";

  //Explicit url resolving
  gout << url << " resolves to: " << Address(url).toString() << endl;
  //Test implicit conversion from string to Address
  gout << "localhost:1234 resolves to: ";
  displayAddress("localhost:1234");
  gout << endl;

  //Test other functions
  Address addr("145.64.129.123");
  gout << "145.64.129.123 resolves to: " << addr << endl;
  gout << "Port is: " << addr.getPort() << endl;
  gout << "Setting port to 15:";
  addr.setPort(15);
  gout << " address is now: " << addr << endl;

  gout << "Trying to set port to -516 (invalid)";
  addr.setPort(-516);
  if (addr)
    gout << " address still valid (NOT CORRECT)." << endl;
  else
    gout << " address no longer valid (this is correct)." << endl;

  Address rTest("www.yahoo.com:80");
  gout << "Testing reverse DNS: www.yahoo.com:80 resolves to " << rTest << endl;
  gout << rTest << " reverses to " << rTest.getNameByAddress() << endl;

  gout << "Try picking your own address: ";
  gin >> url;
  gout << url << " resolves to: " << Address(url) << endl;
  gout << Address(url) << " reverses to " << Address(url).getNameByAddress() << endl;
  gout << "Press a key to continue." << endl;
  getch();

  return 0;
}

void displayAddress(const Address& addr) {
  gout << addr;
}
