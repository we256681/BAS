#include "moduledll.h"
#include <QDir>
#include <QJsonObject>
#include <QMap>
#include <QDir>
#include <QJsonDocument>
#include <QByteArray>
#include <QVariant>
#include <QTimezone>
#include "maxminddb.h"
#include "tmap.h"
#include "countrytolanguage.h"

extern "C" {

    struct GeoDllData
    {
        MMDB_s* mmdb;
    };

    void* StartDll()
    {
        GeoDllData * data = new GeoDllData();
        data->mmdb = new MMDB_s();
        int status = MMDB_open("modules/Timezones/GeoLite2-City.mmdb", MMDB_MODE_MMAP, data->mmdb);
        if (MMDB_SUCCESS != status)
        {
            MMDB_close(data->mmdb);
            delete data->mmdb;
            data->mmdb = 0;
        }
        return data;
    }

    void EndDll(void * DllData)
    {
        GeoDllData * data = (GeoDllData *)DllData;
        if(data->mmdb)
        {
            MMDB_close(data->mmdb);
            delete data->mmdb;
            data->mmdb = 0;
        }
        delete data;
    }

    void* StartThread()
    {
        return 0;
    }

    void EndThread(void * DllData)
    {

    }


    void CountryToLanguage(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QString Country = InputJson;

        QByteArray ResArray = CountryToLanguage(Country).toUtf8();

        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size() );

    }
    void TimezoneOffsetToName(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        int OffsetOriginal = QString(InputJson).toInt();
        int OffsetCurrent = OffsetOriginal;
        bool HasResult = false;
        QByteArray Res;
        while(OffsetCurrent<=800)
        {
            QList<QByteArray> ResList = QTimeZone::availableTimeZoneIds(OffsetCurrent*60);
            if(!ResList.isEmpty())
            {
                Res = ResList.first();
                if(!Res.isEmpty() && !Res.startsWith("UTC"))
                {
                    HasResult = true;
                    break;
                }
            }


            OffsetCurrent++;
        }
        if(!HasResult)
        {
            OffsetCurrent = OffsetOriginal;
            while(OffsetCurrent>=-800)
            {
                QList<QByteArray> ResList = QTimeZone::availableTimeZoneIds(OffsetCurrent*60);
                Res = ResList.first();
                if(!ResList.isEmpty())
                {
                    Res = ResList.first();
                    if(!Res.isEmpty() && !Res.startsWith("UTC"))
                    {
                        HasResult = true;
                        break;
                    }
                }

                OffsetCurrent--;
            }

        }


        if(HasResult)
        {
            char * ResMemory = AllocateSpace(Res.size(),AllocateData);
            memcpy(ResMemory, Res.data(), Res.size() );
        }

    }

    void TimezoneNameToOffset(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        QString TimezoneName = QString(InputJson);





        QVariantMap res;

        if(!QTimeZone(TimezoneName.toUtf8()).isValid())
        {
            res.insert("valid", false);
        }else
        {
            res.insert("valid", true);
            QDateTime now = QDateTime::currentDateTime();

            int offset = - QTimeZone(TimezoneName.toUtf8()).standardTimeOffset(now) / 60;
            int dst = 0;
            for(int i = 1;i<=12;i++)
            {
                dst = - QTimeZone(TimezoneName.toUtf8()).daylightTimeOffset(QDateTime(QDate(now.date().year(),i,1),QTime(0,0))) / 60;
                if(dst != 0)
                    break;
            }

            res.insert("offset", offset);
            res.insert("dstoffset", dst + offset);
        }


        QJsonObject object = QJsonObject::fromVariantMap(res);

        QJsonDocument document;
        document.setObject(object);

        QByteArray ResArray = document.toJson();

        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size() );

    }

    void IpInfo(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        GeoDllData * data = (GeoDllData *)DllData;
        QVariantMap res;

        QString ip = InputJson;
        ip = ip.replace("[","").replace("]","");

        if(!data->mmdb)
        {
            res.insert("valid", false);
        }else
        {
            int gai_error, mmdb_error;
            MMDB_lookup_result_s result = MMDB_lookup_string(data->mmdb, ip.toUtf8().constData(), &gai_error, &mmdb_error);

            if (gai_error != 0 || MMDB_SUCCESS != mmdb_error || !result.found_entry)
            {
                res.insert("valid", false);
            }else
            {

                /*MMDB_entry_data_list_s *entry_data_list = NULL;
                int status = MMDB_get_entry_data_list(&result.entry, &entry_data_list);
                FILE * file = fopen("d:/temp/dump.txt","w");
                MMDB_dump_entry_data_list(file, entry_data_list, 2);
                fclose(file);*/



                MMDB_entry_data_s entry_data_city,entry_data_country,entry_data_latitude,entry_data_longitude,entry_data_timezone;
                int status_city =  MMDB_get_value(&result.entry, &entry_data_city, "city", "names", "en", NULL);
                int status_country =  MMDB_get_value(&result.entry, &entry_data_country, "country", "iso_code", NULL);
                int status_latitude =  MMDB_get_value(&result.entry, &entry_data_latitude, "location", "latitude", NULL);
                int status_longitude =  MMDB_get_value(&result.entry, &entry_data_longitude, "location", "longitude", NULL);
                int status_timezone = MMDB_get_value(&result.entry, &entry_data_timezone, "location", "time_zone", NULL);

                if (MMDB_SUCCESS != status_country || !entry_data_country.has_data ||
                    MMDB_SUCCESS != status_latitude || !entry_data_latitude.has_data ||
                    MMDB_SUCCESS != status_longitude || !entry_data_longitude.has_data ||
                    MMDB_SUCCESS != status_timezone || !entry_data_timezone.has_data)
                {
                    res.insert("valid", false);
                }else
                {
                    res.insert("valid", true);

                    if(MMDB_SUCCESS == status_city && entry_data_city.has_data)
                        res.insert("city", QString::fromUtf8(entry_data_city.utf8_string,entry_data_city.data_size));
                    else
                        res.insert("city", "");

                    res.insert("country", QString::fromUtf8(entry_data_country.utf8_string,entry_data_country.data_size));
                    res.insert("latitude", entry_data_latitude.double_value);
                    res.insert("longitude", entry_data_longitude.double_value);
                    QString Timezone = QString::fromUtf8(entry_data_timezone.utf8_string,entry_data_timezone.data_size);
                    res.insert("timezone", Timezone);

                    QDateTime now = QDateTime::currentDateTime();
                    int offset = - QTimeZone(Timezone.toUtf8()).standardTimeOffset(now) / 60;
                    int dst = 0;

                    for(int i = 1;i<=12;i++)
                    {
                        dst = - QTimeZone(Timezone.toUtf8()).daylightTimeOffset(QDateTime(QDate(now.date().year(),i,1),QTime(0,0))) / 60;
                        if(dst != 0)
                            break;
                    }




                    res.insert("offset", offset);
                    res.insert("dstoffset", dst + offset);
                }



            }


        }

        QJsonObject object = QJsonObject::fromVariantMap(res);

        QJsonDocument document;
        document.setObject(object);

        QByteArray ResArray = document.toJson();

        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size() );
    }



}
