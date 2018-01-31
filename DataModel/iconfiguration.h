#ifndef ICONFIGURATION_H
#define ICONFIGURATION_H

#include <QVector>
#include <QSharedPointer>

#include "configserializer.h"

class ConfigSerializer;

class IConfiguration
{
public:
    virtual ~IConfiguration()
    {

    }

    virtual void Serialize(ConfigSerializer& ser) = 0;

    virtual void Deserialize(ConfigSerializer& desr) = 0;

};

class IConfigurationList
        : public IConfiguration
{
public:
    virtual ~IConfigurationList()
    {

    }

    void Add(IConfiguration* item)
    {
        m_list.push_back(QSharedPointer<IConfiguration>(item));
    }

    int Size() const { return m_list.size(); }

    void CLear() { m_list.clear(); }

    virtual IConfiguration* Item(int index)
    {
        return m_list[index].data();
    }


protected:
    QVector<QSharedPointer<IConfiguration>> m_list;
};

#endif // ICONFIGURATION_H
