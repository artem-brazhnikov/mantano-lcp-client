#ifndef __LCP_UTILS_H__
#define __LCP_UTILS_H__

#include <string>
#include <map>
#include "Public/LcpStatus.h"

namespace lcp
{
    class StatusException : public std::runtime_error
    {
    public:
        StatusException(const Status & status)
            : std::runtime_error(status.Extension)
            , m_status(status)
        {
        }

        Status ResultStatus() const
        {
            return m_status;
        }

    private:
        Status m_status;
    };

    std::string BoolToString(bool val);
    std::string DoubleToExponentString(const double & value);

    void ValidateUtf8(const std::string & utf8Str);
    bool LexicographicalCompareUtf8(const std::string & left, const std::string & right);

    typedef std::map<std::string, std::string> StringsMap;
}

#endif //__LCP_UTILS_H__