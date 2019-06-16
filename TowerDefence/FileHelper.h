#ifndef FILEHELPER
#define FILEHELPER
#include <string>

class FileHelper
{
private:

public:
   std::string ReadInformation(std::string filepath);
   void WriteInformation(std::string filepath,std::string jsonData);
   void WriteInformation(std::string filePath, std::string data, bool append);
};
#endif