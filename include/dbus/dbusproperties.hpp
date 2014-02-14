#ifndef DBUSPROPERTIESPROXY_H
#define DBUSPROPERTIESPROXY_H

#include <functional>
#include <dbus-c++/dbus.h>
#include <dbus-c++/property.h>
#include <dbus/propertiesproxy.hpp>

class DBusProperties :
    public org::freedesktop::DBus::Properties_proxy,
    public DBus::ObjectProxy
{
    public:
        DBusProperties(DBus::Connection& connection);
        void setCallback(std::function<void(void)> callback);
        
        void PropertiesChanged(const std::string& interface_name,
                               const std::vector< std::string >& changed_properties,
                               const std::vector< std::string >& invalidated_properties);
    private:
        std::function<void(void)> callback;
};

#endif /* DBUSPROPERTIESPROXY_H */ 
