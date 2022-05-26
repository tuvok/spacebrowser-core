#ifndef DB_CONFIG_H
#define DB_CONFIG_H

#include <string>

namespace conf
{

/**
 * Structure describes configuration necessary to access database backend
 */
struct DbConfig
{
    uint16_t id;             /// Necessary to keep displaying backends in the same order
    std::string connName;    /// Name of this connection (just label naming db config on particular user machine)
    std::string hostname;    /// Database hostname
    uint16_t port{0};        /// Port on which database is accessible
    std::string dbName;      /// Database name
    std::string schemaName;  /// Schema name
    std::string username;    /// User name
    std::string password;    /// Password
    std::string gpgKey;      /// gpgKey used to encrypt password (empty if password not encrypted)
};

}

#endif // DB_CONFIG_H
