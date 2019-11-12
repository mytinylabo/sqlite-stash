#include <iostream>

#include <SQLiteCpp/SQLiteCpp.h>

class stash
{
public:
    stash();
    stash(std::string db_path);

    template <typename T>
    T get(std::string key);

    template <typename T>
    void set(std::string key, T value);

    ~stash();

private:
    const std::string table_int{"stash_int"};
    const std::string table_double{"stash_double"};
    const std::string table_string{"stash_string"};
    const std::string table_bool{"stash_bool"};

    std::unique_ptr<SQLite::Database> db;

    template <typename T>
    std::string stash_table();
};

stash::stash()
{
    db.reset(new SQLite::Database{"../db/test.db3", SQLite::OPEN_READWRITE});
}

stash::~stash()
{
}

template <>
std::string stash::stash_table<int>() { return table_int; }

template <>
std::string stash::stash_table<double>() { return table_double; }

template <>
std::string stash::stash_table<std::string>() { return table_string; }

template <>
std::string stash::stash_table<bool>() { return table_bool; }

template <typename T>
T stash::get(std::string key)
{
    SQLite::Statement query{*db, "SELECT value FROM " + stash_table<T>() + " WHERE key = @key"};

    query.bind("@key", key);

    query.executeStep();
    return query.getColumn(0);
}

template <>
bool stash::get(std::string key)
{
    SQLite::Statement query{*db, "SELECT value FROM " + stash_table<bool>() + " WHERE key = @key"};

    query.bind("@key", key);

    query.executeStep();
    auto value = query.getColumn(0).getString();

    return value == "true";
}

template <typename T>
void stash::set(std::string key, T value)
{
    SQLite::Statement query{*db, "INSERT OR REPLACE INTO " + stash_table<T>() + " values (@key, @value)"};

    query.bind("@key", key);
    query.bind("@value", value);

    query.executeStep();
}

template <>
void stash::set(std::string key, bool value)
{
    SQLite::Statement query{*db, "INSERT OR REPLACE INTO " + stash_table<bool>() + " values (@key, @value)"};

    query.bind("@key", key);
    query.bind("@value", value ? "true" : "false");

    query.executeStep();
}

int main()
{
    stash st;

    std::cout << "v: " << st.get<bool>("hogeflag") << std::endl;
    std::cout << "v: " << st.get<std::string>("hoge") << std::endl;

    st.set("hogeflag", true);
    st.set("fugaflag", false);

    std::cout << "done" << std::endl;
}
