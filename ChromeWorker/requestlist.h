#ifndef REQUESTLIST_H
#define REQUESTLIST_H

#include <map>
#include <string>
#include <vector>

class RequestList
{
    std::map<std::string,long long> Requests;
    std::string Report();

public:
    void Add(const std::string& id);
    void Remove(const std::string& id);
    void RemoveOld();
    void RemoveAll();
    long long Oldest();
    int Size();

};

#endif // REQUESTLIST_H
