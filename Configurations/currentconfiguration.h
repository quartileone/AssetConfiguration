#ifndef CURRENTCONFIGURATION_H
#define CURRENTCONFIGURATION_H

#include "Configurations/jsonconfiguration.h"

class CurrentConfiguration
        : public JsonConfiguration
{
public:
    CurrentConfiguration()
        : m_id(0)
        , m_site("")
        , m_asset("")
    {
    }

    int id() const { return m_id; }
    void setId(const int id) { m_id = id; }

    QString site() const { return m_site; }
    void setSite(const QString site) { m_site = site; }

    QString asset() const { return m_asset; }
    void setAsset(const QString asset) { m_asset = asset; }

// IConfiguration implementation
private:
    void Serialize(ConfigSerializer& ser) override;
    void Deserialize(ConfigSerializer& desr) override;

private:
    int m_id;
    QString m_site;
    QString m_asset;
};

#endif // CURRENTCONFIGURATION_H
