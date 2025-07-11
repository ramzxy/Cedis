#include "database.h"

#include <stdexcept>

void database::set(const std::string& key, const std::string& value)
{
    db_[key] = value;
}

std::string database::get(const std::string& key)
{
    if (db_.find(key) != db_.end())
    {
        return db_[key];
    }
    throw std::runtime_error("Key not found");
}

bool database::del(const std::string& key)
{
   return db_.erase(key);
}

bool database::exists(const std::string& key) const
{
    return db_.count(key);
}
