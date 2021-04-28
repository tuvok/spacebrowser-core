#include "Settings.h"

#include "conf.h"

#include <QRect>
#include <QSettings>

namespace conf
{

std::optional<Rectangle> Settings::getGeometry()
{
    QSettings settings(orgName.c_str(), appName.c_str());

    if (settings.contains(conf::MainWindow::geometry))
    {
        QRect rect = settings.value(conf::MainWindow::geometry).toRect();

        return {{rect.x(), rect.y(), rect.width(), rect.height()}};
    }

    return std::nullopt;
}

void Settings::setGeometry(Rectangle r)
{
    QSettings settings(orgName.c_str(), appName.c_str());
    QRect qr{std::get<0>(r), std::get<1>(r), std::get<2>(r), std::get<3>(r)};

    settings.setValue(conf::MainWindow::geometry, qr);
}
} // namespace conf
