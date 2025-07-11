#include "asyncresult.h"

int AsyncResult::GetActionUniqueId()
{
    return ActionUniqueId;
}

void AsyncResult::SetActionUniqueId(int ActionUniqueId)
{
    this->ActionUniqueId = ActionUniqueId;
}

void AsyncResult::Interrupt()
{
    IsFinished = true;
    IsSuccess = false;
    this->ErrorMessage = "Interrupt";
    this->ErrorCode = "Interrupt";
}

void AsyncResult::Fail(const std::string& ErrorMessage, const std::string& ErrorCode)
{
    IsFinished = true;
    IsSuccess = false;
    this->ErrorMessage = ErrorMessage;
    this->ErrorCode = ErrorCode;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Fail(const std::string& ErrorMessage, const std::string& ErrorCode, bool Result)
{
    IsFinished = true;
    IsSuccess = false;
    this->ErrorMessage = ErrorMessage;
    this->ErrorCode = ErrorCode;
    IsBoolean = true;
    BooleanValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Fail(const std::string& ErrorMessage, const std::string& ErrorCode, int Result)
{
    IsFinished = true;
    IsSuccess = false;
    this->ErrorMessage = ErrorMessage;
    this->ErrorCode = ErrorCode;
    IsInteger = true;
    IntegerValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Fail(const std::string& ErrorMessage, const std::string& ErrorCode, const std::string& Result)
{
    IsFinished = true;
    IsSuccess = false;
    this->ErrorMessage = ErrorMessage;
    this->ErrorCode = ErrorCode;
    IsString = true;
    StringValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Fail(const std::string& ErrorMessage, const std::string& ErrorCode, const std::map<std::string, std::string>& Result)
{
    IsFinished = true;
    IsSuccess = false;
    this->ErrorMessage = ErrorMessage;
    this->ErrorCode = ErrorCode;
    IsMap = true;
    MapValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Fail(const std::string& ErrorMessage, const std::string& ErrorCode, const std::vector<std::string>& Result)
{
    IsFinished = true;
    IsSuccess = false;
    this->ErrorMessage = ErrorMessage;
    this->ErrorCode = ErrorCode;
    IsList = true;
    ListValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Success()
{
    IsFinished = true;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Success(bool Result)
{
    IsFinished = true;
    IsBoolean = true;
    BooleanValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Success(int Result)
{
    IsFinished = true;
    IsInteger = true;
    IntegerValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Success(const std::string& Result)
{
    IsFinished = true;
    IsString = true;
    StringValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Success(const std::map<std::string, std::string>& Result)
{
    IsFinished = true;
    IsMap = true;
    MapValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::Success(const std::vector<std::string>& Result)
{
    IsFinished = true;
    IsList = true;
    ListValue = Result;
    if(HasCallback)
        Callback(this);
}

void AsyncResult::SetResult(bool Result)
{
    IsBoolean = true;
    BooleanValue = Result;
}

void AsyncResult::SetResult(int Result)
{
    IsInteger = true;
    IntegerValue = Result;
}

void AsyncResult::SetResult(const std::string& Result)
{
    IsString = true;
    StringValue = Result;
}

void AsyncResult::SetResult(const std::map<std::string, std::string>& Result)
{
    IsMap = true;
    MapValue = Result;
}

void AsyncResult::SetResult(const std::vector<std::string>& Result)
{
    IsList = true;
    ListValue = Result;
}

bool AsyncResult::GetIsFinished()
{
    return IsFinished;
}

bool AsyncResult::GetIsSuccess()
{
    return IsSuccess;
}

std::string AsyncResult::GetErrorMessage()
{
    return ErrorMessage;
}

std::string AsyncResult::GetErrorCode()
{
    return ErrorCode;
}

bool AsyncResult::GetIsBoolean()
{
    return IsBoolean;
}

bool AsyncResult::GetBoolean()
{
    return BooleanValue;
}

bool AsyncResult::GetIsInteger()
{
    return IsInteger;
}

int AsyncResult::GetInteger()
{
    return IntegerValue;
}

bool AsyncResult::GetIsString()
{
    return IsString;
}

std::string AsyncResult::GetString()
{
    return StringValue;
}

std::string AsyncResult::GetRawData()
{
    return RawData;
}

void AsyncResult::SetRawData(const std::string& RawData)
{
    this->RawData = RawData;
}

bool AsyncResult::GetIsList()
{
    return IsList;
}

std::vector<std::string> AsyncResult::GetList()
{
    return ListValue;
}

void AsyncResult::Then(std::function<void(AsyncResult*)> Callback)
{
    this->Callback = Callback;
    HasCallback = true;
    if(GetIsFinished())
    {
        Callback(this);
    }
}
