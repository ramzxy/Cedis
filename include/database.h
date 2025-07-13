#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include <unordered_map>
#include <fstream>

class database
{
public:

    database();

    void set(const std::string& key, const std::string& value);

    std::string get(const std::string& key);

    bool del(const std::string& key);

    bool exists(const std::string& key) const;

    void save_local_store() const;

    void read_local_store();

private:

    std::unordered_map<std::string, std::string> store_;

};
#endif //DATABASE_H
