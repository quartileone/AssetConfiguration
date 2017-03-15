#ifndef CONFIGSERIALIZER_H
#define CONFIGSERIALIZER_H

#include <QJsonObject>
#include <QString>
#include <QDebug>

#include "iconfiguration.h"
class IConfiguration;
class IConfigurationList;

class ConfigSerializer
{
public:
    ConfigSerializer()
    {
    }

    template <class V>
    ConfigSerializer& Serialize(const QString& key, V& value)
    {
        QJsonValue jsonValue(value);
        if( !jsonValue.isUndefined()) {
            m_root.insert(key, jsonValue);
        }

        return *this;
    }

    ConfigSerializer& Serialize(const QString& key, IConfiguration* value);

    ConfigSerializer& Deserialize(const QString& key, QString& value);
    ConfigSerializer& Deserialize(const QString& key, int& value);
    ConfigSerializer& Deserialize(const QString& key, QJsonObject& value);
    ConfigSerializer& Deserialize(const QString& key, QJsonValue& value);
    ConfigSerializer& Deserialize(const QString& key, IConfiguration& value);
    ConfigSerializer& Deserialize(const QString& key, IConfigurationList& value);

    static void SerializeS(IConfiguration& in, QString& out);
    static void DeserializeS(IConfiguration& out, QString& in);

    static void SerializeC(IConfiguration& in, IConfiguration& out);
    static void DeserializeC(IConfiguration& out, IConfiguration& in);

    QJsonObject root() const { return m_root; }
    void setRoot(const QJsonObject &root) { m_root = root; }

private:
    QJsonObject m_root;
};

#endif // CONFIGSERIALIZER_H
