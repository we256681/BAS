
#ifndef REQUESTRESTRICTION_H
#define REQUESTRESTRICTION_H

#include <string>

struct RequestRestriction
{
    std::string Mask;
    bool IsAllow;
};

#endif // REQUESTRESTRICTION_H