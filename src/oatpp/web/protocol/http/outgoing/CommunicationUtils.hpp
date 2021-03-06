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

#ifndef oatpp_web_protocol_http_outgoing_CommunicationUtils_hpp
#define oatpp_web_protocol_http_outgoing_CommunicationUtils_hpp

#include "oatpp/web/protocol/http/incoming/Request.hpp"
#include "oatpp/web/protocol/http/outgoing/Response.hpp"

namespace oatpp { namespace web { namespace protocol { namespace http { namespace outgoing {
  
class CommunicationUtils {
public:
  static constexpr v_int32 CONNECTION_STATE_CLOSE = 0;
  static constexpr v_int32 CONNECTION_STATE_KEEP_ALIVE = 1;
  static constexpr v_int32 CONNECTION_STATE_UPGRADE = 2;
private:
  static bool headerEqualsCI_FAST(const oatpp::data::share::MemoryLabel& headerValue, const char* value);
public:
  
  /**
   * Consider keep connection alive taking into account request headers, response headers and protocol version.
   * Corresponding header will be set to response if not existed before
   * return one of (CONNECTION_STATE_CLOSE, CONNECTION_STATE_KEEP_ALIVE, CONNECTION_STATE_UPGRADE)
   */
  static v_int32 considerConnectionState(const std::shared_ptr<protocol::http::incoming::Request>& request,
                                         const std::shared_ptr<protocol::http::outgoing::Response>& response);
  
};
  
}}}}}

#endif /* CommunicationUtils_hpp */
