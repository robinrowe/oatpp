/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_concurrency_SpinLock_hpp
#define oatpp_concurrency_SpinLock_hpp

#include <atomic>
#include <thread>

namespace oatpp { namespace concurrency {

class SpinLock {
public:
  typedef std::atomic<bool> Atom;
private:
  Atom* m_atom;
public:
  
  SpinLock(Atom& atom);
  ~SpinLock();
  
  static void lock(Atom& atom);
  static void unlock(Atom& atom);
  
};
  
}}

#endif /* oatpp_concurrency_SpinLock_hpp */
