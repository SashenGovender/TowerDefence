#include "JsonHelper.h"
//-----------------------------------------------------------------------------------
JsonHelper::JsonHelper()
{

}
JsonHelper::JsonHelper(Json::Value* jData)
{
   _jsonData = *jData;
}
//-----------------------------------------------------------------------------------
bool JsonHelper::Deserialise(const char *data)
{
   if (data == NULL || std::strlen(data) == 0)
   {
      return false;
   }

   Json::Reader reader;
   bool success = reader.parse(data, _jsonData);

   return success;
}

void JsonHelper::Release()
{
   delete this;
}

bool JsonHelper::IsValidIndex(int key)
{
   if (!_jsonData.isArray())
   {
      return false;
   }
   if (key >= static_cast<int>(_jsonData.size()))
   {
      return false;
   }
   return true;
}

void JsonHelper::Serialise(std::string& jsonStringBuffer)
{
   Json::FastWriter writer;
   jsonStringBuffer = writer.write(_jsonData);
}
//-----------------------------------------------------------------------------------

Json::Value* JsonHelper::GetJsonMainObject()
{
   return &_jsonData;
}

int JsonHelper::GetInt(const char* key)
{
   if (!_jsonData[key].isInt())
   {
      return -1;
   }
   return _jsonData[key].asInt();
}

int JsonHelper::GetInt(int key)
{
   if (!IsValidIndex(key))
   {
      return -1;
   }

   if (!_jsonData[key].isInt())
   {
      return -1;
   }
   return _jsonData[key].asInt();
}

double JsonHelper::GetDouble(int key)
{
   if (!IsValidIndex(key))
   {
      return -1;
   }
   if (!_jsonData[key].isDouble())
   {
      return -1;
   }
   return _jsonData[key].asDouble();
}

double JsonHelper::GetDouble(const char* key)
{
   if (!_jsonData[key].isDouble())
   {
      return -1;
   }
   return _jsonData[key].asDouble();
}

int JsonHelper::GetCString(char* key, char* buffer, int bufferSize)
{
   if (!_jsonData[key].isString())
   {
      return -1;
   }
   const char* data = _jsonData[key].asCString();
   if (bufferSize == 0 || buffer == nullptr)
   {
      return  std::strlen(data);
   }
   else if (bufferSize > static_cast<int>(std::strlen(data)))
   {
      std::memcpy(buffer, data, bufferSize);
      return  std::strlen(data);
   }

   return 0;
}

int JsonHelper::GetCString(int key, char* buffer, int bufferSize)
{
   if (!IsValidIndex(key))
   {
      return -1;
   }
   if (!_jsonData[key].isString())
   {
      return -1;
   }

   const char* data = _jsonData[key].asCString();
   if (bufferSize == 0 || buffer == nullptr)
   {
      return  std::strlen(data);
   }
   else if (bufferSize > static_cast<int>(std::strlen(data)))
   {
      std::memcpy(buffer, data, bufferSize);
      return  std::strlen(data);
   }

   return 0;
}

std::string JsonHelper::GetString(const char* key)
{
   if (!_jsonData[key].isString())
   {
      return "";
   }
   std::string data = _jsonData[key].asString();
   return data;
}

std::string JsonHelper::GetString(int key)
{
   if (!IsValidIndex(key))
   {
      return "";
   }
   if (!_jsonData[key].isString())
   {
      return "";
   }

   std::string data = _jsonData[key].asString();
   return data;
}

bool JsonHelper::GetBool(int key)
{
   if (!IsValidIndex(key))
   {
      return false;
   }
   if (!_jsonData[key].isBool())
   {
      return false;
   }
   return _jsonData[key].asBool();
}

bool JsonHelper::GetBool(const char* key)
{
   if (!_jsonData[key].isBool())
   {
      return false;
   }
   return _jsonData[key].asBool();
}

JsonHelper* JsonHelper::GetJsonObject(const char* key)
{
   if (!_jsonData[key].isObject())
   {
      return nullptr;
   }
   Json::Value data = _jsonData[key];
   JsonHelper* copyData = new  JsonHelper(&data);
   return copyData;
}

JsonHelper* JsonHelper::GetJsonObject(int key)
{
   if (!IsValidIndex(key))
   {
      return nullptr;
   }
   if (!_jsonData[key].isObject())
   {
      return nullptr;
   }
   Json::Value data = _jsonData[key];
   JsonHelper* copyData = new  JsonHelper(&data);
   return copyData;
}
int JsonHelper::GetArraySize(const char* key)
{
   if (!_jsonData[key].isArray())
   {
      return -1;
   }
   else
   {
      return _jsonData[key].size();
   }
}
int JsonHelper::GetArraySize(int key)
{
   if (!IsValidIndex(key))
   {
      return -1;
   }
   if (!_jsonData[key].isArray())
   {
      return -1;
   }

   return _jsonData[key].size();
}
Json::Value JsonHelper::GetArrayValue(int key, int index)
{
   if (!IsValidIndex(key))
   {
      return -1;
   }
   if (!_jsonData[key].isArray())
   {
      return -1;
   }

   return _jsonData[key][index];
}

JsonHelper* JsonHelper::GetArray(const char* key)
{
   if (!_jsonData[key].isArray())
   {
      return nullptr;
   }

   Json::Value data = _jsonData[key];
   JsonHelper* copyData = new  JsonHelper(&data);
   return copyData;
}

JsonHelper* JsonHelper::GetArray(int key)
{
   if (!IsValidIndex(key))
   {
      return nullptr;
   }
   if (!_jsonData[key].isArray())
   {
      return nullptr;
   }

   Json::Value data = _jsonData[key];
   JsonHelper* copyData = new  JsonHelper(&data);
   return copyData;
}

//-----------------------------------------------------------------------------------
void JsonHelper::AddInt(int key, int value)
{
   _jsonData[key] = value;
}

void JsonHelper::AddInt(char *key, int value)
{
   _jsonData[key] = value;
}

void JsonHelper::AddDouble(int key, double value)
{
   _jsonData[key] = value;
}

void JsonHelper::AddDouble(char *key, double value)
{
   _jsonData[key] = value;
}

void JsonHelper::AddString(int key, char* value)
{
   _jsonData[key] = value;
}

void JsonHelper::AddString(char *key, char* value)
{
   _jsonData[key] = value;
}

void JsonHelper::AddBool(int key, bool value)
{
   _jsonData[key] = value;
}

void JsonHelper::AddBool(char *key, bool value)
{
   _jsonData[key] = value;
}

void JsonHelper::AddJsonObject(int key, JsonHelper* object)
{
   _jsonData[key] = reinterpret_cast<Json::Value*>(object->GetJsonMainObject());
}

void JsonHelper::AddJsonObject(char* key, JsonHelper* object)
{
   _jsonData[key] = reinterpret_cast<Json::Value*>(object->GetJsonMainObject());
}

//-----------------------------------------------------------------------------------