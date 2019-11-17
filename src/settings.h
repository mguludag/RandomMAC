/*This class can r/w settings, set themes*/
/************created by m231**************/
/**************m231@tnctr.com*************/
#ifndef SETTINGS_H
#define SETTINGS_H
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include <QSettings>
#include <QDesktopWidget>
#include <QToolTip>

class Settings
{
public:
    enum Theme {
        lightFusion = 0,
        darkFusion = 1,
        vista = 2,
        classic = 3,

    };

    static int loadTheme()
    {
        int theme;
        QSettings s("settings.ini",QSettings::IniFormat);  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
        s.beginGroup("Style");
        theme = s.value("Theme", lightFusion).toInt(); //default theme is standard system theme
        s.endGroup();
        return theme;
    }

    static void setTheme(int theme)
    {
        QSettings s("settings.ini",QSettings::IniFormat);  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
        s.beginGroup("Style");
        s.setValue("Theme",theme);
        s.endGroup();

        switch (theme) {
        case lightFusion: {
            qApp->setStyle(QStyleFactory::create("Fusion"));
            qApp->setPalette(qApp->style()->standardPalette());
            break;
        }
        case darkFusion: {
            qApp->setStyle(QStyleFactory::create("Fusion"));
            QPalette darkPalette;
            darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
            darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::ToolTipBase, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::HighlightedText, Qt::black);
            darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(164, 166, 168));
            darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(164, 166, 168));
            darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(164, 166, 168));
            darkPalette.setColor(QPalette::Disabled,
                                 QPalette::HighlightedText,
                                 QColor(164, 166, 168));
            darkPalette.setColor(QPalette::Disabled, QPalette::Base, QColor(68, 68, 68));
            darkPalette.setColor(QPalette::Disabled, QPalette::Window, QColor(68, 68, 68));
            darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(68, 68, 68));
            QToolTip::setPalette(darkPalette);
            qApp->setPalette(darkPalette);
            break;
        }
        case vista:
        {
            qApp->setStyle(QStyleFactory::create("windowsvista"));
            qApp->setPalette(qApp->style()->standardPalette());
            break;
        }
        case classic:
        {
            qApp->setStyle(QStyleFactory::create("windows"));
            qApp->setPalette(qApp->style()->standardPalette());
            break;
        }

        default:
            break;
        }
    }

    template <class T>
    static void writeSettings(QString group, QString key, T option)
    {
        QSettings s("settings.ini",QSettings::IniFormat);  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
        s.beginGroup(group);
        s.setValue(key, option);
        s.endGroup();
    }

    static QVariant readSettings(QString group, QString key)
    {
        QVariant val;
        QSettings s("settings.ini",QSettings::IniFormat);  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
        s.beginGroup(group);
        val=s.value(key);
        s.endGroup();
        return val;
    }
};

#endif // SETTINGS_H
