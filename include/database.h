#ifndef DATABASE_H
#define DATABASE_H
#include <string>
#include <unordered_map>

class database
{
public:
    void set(const std::string& key, const std::string& value);

    std::string get(const std::string& key);

    bool del(const std::string& key);

    bool exists(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> db_;
};
#endif //DATABASE_H
