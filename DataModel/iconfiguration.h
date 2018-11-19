#pragma once

#include <vector>
#include <memory>

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

typedef std::shared_ptr<IConfiguration> IConfigurationPtr;

class IConfigurationList
        : public IConfiguration
{
public:
    virtual ~IConfigurationList()
    {

    }

    void Add(const IConfigurationPtr &item)
    {
        m_list.push_back(item);
    }

    int Size() const { return m_list.size(); }

    void CLear() { m_list.clear(); }

    template <typename ArgType, typename Functor>
    void ForEach(Functor lambda) {
        std::for_each(m_list.cbegin(), m_list.cend(),
                      [lambda](const IConfigurationPtr &el)
        {
            lambda(std::dynamic_pointer_cast<ArgType>(el));
        });
    }

    template <typename RetType>
    std::shared_ptr<RetType> Item(int index)
    {
        return std::dynamic_pointer_cast<RetType>(m_list[index]);
    }


protected:
    std::vector<IConfigurationPtr> m_list;
};

