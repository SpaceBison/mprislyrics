#include <cstring>
#include <dbus/dbus.h>
#include <mpris/mpriscontroller.hpp>

#include <iostream>
#include <cstdio>

MprisController::MprisController()
{
    DBusError err;
    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);

    if (dbus_error_is_set(&err))
    {
        std::cout << "Connection Error " << err.message << std::endl;
        dbus_error_free(&err);
    }

    int ret = dbus_bus_request_name(conn, "test.method.server",
         DBUS_NAME_FLAG_REPLACE_EXISTING
         , &err);

    if (dbus_error_is_set(&err))
    {
        std::cout << "Connection Error " << err.message << std::endl;
        dbus_error_free(&err);
    }

    if (DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret)
    {
        std::cout << "not the DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER" << std::endl;
    }

    dbus_bus_add_match(conn,
         "type='signal',interface='org.freedesktop.MediaPlayer',member='TrackChange'", // MPRIS1
         &err);

    dbus_bus_add_match(conn,
         "type='signal',path='/org/mpris/MediaPlayer2'", // MPRIS2
         &err);

    dbus_connection_flush(conn);
    while(dbus_connection_pop_message(conn) != NULL);

    if (dbus_error_is_set(&err)) {
        std::cout << "Match error " << err.message << std::endl;
    }
}

void MprisController::waitForTrackChange()
{
    DBusMessage* msg;
    bool got_data = false;

    dbus_connection_read_write(conn, -1);

    /* make sure we got a relevant message */
    while(!got_data)
    {
        dbus_connection_read_write(conn, -1);
        msg = dbus_connection_pop_message(conn);
        got_data = handleMessage(msg);
        dbus_message_unref(msg);
    }

    /* make sure it's the last one and flush
     * the incoming buffer */
    dbus_connection_read_write(conn, MSG_DELAY_MS);
    msg = dbus_connection_pop_message(conn);
    while(msg != NULL)
    {
        handleMessage(msg);
        dbus_message_unref(msg);
        dbus_connection_read_write(conn, MSG_DELAY_MS);
        msg = dbus_connection_pop_message(conn);
    }

}

bool MprisController::handleMessage(DBusMessage* msg)
{
    DBusMessageIter rootIter;
    if (dbus_message_iter_init(msg, &rootIter))
    {
        if (dbus_message_iter_get_arg_type(&rootIter) == DBUS_TYPE_STRING)
        {
            return handleMpris2MessageIter(rootIter);
        }
        else
        {
            return handleMprisMessageIter(rootIter);
        }
    }
    else
        return false;
}

/* abandon all hope, ye who scroll down here,
 * for low-level DBus message parsing
 * is an atrocity of unspeakable magnitude */

bool MprisController::handleMpris2MessageIter(DBusMessageIter& rootIter)
{
    char* str;
    dbus_message_iter_get_basic(&rootIter, &str);
    if(!strcmp(str, "org.mpris.MediaPlayer2.Player"))
    {
        dbus_message_iter_next(&rootIter);
        if (dbus_message_iter_get_arg_type(&rootIter) == DBUS_TYPE_ARRAY)
        {
            DBusMessageIter arrayIter;
            dbus_message_iter_recurse(&rootIter, &arrayIter);
            if(dbus_message_iter_get_arg_type(&arrayIter) == DBUS_TYPE_DICT_ENTRY)
            {
                DBusMessageIter dictIter;
                dbus_message_iter_recurse(&arrayIter, &dictIter);
                if(dbus_message_iter_get_arg_type(&dictIter) == DBUS_TYPE_STRING)
                {
                    char* key;
                    dbus_message_iter_get_basic(&dictIter, &key);
                    if(!strcmp(key, "Metadata"))
                    {
                        dbus_message_iter_next(&dictIter);
                        if(dbus_message_iter_get_arg_type(&dictIter) == DBUS_TYPE_VARIANT)
                        {
                            DBusMessageIter varIter;
                            dbus_message_iter_recurse(&dictIter, &varIter);
                            return handleMprisMessageIter(varIter);
                        }
                        else return false;
                    }
                    else return false;
                }
                else return false;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}

bool MprisController::handleMprisMessageIter(DBusMessageIter& rootIter)
{
    if (dbus_message_iter_get_arg_type(&rootIter) == DBUS_TYPE_ARRAY)
    {
        DBusMessageIter arrayIter;
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter) == DBUS_TYPE_DICT_ENTRY)
        {
            DBusMessageIter dictIter;
            dbus_message_iter_recurse(&arrayIter, &dictIter);
            if(dbus_message_iter_get_arg_type(&dictIter) == DBUS_TYPE_STRING)
            {
                char* key;
                std::string* str_ptr = NULL;
                int* int_ptr = 0;
                dbus_message_iter_get_basic(&dictIter, &key);

                if(!strcmp(key, "artist") || !strcmp(key, "xesam:artist"))
                    str_ptr = &trackinfo.artist;
                else if(!strcmp(key, "album") || !strcmp(key, "xesam:album"))
                    str_ptr = &trackinfo.album;
                else if(!strcmp(key, "title") || !strcmp(key, "xesam:title"))
                    str_ptr = &trackinfo.title;
                else if(!strcmp(key, "tracknumber") || !strcmp(key, "xesam:tracknumber"))
                    int_ptr = &trackinfo.track;
                else
                {
                    dbus_message_iter_next(&arrayIter);
                    continue;
                }

                dbus_message_iter_next(&dictIter);
                if(dbus_message_iter_get_arg_type(&dictIter) == DBUS_TYPE_VARIANT)
                {
                    DBusMessageIter varIter;
                    dbus_message_iter_recurse(&dictIter, &varIter);

                    if(dbus_message_iter_get_arg_type(&varIter) == DBUS_TYPE_STRING)
                    {
                        char* str;
                        dbus_message_iter_get_basic(&varIter, &str);
                        if(str_ptr == NULL) // VLC passes tracknumber as a string; go figure
                            sscanf(str, "%d", int_ptr);
                        else
                            *str_ptr = str;
                    }
                    else if(dbus_message_iter_get_arg_type(&varIter) == DBUS_TYPE_ARRAY)
                    {
                        DBusMessageIter stringArrayIter;
                        dbus_message_iter_recurse(&varIter, &stringArrayIter);
                        if(dbus_message_iter_get_arg_type(&stringArrayIter) == DBUS_TYPE_STRING)
                        {
                            char* str;
                            dbus_message_iter_get_basic(&stringArrayIter, &str);
                            *str_ptr = str;
                        }
                        else return false;
                    }
                    else if(dbus_message_iter_get_arg_type(&varIter) == DBUS_TYPE_INT32)
                    {
                        dbus_message_iter_get_basic(&varIter, int_ptr);
                    }
                    else return false;
                }
                else return false;
            }
            else return false;
            dbus_message_iter_next(&arrayIter);
        } // while
    }
    else return false;
    return true;
}

TrackInfo MprisController::getTrackInfo()
{
    return trackinfo;
}

MprisController::~MprisController()
{
    std::cout << "close connection" << std::endl;
    dbus_connection_unref(conn);
}
