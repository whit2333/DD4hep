//==========================================================================
//  AIDA Detector description implementation 
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================

// Framework includes
#include "DD4hep/Printout.h"
#include "DD4hep/detail/ConditionsInterna.h"

// C/C++ include files
#include <climits>
#include <iomanip>
#include <cstdio>

using namespace std;
using namespace dd4hep;

/// Default constructor
Condition::Processor::Processor() {
}

/// Initializing constructor for a pure, undecorated conditions object
Condition::Condition(const std::string& nam, const std::string& typ) : Handle<Object>()
{
  Object* o = new Object();
  assign(o,nam,typ);
  o->hash = 0;
}

/// Initializing constructor for a pure, undecorated conditions object with payload buffer
Condition::Condition(const string& nam,const string& typ, size_t memory)
  : Handle<Object>()
{
  void* ptr = ::operator new(sizeof(Object)+memory);
  Object* o = new(ptr) Object();
  assign(o,nam,typ);
  o->hash = 0;
}

/// Output method
string Condition::str(int flags)  const   {
  stringstream output;
  Object* o = access();
  const IOV* ptr_iov = o->iovData();
  if ( 0 == (flags&NO_NAME) )
    output << setw(16) << left << o->name;
  if ( flags&WITH_IOV )
    output << " " << (ptr_iov ? ptr_iov->str().c_str() : "IOV:[UNKNOWN]");
  if ( flags&WITH_TYPE )
    output << " (" << o->type << ")";
  if ( flags&WITH_ADDRESS )
    output << " " << o->address;
  if ( flags&WITH_DATATYPE )
    output << " -> " << o->data.dataType();
  if ( flags&WITH_DATA )
    output << " Data:" << o->data.str();
  if ( flags&WITH_COMMENT )
    output << " \"" << o->comment << "\"";
  return output.str();
}

/// Access the data type
int Condition::dataType() const   {
  return access()->data.type;
}

/// Access the IOV block
dd4hep::OpaqueData& Condition::data() const   {
  return access()->data;
}

/// Access the IOV type
const dd4hep::IOVType& Condition::iovType() const   {
  return *(access()->iovType());
}

/// Access the IOV block
const dd4hep::IOV& Condition::iov() const   {
  return *(access()->iovData());
}

/// Access the type field of the condition
const string& Condition::type()  const   {
  return access()->type;
}

/// Access the value field of the condition as a string
const string& Condition::value()  const   {
  return access()->value;
}

/// Access the comment field of the condition
const string& Condition::comment()  const   {
  return access()->comment;
}

/// Access the address string [e.g. database identifier]
const string& Condition::address()  const   {
  return access()->address;
}

/// Access to the type information
const type_info& Condition::typeInfo() const   {
  return descriptor().type();
}

/// Hash identifier
Condition::key_type Condition::key()  const    {
  return access()->hash;
}

/// DetElement part of the identifier
Condition::detkey_type Condition::detector_key()  const   {
  return ConditionKey::KeyMaker(access()->hash).values.det_key;
}

/// Item part of the identifier
Condition::itemkey_type Condition::item_key()  const   {
  return ConditionKey::KeyMaker(access()->hash).values.item_key;
}

/// Flag operations: Set a conditons flag
void Condition::setFlag(mask_type option)   {
  access()->setFlag(option);
}

/// Flag operations: UN-Set a conditons flag
void Condition::unFlag(mask_type option)   {
  access()->unFlag(option);
}

/// Flag operations: Test for a given a conditons flag
bool Condition::testFlag(mask_type option) const {
  return access()->testFlag(option);
}

/// Access to the grammar type
const dd4hep::BasicGrammar& Condition::descriptor() const   {
  const BasicGrammar* g = access()->data.grammar;
  if ( !g ) {
    invalidHandleError<Condition>();
    // This code is never reached, since function above throws exception!
    // Needed to satisfay CppCheck
    throw runtime_error("Null pointer in Grammar object");
  }
  return *g;
}

/// Re-evaluate the conditions data according to the previous bound type definition
Condition& Condition::rebind()    {
  Object* o = access();
#if 0
  const IOV* i = o->iov;
  const IOVType* t = i->iovType;
  i->fromString(o->validity);
  if ( t != i->iovType )  {
    except("Condition","Rebinding condition with different IOV types [%d <> %d] is not allowed!",
           t ? t->type : -1, i->iovType ? i->iovType->type : -1);
  }
#endif
  o->data.fromString(o->value);
  printout(INFO,"Condition","+++ condition:%s rebinding value:%s",
           name(), o->value.c_str());
  return *this;
}

/// Default destructor. 
ConditionsSelect::~ConditionsSelect()   {
}

/// Constructor from string
ConditionKey::ConditionKey(DetElement detector, const string& value)  {
  KeyMaker m(detector.key(), detail::hash32(value));
  hash = m.hash;
#ifdef DD4HEP_CONDITIONKEY_HAVE_NAME
  name = detector.path()+"#"+value;
#endif
}

/// Constructor from detector element key and item sub-key
ConditionKey::ConditionKey(Condition::detkey_type det_key, const string& value)    {
  KeyMaker m(det_key, detail::hash32(value));
  hash = m.hash;
#ifdef DD4HEP_CONDITIONKEY_HAVE_NAME
  char text[32];
  ::snprintf(text,sizeof(text),"%08X#",det_key);
  name = text+value;
#endif
}

/// Constructor from detector element key and item sub-key
ConditionKey::ConditionKey(DetElement detector, Condition::itemkey_type item_key)  {
  hash = KeyMaker(detector.key(),item_key).hash;
#ifdef DD4HEP_CONDITIONKEY_HAVE_NAME
  char text[32];
  ::snprintf(text,sizeof(text),"#%08X",item_key);
  name = detector.path()+text;
#endif
}

/// Hash code generation from input string
Condition::key_type ConditionKey::hashCode(DetElement detector, const char* value)  {
  return KeyMaker(detector.key(), detail::hash32(value)).hash;
}

/// Hash code generation from input string
Condition::key_type ConditionKey::hashCode(DetElement detector, const string& value)  {
  return KeyMaker(detector.key(), detail::hash32(value)).hash;
}

/// 32 bit hashcode of the item
Condition::itemkey_type ConditionKey::itemCode(const char* value)  {
  return detail::hash32(value);
}

/// 32 bit hashcode of the item
Condition::itemkey_type ConditionKey::itemCode(const std::string& value)   {
  return detail::hash32(value);
}
