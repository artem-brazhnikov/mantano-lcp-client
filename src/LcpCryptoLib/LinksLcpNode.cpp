#include <algorithm>
#include "rapidjson/document.h"
#include "LinksLcpNode.h"
#include "JsonValueReader.h"
#include "LcpUtils.h"

namespace lcp
{
    void LinksLcpNode::ParseNode(const rapidjson::Value & parentObject, JsonValueReader * reader)
    {
        const rapidjson::Value & linksObject = reader->ReadObjectCheck("links", parentObject);
        
        if (!linksObject.HasMember(HINT))
        {
            throw StatusException(Status(StCodeCover::ErrorOpeningLicenseNotValid, "links object is not valid"));
        }

        for (auto it = linksObject.MemberBegin(); it != linksObject.MemberEnd(); ++it)
        {
            rapidjson::Type type = it->value.GetType();

            if (type == rapidjson::kObjectType)
            {
                Link link = this->ParseLinkValues(it->value, reader);
                m_linksMultiMap.insert(std::make_pair(it->name.GetString(), link));
            }
            else if (type == rapidjson::kArrayType)
            {
                for (auto arrayIt = it->value.Begin(); arrayIt != it->value.End(); ++arrayIt)
                {
                    Link link = this->ParseLinkValues(*arrayIt, reader);
                    m_linksMultiMap.insert(std::make_pair(it->name.GetString(), link));
                }
            }
            else
            {
                throw StatusException(Status(StCodeCover::ErrorOpeningLicenseNotValid, "links object is not valid"));
            }
        }

        BaseLcpNode::ParseNode(linksObject, reader);
    }

    std::vector<Link> LinksLcpNode::GetLinks(const std::string & name) const
    {
        std::vector<Link> result;
        std::pair<LinksMapConstIt, LinksMapConstIt> range = m_linksMultiMap.equal_range(name);
        std::transform(
            range.first, range.second,
            std::back_inserter(result),
            [&result](const LinksMap::value_type & val) { return val.second; }
        );
        return result;
    }

    Link LinksLcpNode::ParseLinkValues(const rapidjson::Value & linkObject, JsonValueReader * reader)
    {
        Link link;
        link.href = reader->ReadStringCheck("href", linkObject);
        link.title = reader->ReadString("title", linkObject);
        link.type = reader->ReadString("type", linkObject);
        link.templated = reader->ReadString("templated", linkObject);
        link.length = reader->ReadString("length", linkObject);
        link.hash = reader->ReadString("hash", linkObject);
        return link;
    }
}