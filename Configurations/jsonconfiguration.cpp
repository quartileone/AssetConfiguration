#include "jsonconfiguration.h"


void JsonConfiguration::InsertObject(const QJsonObject &obj)
{
    foreach (QString key, obj.keys()) {
        m_configValueMap.insert(key, obj.value(key));
    }
}

void JsonConfiguration::InsertConfiguration(const QString &key, IConfigurationPtr value)
{
    ConfigSerializer ser;
    value->Serialize(ser);

    if (key.isEmpty()) {
        QJsonObject jsonObj = ser.root();
        foreach (QString val, jsonObj.keys()) {
            m_configValueMap.insert(val, jsonObj.take(val));
        }
    }
    else {
        m_configValueMap.insert(key, QJsonValue(ser.root()));
    }
}

void JsonConfiguration::TakeValue(const QString &key, QString &value)
{
    QJsonValue jsonValue = m_configValueMap.take(key);
    value = jsonValue.toString();
}

void JsonConfiguration::TakeValue(const QString &key, int &value)
{
    QJsonValue jsonValue = m_configValueMap.take(key);
    value = jsonValue.toInt();
}

void JsonConfiguration::TakeValue(const QString &key, IConfiguration *value)
{
    ConfigSerializer deser;
    QJsonValue jsonValue = m_configValueMap.take(key);
    deser.setRoot(jsonValue.toObject());

    value->Deserialize(deser);
}


void JsonConfiguration::Serialize(ConfigSerializer &ser)
{
    foreach (QJsonValue it, m_configValueMap) {
        ser.Serialize(m_configValueMap.key(it), it);
    }
}

void JsonConfiguration::Deserialize(ConfigSerializer &/*desr*/)
{

}
