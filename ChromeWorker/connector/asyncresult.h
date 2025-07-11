#ifndef ASYNCRESULT_H
#define ASYNCRESULT_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>

class AsyncResult
{
    
    private:
        bool IsSuccess = true;
        std::string ErrorMessage;
        std::string ErrorCode;

        bool IsFinished = false;

        bool IsBoolean = false;
        bool BooleanValue = false;

        bool IsInteger = false;
        int IntegerValue = 0;

        bool IsString = false;
        std::string StringValue;

        std::string RawData;

        bool IsMap = false;
        std::map<std::string, std::string> MapValue;

        bool IsList = false;
        std::vector<std::string> ListValue;

        std::function<void(AsyncResult*)> Callback;
        bool HasCallback = false;

        int ActionUniqueId = 0;

    public:
        
        void Interrupt();

        void Fail(const std::string& ErrorMessage, const std::string& ErrorCode = std::string());

        void Fail(const std::string& ErrorMessage, const std::string& ErrorCode, bool Result);
        void Fail(const std::string& ErrorMessage, const std::string& ErrorCode, int Result);
        void Fail(const std::string& ErrorMessage, const std::string& ErrorCode, const std::string& Result);
        void Fail(const std::string& ErrorMessage, const std::string& ErrorCode, const std::map<std::string, std::string>& Result);
        void Fail(const std::string& ErrorMessage, const std::string& ErrorCode, const std::vector<std::string>& Result);

        void Success();
        void Success(bool Result);
        void Success(int Result);
        void Success(const std::string& Result);
        void Success(const std::map<std::string, std::string>& Result);
        void Success(const std::vector<std::string>& Result);

        void SetResult(bool Result);
        void SetResult(int Result);
        void SetResult(const std::string& Result);
        void SetResult(const std::map<std::string, std::string>& Result);
        void SetResult(const std::vector<std::string>& Result);

        int GetActionUniqueId();
        void SetActionUniqueId(int ActionUniqueId);

        bool GetIsFinished();

        bool GetIsSuccess();
        std::string GetErrorMessage();
        std::string GetErrorCode();

        bool GetIsBoolean();
        bool GetBoolean();

        bool GetIsInteger();
        int GetInteger();

        bool GetIsString();
        std::string GetString();

        void SetRawData(const std::string& RawData);
        std::string GetRawData();

        bool GetIsList();
        std::vector<std::string> GetList();

        void Then(std::function<void(AsyncResult*)> Callback);
};

using Async = std::shared_ptr<AsyncResult>;


#endif // ASYNCRESULT_H
