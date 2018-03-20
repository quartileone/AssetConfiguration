
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

#include "configserializer.h"

#include "Configurations/jsonconfiguration.h"


ConfigSerializer &ConfigSerializer::Serialize(const QString &key, IConfiguration* value)
{
    ConfigSerializer ser;
    value->Serialize(ser);

    QJsonValue json_val = QJsonValue(ser.root());
    if(!json_val.isNull() && !json_val.isUndefined()) {
        m_root.insert(key, json_val);
    }

    return *this;
}

ConfigSerializer &ConfigSerializer::Deserialize(const QString &key, QString &value)
{
    QJsonValue jsonVal = m_root.value(key);
    if (!jsonVal.isUndefined()) {
        value = jsonVal.toString();
    }
    return *this;
}

ConfigSerializer &ConfigSerializer::Deserialize(const QString &key, int &value)
{
    QJsonValue jsonVal = m_root.value(key);
    if (!jsonVal.isUndefined()) {
        value = jsonVal.toInt();
    }
    return *this;
}

ConfigSerializer &ConfigSerializer::Deserialize(const QString &key, QJsonObject& value)
{
    QJsonValue jsonVal = m_root.value(key);
    if (!jsonVal.isUndefined()) {
        value = jsonVal.toObject();
    }

    return *this;
}

ConfigSerializer &ConfigSerializer::Deserialize(const QString &key, QJsonValue &value)
{
    if (!m_root.value(key).isUndefined()) {
        value = m_root.value(key);
    }

    return *this;
}

ConfigSerializer &ConfigSerializer::Deserialize(const QString &key, IConfiguration& value)
{
    QJsonObject jsonObj;
    Deserialize(key, jsonObj);

    ConfigSerializer deserializer;
    deserializer.setRoot(jsonObj);

    value.Deserialize(deserializer);

    return *this;
}

ConfigSerializer &ConfigSerializer::Deserialize(const QString &key, IConfigurationList &value)
{
    QJsonArray assetArr = m_root.value(key).toArray();
    foreach (QJsonValue val, assetArr) {
        ConfigSerializer deserializer;
        deserializer.setRoot(val.toObject());
        value.Deserialize(deserializer);
    }

    return *this;
}

void ConfigSerializer::SerializeS(IConfiguration &in, QString &out)
{
    ConfigSerializer serializer;

    in.Serialize(serializer);

    QJsonDocument jsonDoc(serializer.root());
    out = jsonDoc.toJson(QJsonDocument::Indented);
}

void ConfigSerializer::DeserializeS(IConfiguration &out, QString &in)
{
    ConfigSerializer deserializer;

    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(in.toUtf8(), &err);

    if (jsonDoc.isNull())
    {
        qDebug() << err.errorString();
        throw err;
    }

    deserializer.setRoot(jsonDoc.object());

    out.Deserialize(deserializer);
}

void ConfigSerializer::SerializeC(IConfiguration &/*in*/, IConfiguration &/*out*/)
{
    //TODO write immplementation
}

void ConfigSerializer::DeserializeC(IConfiguration &out, IConfiguration &in)
{
    QString strData;
    ConfigSerializer::SerializeS(in, strData);

    ConfigSerializer outDeserializer;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(strData.toUtf8());
    outDeserializer.setRoot(jsonDoc.object());

    out.Deserialize(outDeserializer);

    foreach (QString key, outDeserializer.root().keys()) {
        if (outDeserializer.root().value(key).isObject()) {

            ConfigSerializer deserialiser;
            deserialiser.setRoot(outDeserializer.root().value(key).toObject());

            JsonConfiguration configuration;
            configuration.InsertObject(deserialiser.root());

            DeserializeC(out, configuration);
        }
    }
}

