#include<fstream>
#include <sstream>
#include "FileHelper.h"

std::string FileHelper::ReadInformation(std::string filePath)
{
   std::ifstream ifs;
   ifs.open(filePath);

   std::stringstream bufferStream;
   bufferStream << ifs.rdbuf();
   std::string bufferFileData = bufferStream.str();

   ifs.close();

   return bufferFileData;
}

void FileHelper::WriteInformation(std::string filePath, std::string jsonData)
{
   std::ofstream ofs;
   ofs.open(filePath);

   for (int index = 0; index < static_cast<int>(jsonData.size()); index++)
   {
      ofs << jsonData.at(index);
   }

   ofs.close();
}

void FileHelper::WriteInformation(std::string filePath, std::string data, bool append)
{
   std::ofstream ofs;
   if (append)
   {
      ofs.open(filePath, std::ios_base::app);
   }
   else
   {
      ofs.open(filePath);
   }

   ofs << data;

   ofs.close();
}
