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

#include "Pattern.hpp"

#include "oatpp/core/data/stream/ChunkedBuffer.hpp"

namespace oatpp { namespace web { namespace url { namespace mapping {

const char* Pattern::Part::FUNCTION_CONST = "const";
const char* Pattern::Part::FUNCTION_VAR = "var";
const char* Pattern::Part::FUNCTION_ANY_END = "tail";

std::shared_ptr<Pattern> Pattern::parse(p_char8 data, v_int32 size){
  
  if(size <= 0){
    return nullptr;
  }
  
  auto result = Pattern::createShared();

  v_int32 lastPos = 0;
  v_int32 i = 0;
  
  while(i < size){
    
    v_char8 a = data[i];
    
    if(a == '/'){
      
      if(i - lastPos > 0){
        auto part = Part::createShared(Part::FUNCTION_CONST, oatpp::String((const char*)&data[lastPos], i - lastPos, true));
        result->m_parts->pushBack(part);
      }
      
      lastPos = i + 1;
      
    } else if(a == '*'){
      lastPos = i + 1;
      if(size > lastPos){
        auto part = Part::createShared(Part::FUNCTION_ANY_END, oatpp::String((const char*)&data[lastPos], size - lastPos, true));
        result->m_parts->pushBack(part);
      }else{
        auto part = Part::createShared(Part::FUNCTION_ANY_END, oatpp::String(0));
        result->m_parts->pushBack(part);
      }
      return result;
    
    } else if(a == '{'){
      
      lastPos = i + 1;
      while(i < size && data[i] != '}'){
        i++;
      }
      
      if(i > lastPos){
        auto part = Part::createShared(Part::FUNCTION_VAR, oatpp::String((const char*)&data[lastPos], i - lastPos, true));
        result->m_parts->pushBack(part);
      }else{
        auto part = Part::createShared(Part::FUNCTION_VAR, oatpp::String(0));
        result->m_parts->pushBack(part);
      }
      
      lastPos = i + 1;
      
    }
    
    i++;
    
  }
  
  if(i - lastPos > 0){
    auto part = Part::createShared(Part::FUNCTION_CONST, oatpp::String((const char*)&data[lastPos], i - lastPos, true));
    result->m_parts->pushBack(part);
  }
  
  return result;
}

std::shared_ptr<Pattern> Pattern::parse(const char* data){
  return parse((p_char8) data, (v_int32) std::strlen(data));
}

std::shared_ptr<Pattern> Pattern::parse(const oatpp::String& data){
  return parse(data->getData(), data->getSize());
}
  
v_char8 Pattern::findSysChar(oatpp::parser::ParsingCaret& caret) {
  auto data = caret.getData();
  for (v_int32 i = caret.getPosition(); i < caret.getSize(); i++) {
    v_char8 a = data[i];
    if(a == '/' || a == '?') {
      caret.setPosition(i);
      return a;
    }
  }
  caret.setPosition(caret.getSize());
  return 0;
}
  
bool Pattern::match(const StringKeyLabel& url, MatchMap& matchMap) {
  
  oatpp::parser::ParsingCaret caret(url.getData(), url.getSize());
  
  if(m_parts->count() == 0){
    
    if(caret.skipChar('/')){
      return false;
    }
    
    return true;
    
  }
  
  auto curr = m_parts->getFirstNode();
  
  while(curr != nullptr){
    const std::shared_ptr<Part>& part = curr->getData();
    curr = curr->getNext();
    caret.skipChar('/');
    
    if(part->function == Part::FUNCTION_CONST){
      
      if(!caret.proceedIfFollowsText(part->text->getData(), part->text->getSize())){
        return false;
      }
      
      if(caret.canContinue() && !caret.isAtChar('/')){
        if(caret.isAtChar('?') && (curr == nullptr || curr->getData()->function == Part::FUNCTION_ANY_END)) {
          matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getSize() - caret.getPosition());
          return true;
        }
        return false;
      }
      
    }else if(part->function == Part::FUNCTION_ANY_END){
      if(caret.getSize() > caret.getPosition()){
        matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getSize() - caret.getPosition());
      }
      return true;
    }else if(part->function == Part::FUNCTION_VAR){
      
      if(!caret.canContinue()){
        return false;
      }
      
      oatpp::parser::ParsingCaret::Label label(caret);
      v_char8 a = findSysChar(caret);
      if(a == '?') {
        if(curr == nullptr || curr->getData()->function == Part::FUNCTION_ANY_END) {
          matchMap.m_variables[part->text] = StringKeyLabel(url.getMemoryHandle(), label.getData(), label.getSize());
          matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getSize() - caret.getPosition());
          return true;
        }
        caret.findChar('/');
      }
      
      matchMap.m_variables[part->text] = StringKeyLabel(url.getMemoryHandle(), label.getData(), label.getSize());
      
    }
    
  }
  
  caret.skipChar('/');
  if(caret.canContinue()){
    return false;
  }
  
  return true;
  
}

oatpp::String Pattern::toString() {
  auto stream = oatpp::data::stream::ChunkedBuffer::createShared();
  auto curr = m_parts->getFirstNode();
  while (curr != nullptr) {
    const std::shared_ptr<Part>& part = curr->getData();
    curr = curr->getNext();
    if(part->function == Part::FUNCTION_CONST) {
      stream->write("/", 1);
      stream->data::stream::OutputStream::write(part->text);
    } else if(part->function == Part::FUNCTION_VAR) {
      stream->write("/{", 2);
      stream->data::stream::OutputStream::write(part->text);
      stream->write("}", 1);
    } else if(part->function == Part::FUNCTION_ANY_END) {
      stream->write("/*", 2);
    }
  }
  return stream->toString();
}
  
}}}}
