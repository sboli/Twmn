#include "settings.h"
#include <iostream>
#include <QKeySequence>

Settings::Settings(QString file) : m_file(file)
{
    m_defaultSettings["main/port"] = 9797;
    m_defaultSettings["main/sound_command"] = "";
    m_defaultSettings["main/activate_command"] = "";
    m_defaultSettings["main/duration"] = "3000";
    m_defaultSettings["main/enable_shortcuts"] = true;
    m_defaultSettings["gui/position"] = "top_right";
    m_defaultSettings["gui/screen"] = "";
    m_defaultSettings["gui/absolute_position"] = "";
    m_defaultSettings["gui/bounce"] = true;
    m_defaultSettings["gui/height"] = 18;
    m_defaultSettings["gui/font"] = "Sans";
    m_defaultSettings["gui/font_size"] = 13;
    m_defaultSettings["gui/foreground_color"] = "#999999";
    m_defaultSettings["gui/background_color"] = "#000000";
    m_defaultSettings["gui/always_on_top"] = true;
    m_defaultSettings["gui/in_animation"] = 38;
    m_defaultSettings["gui/in_animation_duration"] = 1000;
    m_defaultSettings["gui/out_animation"] = 13;
    m_defaultSettings["gui/out_animation_duration"] = 1000;
    m_defaultSettings["icons/critical_icon"] = "";
    m_defaultSettings["icons/warning_icon"] = "";
    m_defaultSettings["icons/info_icon"] = "";
    m_defaultSettings["shortcuts/modifiers"] = QKeySequence(Qt::AltModifier);
    m_defaultSettings["shortcuts/previous"] = QKeySequence("K");
    m_defaultSettings["shortcuts/next"] = QKeySequence("J");
    m_defaultSettings["shortcuts/activate"] = QKeySequence("Return");
    m_defaultSettings["shortcuts/hide"] = QKeySequence("X");
    reload();
}

Settings::~Settings()
{
    save();
}

void Settings::reload()
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "twmn", m_file);
    settings.setIniCodec("UTF-8");
    QStringList keys = settings.allKeys();
    m_data.clear();
    foreach (QString i, keys) {
        m_data[i] = settings.value(i);
    }
    save();
}

void Settings::set(QString setting, const QVariant &value)
{
    m_data[setting] = value;
}

QVariant Settings::get(QString setting)
{
    if (!m_data.contains(setting)) {
        if (!m_defaultSettings.contains(setting)) {
            std::cout << ("Attempt to get a non existing setting.\n"
                          "You might want to create a key for " + setting + " in the config file.").toStdString();
            qApp->quit();
        }
        return m_defaultSettings[setting];
    }
    return m_data[setting];
}

bool Settings::has(QString setting)
{
    return m_data.contains(setting) || m_defaultSettings.contains(setting);
}

void Settings::save()
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "twmn", m_file);
    settings.setIniCodec("UTF-8");
    settings.clear();
    for (QMap<QString, QVariant>::const_iterator it = m_data.begin(); it != m_data.end(); ++it) {
        m_defaultSettings[it.key()] = it.value();
    }
    for (QMap<QString, QVariant>::const_iterator it = m_defaultSettings.begin(); it != m_defaultSettings.end(); ++it) {
        settings.setValue(it.key(), it.value());
    }
}

void Settings::fillWith(const Settings &s)
{
    for (QMap<QString, QVariant>::const_iterator it = s.m_data.begin(); it != s.m_data.end(); ++it) {
        if (m_data.contains(it.key()))
            continue;
        else
            m_data[it.key()] = it.value();
    }
}
