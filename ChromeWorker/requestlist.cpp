#include "requestlist.h"
#include <chrono>
#include "multithreading.h"


using namespace std::chrono;

void RequestList::Add(const std::string& id)
{
    Requests[id] = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count();
    //WORKER_LOG("!!!!!!!!!!! + " + std::to_string(Size()) + " " + Report());
}

void RequestList::Remove(const std::string& id)
{
    Requests.erase(id);
    //WORKER_LOG("!!!!!!!!!!! - " + std::to_string(Size()) + " " + Report());
}

void RequestList::RemoveAll()
{
    Requests.clear();
    //WORKER_LOG("!!!!!!!!!!! x " + std::to_string(Size()) + " " + Report());
}

void RequestList::RemoveOld()
{
    long long limit = duration_cast< milliseconds >( system_clock::now().time_since_epoch() ).count() - 120000;
    auto i = Requests.begin();
    while (i != Requests.end())
    {
        if(i->second < limit)
            i = Requests.erase(i);
        else
            i++;
    }

    //WORKER_LOG("!!!!!!!!!!! * " + std::to_string(Size()) + " " + Report());
}

long long RequestList::Oldest()
{
    long long res = 0;
    for(std::map<std::string,long long>::iterator it = Requests.begin(); it != Requests.end(); ++it)
    {
        if(it->second < res || res == 0)
            res = it->second;
    }
    return res;
}

std::string RequestList::Report()
{
    std::string res;
    for(std::map<std::string,long long>::iterator it = Requests.begin(); it != Requests.end(); ++it)
    {
        if(!res.empty())
            res += ",";
        res += it->first;
    }
    return res;
}

int RequestList::Size()
{
    return Requests.size();
}


