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

#include "gneintern.h"
#include "Mutex.h"

//##ModelId=3AE20A040384
Mutex::Mutex() {
  valassert(pthread_mutex_init( &mutex, NULL ), 0);
}

//##ModelId=3AE20A040385
Mutex::~Mutex() {
  valassert(pthread_mutex_destroy( &mutex ), 0);
}

//##ModelId=3AE20A070398
void Mutex::acquire() {
  valassert(pthread_mutex_lock( &mutex ), 0);
}

//##ModelId=3AE20A0D035C
void Mutex::release() {
  valassert(pthread_mutex_unlock( &mutex ), 0);
}


