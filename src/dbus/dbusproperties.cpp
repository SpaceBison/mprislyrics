#include <dbus/dbusproperties.hpp>

DBusProperties::DBusProperties(DBus::Connection& connection) :
    DBus::ObjectProxy(connection, "/org/mpris/MediaPlayer2", "org.freedesktop.DBus.Properties") {}

void DBusProperties::setCallback(std::function<void(void)> callback)
{
    this->callback = callback;
}

void DBusProperties::PropertiesChanged(const std::string& interface_name,
                                       const std::vector< std::string >& changed_properties,
                                       const std::vector< std::string >& invalidated_properties)
{
    if(!changed_properties.empty())
    {
        for(std::string s : changed_properties)
        {
            if(s == "Metadata" && callback)
            {
                callback();
                break;
            }
        }
    }
}
