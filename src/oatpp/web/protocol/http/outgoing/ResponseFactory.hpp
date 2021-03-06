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

#ifndef oatpp_web_protocol_http_outgoing_ResponseFactory_hpp
#define oatpp_web_protocol_http_outgoing_ResponseFactory_hpp

#include "./Response.hpp"

#include "oatpp/core/data/mapping/ObjectMapper.hpp"
#include "oatpp/core/data/mapping/type/Type.hpp"
#include "oatpp/core/data/stream/ChunkedBuffer.hpp"

namespace oatpp { namespace web { namespace protocol { namespace http { namespace outgoing {
  
class ResponseFactory {
public:
  
  static std::shared_ptr<Response> createShared(const Status& status, const oatpp::String& text);
  static std::shared_ptr<Response> createShared(const Status& status, const std::shared_ptr<oatpp::data::stream::ChunkedBuffer>& segBuffer);
  static std::shared_ptr<Response> createShared(const Status& status,
                              const oatpp::data::mapping::type::AbstractObjectWrapper& dto,
                              oatpp::data::mapping::ObjectMapper* objectMapper);
  
};
  
}}}}}

#endif /* oatpp_web_protocol_http_outgoing_ResponseFactory_hpp */
