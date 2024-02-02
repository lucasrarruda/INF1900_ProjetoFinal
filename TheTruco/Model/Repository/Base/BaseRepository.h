#pragma once
#include <string>
#include<vector>
#include <Repository/DTOs/BaseDTO.h>
#include <Helpers/DatabaseUtils.h>
#include <Helpers/Utils.h>
#include <Serialize.h>

namespace Repository::Base
{
    template <class T = Repository::DTOs::BaseDTO> class BaseRepository
    {
    public:
        BaseRepository() = default;
        ~BaseRepository() = default;

        virtual std::vector<T> GetAll(const std::string& section);
        virtual T GetById(const std::string& section, const std::string& id);
        virtual T Save(const std::string& section, const std::string& key, T& value);
        virtual T Update(const std::string& section, const std::string& key, T& value);
        virtual void Remove(const std::string& section, const std::string& key);

        virtual T ConvertStringToModel(const std::string& result) = 0;
        virtual std::string ConvertModelToString(const T& result) = 0;
    private:
        std::vector<T> ConvertStringToVectorModel(const std::string& result);
    };

    template<class T>
    std::vector<T> BaseRepository<T>::GetAll(const std::string& section)
    {
        std::wstring result;
        std::wstring wsection = Helpers::Serialize::ConvertStringToWString(static_cast<std::string>(section));

        if (ERROR_NOT_FOUND != Helpers::DatabaseUtils::GetAll(wsection, result))
        {
            return ConvertStringToVectorModel(Helpers::Serialize::ConvertWStringToString(result));
        }

        return std::vector<T>();
    }

    template<class T>
    T BaseRepository<T>::GetById(const std::string& section, const std::string& id)
    {
        auto getModelos = GetAll(section);

        int count = 0;
        for (auto& value : getModelos)
        {
            auto GUID = Helpers::Serialize::ConvertGUIDToString(value.Id);

            if (GUID.compare(id) == 0)
            {
                return value;
            }
        }

        return T();
    }

    template<class T>
    T BaseRepository<T>::Save(const std::string& section, const std::string& key, T& value)
    {
        GUID guidReference;
        CoCreateGuid(&guidReference);

        value.Id = guidReference;

        std::wstring wSection = Helpers::Serialize::ConvertStringToWString(section);

        std::wstring wKey;
        if (key.compare("") == 0)
        {
            wKey = Helpers::Serialize::ConvertStringToWString(Helpers::Serialize::ConvertGUIDToString(value.Id));
        }
        else
        {
            wKey = Helpers::Serialize::ConvertStringToWString(key);
        }

        std::wstring wstringValue = Helpers::Serialize::ConvertStringToWString(ConvertModelToString(value));

        if (ERROR_SUCCESS == Helpers::DatabaseUtils::Set(wSection, wKey, wstringValue))
        {
            return value;
        }

        return T();
    }

    template<class T>
    T BaseRepository<T>::Update(const std::string& section, const std::string& key, T& value)
    {
        std::wstring wsection = Helpers::Serialize::ConvertStringToWString(section);
        std::wstring wkey = Helpers::Serialize::ConvertStringToWString(key);

        std::wstring wstringValue = Helpers::Serialize::ConvertStringToWString(ConvertModelToString(value));

        if (ERROR_SUCCESS == Helpers::DatabaseUtils::Set(wsection, wkey, wstringValue))
        {
            return value;
        }

        return T();
    }

    template<class T>
    void BaseRepository<T>::Remove(const std::string& section, const std::string& key)
    {
        std::wstring wsection = Helpers::Serialize::ConvertStringToWString(section);
        std::wstring wkey = Helpers::Serialize::ConvertStringToWString(key);
       
        Helpers::DatabaseUtils::Remove(wsection, wkey);
    }

    template<class T>
    std::vector<T> BaseRepository<T>::ConvertStringToVectorModel(const std::string& result)
    {
        std::vector<std::string> response = Helpers::Utils::SplitString(result, "|");
        std::vector<std::string> keys;
        std::vector<std::string> values;
        std::vector<std::string> attirbutes;

        for (auto& key : response)
        {
            keys = Helpers::Utils::SplitString(key, "=");
            values.push_back(keys[1]);
        }

        std::vector<T> models;

        int count = 0;
        for (auto& value : values)
        {
            T model = ConvertStringToModel(value);
            models.push_back(model);
        }

        return models;
    }
}