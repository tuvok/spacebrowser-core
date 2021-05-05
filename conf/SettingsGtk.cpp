#include "Settings.h"

#include <glibmm.h>

#include <boost/json/src.hpp>
#include <fstream>
#include <filesystem>

#include <iostream>

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
///     }
/// }
///
///
///
std::optional<js::object> config = std::nullopt;

void Settings::initialize()
{
    fs::path filename{Glib::get_user_config_dir()};
    filename /= orgName;
    filename /= appName;
    filename += ".json";

    if (!fs::exists(filename))
        return;

    std::ifstream configFile(filename);
    std::string conf((std::istreambuf_iterator<char>(configFile)),
                     std::istreambuf_iterator<char>());

    try
    {
        auto value = js::parse(conf);
        if (value.is_object())
            config = value.as_object();
    }
    catch (std::exception& e)
    {// FIXME: add proper logging library
        std::cout << "failed parsing config: " << e.what() << std::endl;
        return;
    }
}

void Settings::save()
{
    if (!config)
        return;

    fs::path filename{Glib::get_user_config_dir()};
    filename /= orgName;
    filename /= appName;
    filename += ".json";

    std::string conf = js::serialize(*config);

    std::ofstream configFile;
    configFile.open(filename);
    if (configFile.fail()) {
        std::cout << "errno: " << strerror(errno) << std::endl;
    }
    configFile << conf << std::endl;
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
