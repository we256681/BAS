#include "writefile.h"
#include <fstream>

void WriteStringToFile(const std::string& filename,const std::string& data)
{
    try
    {
        std::ofstream outfile(filename, std::ios::binary);
        if(outfile.is_open())
        {
            outfile<<data;
        }
        outfile.flush();
        outfile.close();
    }catch(...)
    {

    }
}

void WriteStringToFile(const std::wstring& filename,const std::string& data)
{
    try
    {
        std::ofstream outfile(filename, std::ios::binary);
        if(outfile.is_open())
        {
            outfile<<data;
        }
        outfile.flush();
        outfile.close();
    }catch(...)
    {

    }
}
