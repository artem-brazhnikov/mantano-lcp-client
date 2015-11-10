#include "rapidjson/document.h"
#include "JsonValueReader.h"
#include "LcpUtils.h"

namespace lcp
{
    std::string JsonValueReader::ReadString(const std::string & name, const rapidjson::Value & jsonValue)
    {
        auto it = jsonValue.FindMember(name.c_str());
        if (it != jsonValue.MemberEnd() && it->value.IsString())
        {
            return it->value.GetString();
        }
        return std::string();
    }

    std::string JsonValueReader::ReadStringCheck(const std::string & name, const rapidjson::Value & jsonValue)
    {
        auto it = jsonValue.FindMember(name.c_str());
        if (it == jsonValue.MemberEnd() || !it->value.IsString())
        {
            throw StatusException(Status(StCodeCover::ErrorOpeningLicenseNotValid, name + " object is not valid"));
        }

        std::string result(it->value.GetString());
        if (result.empty())
        {
            throw StatusException(Status(StCodeCover::ErrorOpeningLicenseNotValid, name + " object is not valid"));
        }
        return result;
    }

    const rapidjson::Value & JsonValueReader::ReadObject(const std::string & name, const rapidjson::Value & jsonValue)
    {
        auto it = jsonValue.FindMember(name.c_str());
        if (it != jsonValue.MemberEnd() && it->value.IsObject())
        {
            return it->value;
        }
        return rapidjson::Value(rapidjson::kNullType).Move();
    }

    const rapidjson::Value & JsonValueReader::ReadObjectCheck(const std::string & name, const rapidjson::Value & jsonValue)
    {
        auto it = jsonValue.FindMember(name.c_str());
        if (it == jsonValue.MemberEnd() || !it->value.IsObject())
        {
            throw StatusException(Status(StCodeCover::ErrorOpeningLicenseNotValid, name + " object is not valid"));
        }
        return it->value;
    }

    std::string JsonValueReader::ConvertToString(const rapidjson::Value & value)
    {
        switch (value.GetType())
        {
        case rapidjson::kFalseType:
            return BoolToString(value.GetBool());
        case rapidjson::kTrueType:
            return BoolToString(value.GetBool());
        case rapidjson::kStringType:
            return value.GetString();
        case rapidjson::kNumberType:
            if (value.IsUint64())
                return std::to_string(value.GetUint64());
            else if (value.IsInt64())
                return std::to_string(value.GetInt64());
            else if (value.IsUint())
                return std::to_string(value.GetUint());
            else if (value.IsInt())
                return std::to_string(value.GetInt());
            else if (value.IsDouble())
                return std::to_string(value.GetDouble());
            else
                return std::string();
        case rapidjson::kNullType:
        case rapidjson::kObjectType:
        case rapidjson::kArrayType:
        default:
            return std::string();
        }
    }

    /*static*/ Status JsonValueReader::CreateRapidJsonError(rapidjson::ParseErrorCode errorCode, size_t offset /* = INVALID_OFFSET */)
    {
        std::stringstream strm;
        strm << "RapidJson error code: " << static_cast<int>(errorCode);
        if (offset != INVALID_OFFSET)
        {
            strm << ". Error offset: " << offset;
        }

        return Status(
            StCodeCover::ErrorOpeningLicenseNotValid,
            strm.str().c_str()
            );
    }
}