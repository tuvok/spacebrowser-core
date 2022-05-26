#include <conf/Settings.h>

#include <log/log.h>

#include <boost/json/src.hpp>
#include <glibmm.h>

#include <fstream>
#include <filesystem>

namespace conf
{
namespace fs = std::filesystem;
namespace js = boost::json;

/// Configuration schema:
///
/// {
///     "MainWindow": {
///         "geometry": {
///             "x": int,
///             "y": int,
///             "w": int,
///             "h": int
///         }
///     },
///     "Databases": [
///         {
///             "id": 12345,
///             "connName": "",
///             "hostname": "",
///             "port": 12345,
///             "dbName": "",
///             "schemaName": "",
///             "username": "",
///             "password": "",
///             "gpgKey": ""
///         },
///           ...
///     ],
/// }
///
///
///

/// global variable for storing config copy in memory:
std::optional<js::object> config = std::nullopt;
/// TODO: if few instances of browser will be started up, then it might be useful to get
///       notifications from system on the file being modified, to synchronize some configs,
///       especially list of database backends


void Settings::initialize()
{
    /// Find configuration file
    fs::path filename{Glib::get_user_config_dir()};
    filename /= orgName;
    filename /= appName;
    filename += ".json";

    if (!fs::exists(filename))
        return;

    /// Load contents
    std::ifstream configFile(filename);
    std::string conf((std::istreambuf_iterator<char>(configFile)),
                     std::istreambuf_iterator<char>());

    /// Parse configuration file as json
    try
    {
        auto value = js::parse(conf);
        if (value.is_object())
            config = value.as_object();
    }
    catch (std::exception& e)
    {
        logging::error(std::string{"failed parsing config: "} + e.what());
        return;
    }
}

void Settings::save()
{
    if (!config)
        return;

    /// Prepare output path and filename
    fs::path filename{Glib::get_user_config_dir()};
    filename /= orgName;
    filename /= appName;
    filename += ".json";

    /// Serialize
    std::string conf = js::serialize(*config);

    /// Write file to permanent storage
    std::ofstream configFile;
    configFile.open(filename);
    if (configFile.fail()) {
        logging::error(std::string{"Failed opening file: "} + strerror(errno));
    }
    configFile << conf << std::endl;
}

std::vector<DbConfig> Settings::getDbConfiguration()
{
    if (!config)
        initialize();

    if (!config)
        return {};

    if (auto dbs = (*config).if_contains("Databases");
        !dbs || !dbs->is_array())
        return {};
    else
    {
        std::vector<DbConfig> results;

        for (auto db: dbs->as_array())
        {
            DbConfig dbc;
            auto dbo = db.as_object();

            if (dbo.contains("id"))
                dbc.id = dbo["id"].as_uint64();
            if (dbo.contains("connName"))
                dbc.connName = dbo["connName"].as_string().c_str();
            if (dbo.contains("hostname"))
                dbc.hostname = dbo["hostname"].as_string().c_str();
            if (dbo.contains("port"))
                dbc.port = dbo["port"].as_uint64();
            if (dbo.contains("dbName"))
                dbc.dbName = dbo["dbName"].as_string().c_str();
            if (dbo.contains("schemaName"))
                dbc.schemaName = dbo["schemaName"].as_string().c_str();
            if (dbo.contains("username"))
                dbc.username = dbo["username"].as_string().c_str();
            if (dbo.contains("password"))
                dbc.password = dbo["password"].as_string().c_str();
            if (dbo.contains("gpgKey"))
                dbc.gpgKey = dbo["gpgKey"].as_string().c_str();

            results.push_back(dbc);
        }

        return results;
    }

}

void Settings::setDbConfiguration(std::vector<DbConfig> dbConfig)
{
    if (!config)
        Settings::initialize();

    if (!config)
        config = js::object();

    js::value& db = (*config)["Databases"];
    if (!db.is_array())
        db = js::array();
    js::array& dbo = db.as_array();
    dbo.clear();

    for (auto& dbc: dbConfig)
    {
        auto jsobj = js::object();

        jsobj["id"] = dbc.id;
        jsobj["connName"] = dbc.connName;
        jsobj["hostname"] = dbc.hostname;
        jsobj["port"] = dbc.port;
        jsobj["dbName"] = dbc.dbName;
        jsobj["schemaName"] = dbc.schemaName;
        jsobj["username"] = dbc.username;
        jsobj["password"] = dbc.password;
        jsobj["gpgKey"] = dbc.gpgKey;

        dbo.push_back(jsobj);
    }

    save();
}

std::optional<Rectangle> Settings::getGeometry()
{
    if (!config)
        initialize();

    if (!config)
        return std::nullopt;

    if (auto mw = (*config).if_contains("MainWindow");
        !mw || !mw->is_object())
        return std::nullopt;
    else if (auto geo = mw->as_object().if_contains("geometry");
        !geo || !geo->is_object())
        return std::nullopt;
    else if (auto gobj = geo->as_object();
             gobj.contains("x") && gobj.contains("y") &&
             gobj.contains("w") && gobj.contains("h"))
    {
        Rectangle r{gobj["x"].as_int64(), gobj["y"].as_int64(),
            gobj["w"].as_int64(), gobj["h"].as_int64()};

        return r;
    }

    return std::nullopt;
}


void Settings::setGeometry(Rectangle r)
{
    if (!config)
        Settings::initialize();

    if (!config)
        config = js::object();

    js::value& mw = (*config)["MainWindow"];
    if (!mw.is_object())
        mw = js::object();
    js::object& mwo = mw.as_object();

    js::value& geo = mwo["geometry"];
    if (!geo.is_object())
        geo = js::object();
    js::object& geoo = geo.as_object();

    geoo["x"] = std::get<0>(r);
    geoo["y"] = std::get<1>(r);
    geoo["w"] = std::get<2>(r);
    geoo["h"] = std::get<3>(r);

    save();
}

} // namespace conf
