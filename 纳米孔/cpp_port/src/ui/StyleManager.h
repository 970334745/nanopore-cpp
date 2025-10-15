#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QString>
#include <QApplication>

class StyleManager
{
public:
    enum Theme {
        LightTheme,
        DarkTheme,
        BlueTheme,
        HighContrastTheme
    };

    static void applyTheme(Theme theme);
    static void loadStyleSheet(const QString& filePath);
    static QString getCurrentTheme();
    static void setCustomColors(const QString& primaryColor, const QString& secondaryColor);

private:
    static QString generateLightThemeCSS();
    static QString generateDarkThemeCSS();
    static QString generateBlueThemeCSS();
    static QString generateHighContrastThemeCSS();
    static QString currentTheme_;
};

#endif // STYLEMANAGER_H