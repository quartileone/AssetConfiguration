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
        : public IConfiguration, public std::vector<IConfigurationPtr>
{
public:
    virtual ~IConfigurationList()
    {

    }

    template <typename ArgType, typename Functor>
    void ForEach(Functor lambda) {
        std::for_each(cbegin(), cend(), [lambda](const IConfigurationPtr &el)
        {
            lambda(std::dynamic_pointer_cast<ArgType>(el));
        });
    }

    template <typename RetType>
    std::shared_ptr<RetType> Item(int index)
    {
        return std::dynamic_pointer_cast<RetType>(this->operator[](index));
    }
};

