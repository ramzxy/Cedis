#include "database.h"

#include <iostream>
#include <stdexcept>

database::database() {
    std::cout << "database constructor" << std::endl;
    read_local_store();
}

void database::set(const std::string& key, const std::string& value)
{
    store_[key] = value;
}

std::string database::get(const std::string& key)
{
    if (store_.find(key) != store_.end())
    {
        return store_[key];
    }
    throw std::runtime_error("Key not found");
}

bool database::del(const std::string& key)
{
   return store_.erase(key);
}

bool database::exists(const std::string& key) const
{
    return store_.count(key);
}

void database::save_local_store() const {
    std::ofstream ofs("dump.rdb");

    for (const auto& [key, value] : store_) {
        ofs << key << '\t' << value << '\n';
    }
}

void database::read_local_store() {
    std::ifstream ifs("dump.rdb");
    std::string key, value;

    while ( ifs >> key >> value ) {
        store_[key] = value;
        std::cout << key << '\t' << value << '\n';
    }
}
