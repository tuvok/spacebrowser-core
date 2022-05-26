#ifndef SETTINGS_H
#define SETTINGS_H

#include <conf/DbConfig.h>

#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace conf
{
/**
 * Rectangle declared in terms of tuple:
 * std::tuple<x, y, width, height>
 */
using Rectangle = std::tuple<int, int, int, int>;

/**
 * Class for accessing application settings stored locally on users machine
 */
class Settings
{
    inline static const std::string orgName{"SpaceFoundation"};
    inline static const std::string appName{"SpaceBrowser"};

    static void initialize(); /// Initialize configuration backend
    static void save();       /// Save configuration to permanent storage
public:


    /**
     * Returns database backends configuration
     */
    static std::vector<DbConfig> getDbConfiguration();

    /**
     * Save database backends configuration
     */
    static void setDbConfiguration(std::vector<DbConfig> dbConfig);

    /**
     * Returns last saved main window geometry if available
     */
    static std::optional<Rectangle> getGeometry();

    /**
     * Save window geometry
     */
    static void setGeometry(Rectangle r);
};

}

#endif
