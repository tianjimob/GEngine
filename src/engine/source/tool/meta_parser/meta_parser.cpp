#include "meta_parser.h"

MetaParser::MetaParser(const std::filesystem::path &metaHead,
                       const std::filesystem::path &includePath)
    : m_metaHead(metaHead), m_includePath(includePath)
{
    
}
