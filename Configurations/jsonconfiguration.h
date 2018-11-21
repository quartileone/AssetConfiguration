#ifndef JSONCONFIGURATION_H
#define JSONCONFIGURATION_H

#include <QMap>
#include "DataModel/iconfiguration.h"

class JsonConfiguration
        : public IConfiguration
{
public:
    JsonConfiguration()
    {
    }

    template <class V>
    void InsertValue(const QString& key, const V& value)
    {
        m_configValueMap.insert(key, QJsonValue(value));
    }

    void InsertObject(const QJsonObject& obj);

    void InsertConfiguration(const QString& key, IConfigurationPtr value);

    virtual void TakeValue(const QString& key, QString& value);
    virtual void TakeValue(const QString& key, int& value);
    virtual void TakeValue(const QString& key, IConfiguration* value);

// IConfiguration implementation
public:
    void Serialize(ConfigSerializer& ser) override;
    void Deserialize(ConfigSerializer& desr) override;

protected:
    QMap<QString, QJsonValue> m_configValueMap;
};

typedef std::shared_ptr<JsonConfiguration> JsonConfigurationPtr;

#endif // JSONCONFIGURATION_H
