#ifndef SETTINGS_H
#define SETTINGS_H

#include <optional>
#include <string>
#include <tuple>

namespace conf
{
/**
 * Rectangle declared in terms of tuple:
 * std::tuple<x, y, width, height>
 */
using Rectangle = std::tuple<int, int, int, int>;

class Settings
{
    inline static const std::string orgName{"SpaceFoundation"};
    inline static const std::string appName{"SpaceBrowser"};

public:

    /**
     * Returns last saved main window geometry if available
     * @return
     */
    static std::optional<Rectangle> getGeometry();

    /**
     * Save window geometry
     */
    static void setGeometry(Rectangle r);
};

}

#endif
