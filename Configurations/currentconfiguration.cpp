#include "currentconfiguration.h"


void CurrentConfiguration::Serialize(ConfigSerializer &ser)
{
    JsonConfiguration::Serialize(ser);

     ser.Serialize("id",      m_id)
        .Serialize("asset",   m_asset)
        .Serialize("site",    m_site);
}

void CurrentConfiguration::Deserialize(ConfigSerializer &desr)
{
    desr.Deserialize("id",      m_id)
        .Deserialize("asset",   m_asset)
        .Deserialize("site",    m_site);
}
