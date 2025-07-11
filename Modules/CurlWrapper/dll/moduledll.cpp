#include "moduledll.h"
#include <QDir>
#include <QJsonObject>
#include <QMap>
#include <QDir>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QByteArray>
#include <QVariant>
#include <QFile>
#include <QJsonArray>
#include <QSharedPointer>
#include <QDateTime>
#include <QTextCodec>
//#include <QDebug>
#include "curl/curl.h"

extern "C" {

    struct CurlDataClass
    {
        CURL *Handler = 0;
        quint64 EndTimeout = 0;
    };

    void* StartDll()
    {
        return 0;
    }

    void EndDll(void * DllData)
    {

    }

    void* StartThread()
    {
        return new CurlDataClass();
    }

    void EndThread(void * DllData)
    {
        CurlDataClass* CurlData = (CurlDataClass*)DllData;

        if(CurlData)
        {
            if(CurlData->EndTimeout > 0 && CurlData->Handler)
            {
                curl_easy_cleanup(CurlData->Handler);
                CurlData->Handler = 0;
            }
            CurlData->EndTimeout = 0;
            delete CurlData;
        }

    }

    void IddleThread(void * ThreadData)
    {
        CurlDataClass* CurlData = (CurlDataClass*)ThreadData;

        if(CurlData)
        {
            quint64 Now = QDateTime::currentMSecsSinceEpoch();
            if(CurlData->EndTimeout > 0 && Now > CurlData->EndTimeout)
            {
                if(CurlData->Handler)
                {
                    curl_easy_cleanup(CurlData->Handler);
                    CurlData->Handler = NULL;
                }

                CurlData->EndTimeout = 0;
            }
        }
    }


    QString StrinfigyReturnCode(int key)
    {
        if(key == 0)return "CURLE_OK";
        if(key == 1)return "CURLE_UNSUPPORTED_PROTOCOL";
        if(key == 2)return "CURLE_FAILED_INIT";
        if(key == 3)return "CURLE_URL_MALFORMAT";
        if(key == 4)return "CURLE_NOT_BUILT_IN";
        if(key == 5)return "CURLE_COULDNT_RESOLVE_PROXY";
        if(key == 6)return "CURLE_COULDNT_RESOLVE_HOST";
        if(key == 7)return "CURLE_COULDNT_CONNECT";
        if(key == 8)return "CURLE_FTP_WEIRD_SERVER_REPLY";
        if(key == 9)return "CURLE_REMOTE_ACCESS_DENIED";
        if(key == 10)return "CURLE_FTP_ACCEPT_FAILED";
        if(key == 11)return "CURLE_FTP_WEIRD_PASS_REPLY";
        if(key == 12)return "CURLE_FTP_ACCEPT_TIMEOUT";
        if(key == 13)return "CURLE_FTP_WEIRD_PASV_REPLY";
        if(key == 14)return "CURLE_FTP_WEIRD_227_FORMAT";
        if(key == 15)return "CURLE_FTP_CANT_GET_HOST";
        if(key == 16)return "CURLE_HTTP2";
        if(key == 17)return "CURLE_FTP_COULDNT_SET_TYPE";
        if(key == 18)return "CURLE_PARTIAL_FILE";
        if(key == 19)return "CURLE_FTP_COULDNT_RETR_FILE";
        if(key == 20)return "CURLE_OBSOLETE20";
        if(key == 21)return "CURLE_QUOTE_ERROR";
        if(key == 22)return "CURLE_HTTP_RETURNED_ERROR";
        if(key == 23)return "CURLE_WRITE_ERROR";
        if(key == 24)return "CURLE_OBSOLETE24";
        if(key == 25)return "CURLE_UPLOAD_FAILED";
        if(key == 26)return "CURLE_READ_ERROR";
        if(key == 27)return "CURLE_OUT_OF_MEMORY";
        if(key == 28)return "CURLE_OPERATION_TIMEDOUT";
        if(key == 29)return "CURLE_OBSOLETE29";
        if(key == 30)return "CURLE_FTP_PORT_FAILED";
        if(key == 31)return "CURLE_FTP_COULDNT_USE_REST";
        if(key == 32)return "CURLE_OBSOLETE32";
        if(key == 33)return "CURLE_RANGE_ERROR";
        if(key == 34)return "CURLE_HTTP_POST_ERROR";
        if(key == 35)return "CURLE_SSL_CONNECT_ERROR";
        if(key == 36)return "CURLE_BAD_DOWNLOAD_RESUME";
        if(key == 37)return "CURLE_FILE_COULDNT_READ_FILE";
        if(key == 38)return "CURLE_LDAP_CANNOT_BIND";
        if(key == 39)return "CURLE_LDAP_SEARCH_FAILED";
        if(key == 40)return "CURLE_OBSOLETE40";
        if(key == 41)return "CURLE_FUNCTION_NOT_FOUND";
        if(key == 42)return "CURLE_ABORTED_BY_CALLBACK";
        if(key == 43)return "CURLE_BAD_FUNCTION_ARGUMENT";
        if(key == 44)return "CURLE_OBSOLETE44";
        if(key == 45)return "CURLE_INTERFACE_FAILED";
        if(key == 46)return "CURLE_OBSOLETE46";
        if(key == 47)return "CURLE_TOO_MANY_REDIRECTS ";
        if(key == 48)return "CURLE_UNKNOWN_OPTION";
        if(key == 49)return "CURLE_TELNET_OPTION_SYNTAX ";
        if(key == 50)return "CURLE_OBSOLETE50";
        if(key == 51)return "CURLE_PEER_FAILED_VERIFICATION";
        if(key == 52)return "CURLE_GOT_NOTHING";
        if(key == 53)return "CURLE_SSL_ENGINE_NOTFOUND";
        if(key == 54)return "CURLE_SSL_ENGINE_SETFAILED";
        if(key == 55)return "CURLE_SEND_ERROR";
        if(key == 56)return "CURLE_RECV_ERROR";
        if(key == 57)return "CURLE_OBSOLETE57";
        if(key == 58)return "CURLE_SSL_CERTPROBLEM";
        if(key == 59)return "CURLE_SSL_CIPHER";
        if(key == 60)return "CURLE_SSL_CACERT";
        if(key == 61)return "CURLE_BAD_CONTENT_ENCODING";
        if(key == 62)return "CURLE_LDAP_INVALID_URL";
        if(key == 63)return "CURLE_FILESIZE_EXCEEDED";
        if(key == 64)return "CURLE_USE_SSL_FAILED";
        if(key == 65)return "CURLE_SEND_FAIL_REWIND";
        if(key == 66)return "CURLE_SSL_ENGINE_INITFAILED";
        if(key == 67)return "CURLE_LOGIN_DENIED";
        if(key == 68)return "CURLE_TFTP_NOTFOUND";
        if(key == 69)return "CURLE_TFTP_PERM";
        if(key == 70)return "CURLE_REMOTE_DISK_FULL";
        if(key == 71)return "CURLE_TFTP_ILLEGAL";
        if(key == 72)return "CURLE_TFTP_UNKNOWNID";
        if(key == 73)return "CURLE_REMOTE_FILE_EXISTS";
        if(key == 74)return "CURLE_TFTP_NOSUCHUSER";
        if(key == 75)return "CURLE_CONV_FAILED";
        if(key == 76)return "CURLE_CONV_REQD";
        if(key == 77)return "CURLE_SSL_CACERT_BADFILE";
        if(key == 78)return "CURLE_REMOTE_FILE_NOT_FOUND";
        if(key == 79)return "CURLE_SSH";
        if(key == 80)return "CURLE_SSL_SHUTDOWN_FAILED";
        if(key == 81)return "CURLE_AGAIN";
        if(key == 82)return "CURLE_SSL_CRL_BADFILE";
        if(key == 83)return "CURLE_SSL_ISSUER_ERROR";
        if(key == 84)return "CURLE_FTP_PRET_FAILED";
        if(key == 85)return "CURLE_RTSP_CSEQ_ERROR";
        if(key == 86)return "CURLE_RTSP_SESSION_ERROR";
        if(key == 87)return "CURLE_FTP_BAD_FILE_LIST";
        if(key == 88)return "CURLE_CHUNK_FAILED";
        if(key == 89)return "CURLE_NO_CONNECTION_AVAILABLE";
        if(key == 90)return "CURLE_SSL_PINNEDPUBKEYNOTMATCH";
        return "UNKNOWN";

    }

    int ParseKey(const QString& key)
    {
        if(key == "CURLOPT_WRITEDATA") return 10000 + 1;
        if(key == "CURLOPT_URL") return 10000 + 2;
        if(key == "CURLOPT_PORT") return 3;
        if(key == "CURLOPT_PROXY") return 10000 + 4;
        if(key == "CURLOPT_USERPWD") return 10000 + 5;
        if(key == "CURLOPT_PROXYUSERPWD") return 10000 + 6;
        if(key == "CURLOPT_RANGE") return 10000 + 7;
        if(key == "CURLOPT_READDATA") return 10000 + 9;
        if(key == "CURLOPT_ERRORBUFFER") return 10000 + 10;
        if(key == "CURLOPT_WRITEFUNCTION") return 20000 + 11;
        if(key == "CURLOPT_READFUNCTION") return 20000 + 12;
        if(key == "CURLOPT_TIMEOUT") return 13;
        if(key == "CURLOPT_INFILESIZE") return 14;
        if(key == "CURLOPT_POSTFIELDS") return 10000 + 15;
        if(key == "CURLOPT_REFERER") return 10000 + 16;
        if(key == "CURLOPT_FTPPORT") return 10000 + 17;
        if(key == "CURLOPT_USERAGENT") return 10000 + 18;
        if(key == "CURLOPT_LOW_SPEED_LIMIT") return 19;
        if(key == "CURLOPT_LOW_SPEED_TIME") return 20;
        if(key == "CURLOPT_RESUME_FROM") return 21;
        if(key == "CURLOPT_COOKIE") return 10000 + 22;
        if(key == "CURLOPT_HTTPHEADER") return 10000 + 23;
        if(key == "CURLOPT_HTTPPOST") return 10000 + 24;
        if(key == "CURLOPT_SSLCERT") return 10000 + 25;
        if(key == "CURLOPT_KEYPASSWD") return 10000 + 26;
        if(key == "CURLOPT_CRLF") return 27;
        if(key == "CURLOPT_QUOTE") return 10000 + 28;
        if(key == "CURLOPT_HEADERDATA") return 10000 + 29;
        if(key == "CURLOPT_COOKIEFILE") return 10000 + 31;
        if(key == "CURLOPT_SSLVERSION") return 32;
        if(key == "CURLOPT_TIMECONDITION") return 33;
        if(key == "CURLOPT_TIMEVALUE") return 34;
        if(key == "CURLOPT_CUSTOMREQUEST") return 10000 + 36;
        if(key == "CURLOPT_STDERR") return 10000 + 37;
        if(key == "CURLOPT_POSTQUOTE") return 10000 + 39;
        if(key == "CURLOPT_OBSOLETE40") return 10000 + 40;
        if(key == "CURLOPT_VERBOSE") return 41;
        if(key == "CURLOPT_HEADER") return 42;
        if(key == "CURLOPT_NOPROGRESS") return 43;
        if(key == "CURLOPT_NOBODY") return 44;
        if(key == "CURLOPT_FAILONERROR") return 45;
        if(key == "CURLOPT_UPLOAD") return 46;
        if(key == "CURLOPT_POST") return 47;
        if(key == "CURLOPT_DIRLISTONLY") return 48;
        if(key == "CURLOPT_APPEND") return 50;
        if(key == "CURLOPT_NETRC") return 51;
        if(key == "CURLOPT_FOLLOWLOCATION") return 52;
        if(key == "CURLOPT_TRANSFERTEXT") return 53;
        if(key == "CURLOPT_PUT") return 54;
        if(key == "CURLOPT_PROGRESSFUNCTION") return 20000 + 56;
        if(key == "CURLOPT_PROGRESSDATA") return 10000 + 57;
        if(key == "CURLOPT_AUTOREFERER") return 58;
        if(key == "CURLOPT_PROXYPORT") return 59;
        if(key == "CURLOPT_POSTFIELDSIZE") return 60;
        if(key == "CURLOPT_HTTPPROXYTUNNEL") return 61;
        if(key == "CURLOPT_INTERFACE") return 10000 + 62;
        if(key == "CURLOPT_KRBLEVEL") return 10000 + 63;
        if(key == "CURLOPT_SSL_VERIFYPEER") return 64;
        if(key == "CURLOPT_CAINFO") return 10000 + 65;
        if(key == "CURLOPT_MAXREDIRS") return 68;
        if(key == "CURLOPT_FILETIME") return 69;
        if(key == "CURLOPT_TELNETOPTIONS") return 10000 + 70;
        if(key == "CURLOPT_MAXCONNECTS") return 71;
        if(key == "CURLOPT_OBSOLETE72") return 72;
        if(key == "CURLOPT_FRESH_CONNECT") return 74;
        if(key == "CURLOPT_FORBID_REUSE") return 75;
        if(key == "CURLOPT_RANDOM_FILE") return 10000 + 76;
        if(key == "CURLOPT_EGDSOCKET") return 10000 + 77;
        if(key == "CURLOPT_CONNECTTIMEOUT") return 78;
        if(key == "CURLOPT_HEADERFUNCTION") return 20000 + 79;
        if(key == "CURLOPT_HTTPGET") return 80;
        if(key == "CURLOPT_SSL_VERIFYHOST") return 81;
        if(key == "CURLOPT_COOKIEJAR") return 10000 + 82;
        if(key == "CURLOPT_SSL_CIPHER_LIST") return 10000 + 83;
        if(key == "CURLOPT_HTTP_VERSION") return 84;
        if(key == "CURLOPT_FTP_USE_EPSV") return 85;
        if(key == "CURLOPT_SSLCERTTYPE") return 10000 + 86;
        if(key == "CURLOPT_SSLKEY") return 10000 + 87;
        if(key == "CURLOPT_SSLKEYTYPE") return 10000 + 88;
        if(key == "CURLOPT_SSLENGINE") return 10000 + 89;
        if(key == "CURLOPT_SSLENGINE_DEFAULT") return 90;
        if(key == "CURLOPT_DNS_USE_GLOBAL_CACHE") return 91;
        if(key == "CURLOPT_DNS_CACHE_TIMEOUT") return 92;
        if(key == "CURLOPT_PREQUOTE") return 10000 + 93;
        if(key == "CURLOPT_DEBUGFUNCTION") return 20000 + 94;
        if(key == "CURLOPT_DEBUGDATA") return 10000 + 95;
        if(key == "CURLOPT_COOKIESESSION") return 96;
        if(key == "CURLOPT_CAPATH") return 10000 + 97;
        if(key == "CURLOPT_BUFFERSIZE") return 98;
        if(key == "CURLOPT_NOSIGNAL") return 99;
        if(key == "CURLOPT_SHARE") return 10000 + 100;
        if(key == "CURLOPT_PROXYTYPE") return 101;
        if(key == "CURLOPT_ACCEPT_ENCODING") return 10000 + 102;
        if(key == "CURLOPT_PRIVATE") return 10000 + 103;
        if(key == "CURLOPT_HTTP200ALIASES") return 10000 + 104;
        if(key == "CURLOPT_UNRESTRICTED_AUTH") return 105;
        if(key == "CURLOPT_FTP_USE_EPRT") return 106;
        if(key == "CURLOPT_HTTPAUTH") return 107;
        if(key == "CURLOPT_SSL_CTX_FUNCTION") return 20000 + 108;
        if(key == "CURLOPT_SSL_CTX_DATA") return 10000 + 109;
        if(key == "CURLOPT_FTP_CREATE_MISSING_DIRS") return 110;
        if(key == "CURLOPT_PROXYAUTH") return 111;
        if(key == "CURLOPT_FTP_RESPONSE_TIMEOUT") return 112;
        if(key == "CURLOPT_IPRESOLVE") return 113;
        if(key == "CURLOPT_MAXFILESIZE") return 114;
        if(key == "CURLOPT_NETRC_FILE") return 10000 + 118;
        if(key == "CURLOPT_USE_SSL") return 119;
        if(key == "CURLOPT_TCP_NODELAY") return 121;
        if(key == "CURLOPT_FTPSSLAUTH") return 129;
        if(key == "CURLOPT_IOCTLFUNCTION") return 20000 + 130;
        if(key == "CURLOPT_IOCTLDATA") return 10000 + 131;
        if(key == "CURLOPT_FTP_ACCOUNT") return 10000 + 134;
        if(key == "CURLOPT_COOKIELIST") return 10000 + 135;
        if(key == "CURLOPT_IGNORE_CONTENT_LENGTH") return 136;
        if(key == "CURLOPT_FTP_SKIP_PASV_IP") return 137;
        if(key == "CURLOPT_FTP_FILEMETHOD") return 138;
        if(key == "CURLOPT_LOCALPORT") return 139;
        if(key == "CURLOPT_LOCALPORTRANGE") return 140;
        if(key == "CURLOPT_CONNECT_ONLY") return 141;
        if(key == "CURLOPT_CONV_FROM_NETWORK_FUNCTION") return 20000 + 142;
        if(key == "CURLOPT_CONV_TO_NETWORK_FUNCTION") return 20000 + 143;
        if(key == "CURLOPT_CONV_FROM_UTF8_FUNCTION") return 20000 + 144;
        if(key == "CURLOPT_FTP_ALTERNATIVE_TO_USER") return 10000 + 147;
        if(key == "CURLOPT_SOCKOPTFUNCTION") return 20000 + 148;
        if(key == "CURLOPT_SOCKOPTDATA") return 10000 + 149;
        if(key == "CURLOPT_SSL_SESSIONID_CACHE") return 150;
        if(key == "CURLOPT_SSH_AUTH_TYPES") return 151;
        if(key == "CURLOPT_SSH_PUBLIC_KEYFILE") return 10000 + 152;
        if(key == "CURLOPT_SSH_PRIVATE_KEYFILE") return 10000 + 153;
        if(key == "CURLOPT_FTP_SSL_CCC") return 154;
        if(key == "CURLOPT_TIMEOUT_MS") return 155;
        if(key == "CURLOPT_CONNECTTIMEOUT_MS") return 156;
        if(key == "CURLOPT_HTTP_TRANSFER_DECODING") return 157;
        if(key == "CURLOPT_HTTP_CONTENT_DECODING") return 158;
        if(key == "CURLOPT_NEW_FILE_PERMS") return 159;
        if(key == "CURLOPT_NEW_DIRECTORY_PERMS") return 160;
        if(key == "CURLOPT_POSTREDIR") return 161;
        if(key == "CURLOPT_SSH_HOST_PUBLIC_KEY_MD5") return 10000 + 162;
        if(key == "CURLOPT_OPENSOCKETFUNCTION") return 20000 + 163;
        if(key == "CURLOPT_OPENSOCKETDATA") return 10000 + 164;
        if(key == "CURLOPT_COPYPOSTFIELDS") return 10000 + 165;
        if(key == "CURLOPT_PROXY_TRANSFER_MODE") return 166;
        if(key == "CURLOPT_SEEKFUNCTION") return 20000 + 167;
        if(key == "CURLOPT_SEEKDATA") return 10000 + 168;
        if(key == "CURLOPT_CRLFILE") return 10000 + 169;
        if(key == "CURLOPT_ISSUERCERT") return 10000 + 170;
        if(key == "CURLOPT_ADDRESS_SCOPE") return 171;
        if(key == "CURLOPT_CERTINFO") return 172;
        if(key == "CURLOPT_USERNAME") return 10000 + 173;
        if(key == "CURLOPT_PASSWORD") return 10000 + 174;
        if(key == "CURLOPT_PROXYUSERNAME") return 10000 + 175;
        if(key == "CURLOPT_PROXYPASSWORD") return 10000 + 176;
        if(key == "CURLOPT_NOPROXY") return 10000 + 177;
        if(key == "CURLOPT_TFTP_BLKSIZE") return 178;
        if(key == "CURLOPT_SOCKS5_GSSAPI_SERVICE") return 10000 + 179;
        if(key == "CURLOPT_SOCKS5_GSSAPI_NEC") return 180;
        if(key == "CURLOPT_PROTOCOLS") return 181;
        if(key == "CURLOPT_REDIR_PROTOCOLS") return 182;
        if(key == "CURLOPT_SSH_KNOWNHOSTS") return 10000 + 183;
        if(key == "CURLOPT_SSH_KEYFUNCTION") return 20000 + 184;
        if(key == "CURLOPT_SSH_KEYDATA") return 10000 + 185;
        if(key == "CURLOPT_MAIL_FROM") return 10000 + 186;
        if(key == "CURLOPT_MAIL_RCPT") return 10000 + 187;
        if(key == "CURLOPT_FTP_USE_PRET") return 188;
        if(key == "CURLOPT_RTSP_REQUEST") return 189;
        if(key == "CURLOPT_RTSP_SESSION_ID") return 10000 + 190;
        if(key == "CURLOPT_RTSP_STREAM_URI") return 10000 + 191;
        if(key == "CURLOPT_RTSP_TRANSPORT") return 10000 + 192;
        if(key == "CURLOPT_RTSP_CLIENT_CSEQ") return 193;
        if(key == "CURLOPT_RTSP_SERVER_CSEQ") return 194;
        if(key == "CURLOPT_INTERLEAVEDATA") return 10000 + 195;
        if(key == "CURLOPT_INTERLEAVEFUNCTION") return 20000 + 196;
        if(key == "CURLOPT_WILDCARDMATCH") return 197;
        if(key == "CURLOPT_CHUNK_BGN_FUNCTION") return 20000 + 198;
        if(key == "CURLOPT_CHUNK_END_FUNCTION") return 20000 + 199;
        if(key == "CURLOPT_FNMATCH_FUNCTION") return 20000 + 200;
        if(key == "CURLOPT_CHUNK_DATA") return 10000 + 201;
        if(key == "CURLOPT_FNMATCH_DATA") return 10000 + 202;
        if(key == "CURLOPT_RESOLVE") return 10000 + 203;
        if(key == "CURLOPT_TLSAUTH_USERNAME") return 10000 + 204;
        if(key == "CURLOPT_TLSAUTH_PASSWORD") return 10000 + 205;
        if(key == "CURLOPT_TLSAUTH_TYPE") return 10000 + 206;
        if(key == "CURLOPT_TRANSFER_ENCODING") return 207;
        if(key == "CURLOPT_CLOSESOCKETFUNCTION") return 20000 + 208;
        if(key == "CURLOPT_CLOSESOCKETDATA") return 10000 + 209;
        if(key == "CURLOPT_GSSAPI_DELEGATION") return 210;
        if(key == "CURLOPT_DNS_SERVERS") return 10000 + 211;
        if(key == "CURLOPT_ACCEPTTIMEOUT_MS") return 212;
        if(key == "CURLOPT_TCP_KEEPALIVE") return 213;
        if(key == "CURLOPT_TCP_KEEPIDLE") return 214;
        if(key == "CURLOPT_TCP_KEEPINTVL") return 215;
        if(key == "CURLOPT_SSL_OPTIONS") return 216;
        if(key == "CURLOPT_MAIL_AUTH") return 10000 + 217;
        if(key == "CURLOPT_SASL_IR") return 218;
        if(key == "CURLOPT_XFERINFOFUNCTION") return 20000 + 219;
        if(key == "CURLOPT_XOAUTH2_BEARER") return 10000 + 220;
        if(key == "CURLOPT_DNS_INTERFACE") return 10000 + 221;
        if(key == "CURLOPT_DNS_LOCAL_IP4") return 10000 + 222;
        if(key == "CURLOPT_DNS_LOCAL_IP6") return 10000 + 223;
        if(key == "CURLOPT_LOGIN_OPTIONS") return 10000 + 224;
        if(key == "CURLOPT_SSL_ENABLE_NPN") return 225;
        if(key == "CURLOPT_SSL_ENABLE_ALPN") return 226;
        if(key == "CURLOPT_EXPECT_100_TIMEOUT_MS") return 227;
        if(key == "CURLOPT_PROXYHEADER") return 10000 + 228;
        if(key == "CURLOPT_HEADEROPT") return 229;
        if(key == "CURLOPT_PINNEDPUBLICKEY") return 10000 + 230;
        return -1;
    }

    struct WriteDataClass
    {
        bool WriteToString = false;
        bool WriteToFile = false;
        bool Base64Write = false;
        QByteArray Data;
        QFile File;
    };

    struct BlobClass
    {
        bool IsFile = false;
        bool IsString = false;
        int BytesReadData = 0;
        QByteArray Data;
        QSharedPointer<QFile> File;
    };

    struct BlobList
    {
        QList<BlobClass> Blobs;
        int ReadIndex = 0;
    };

    struct ReadDataClass
    {
        bool ReadFromString = false;
        bool ReadFromFile = false;
        bool ReadFromBlob = false;
        bool Base64Read = false;
        int BytesReadData = 0;
        QByteArray Data;
        QFile File;
        BlobList Blobs;
    };

    struct TraceDataClass
    {
        bool SaveHeadersInData = false;
        bool SaveTraceData = false;
        QByteArray TraceData;
        QByteArray HeadersInData;
    };

    QStringList GetFetchData(const QString& HeadersIn)
    {
        QStringList Result;

        int IndexFetch1 = HeadersIn.indexOf(QRegularExpression("\\\n\\* \\d+ FETCH", QRegularExpression::MultilineOption));
        int IndexFetch2 = HeadersIn.indexOf(QRegularExpression("^\\* \\d+ FETCH", QRegularExpression::MultilineOption));
        int IndexFetchStart = -1;
        if(IndexFetch1 >= 0 && IndexFetch1 > IndexFetchStart)
        {
            IndexFetchStart = IndexFetch1;
        }
        if(IndexFetch2 >= 0 && IndexFetch2 > IndexFetchStart)
        {
            IndexFetchStart = IndexFetch2;
        }

        if(IndexFetchStart < 0)
        {
            return Result;
        }

        int IndexFetchEnd = HeadersIn.indexOf(QRegularExpression("\\\n[A-Z]\\d+ OK", QRegularExpression::MultilineOption),IndexFetchStart);
        if(IndexFetchEnd < 0)
        {
            return Result;
        }

        int IndexCurrent = IndexFetchStart;


        while(true)
        {
            int IndexCurrentNew = HeadersIn.indexOf(QRegularExpression("\\\n\\* \\d+ FETCH", QRegularExpression::MultilineOption),IndexCurrent + 5);

            if(IndexCurrentNew < 0 || IndexCurrentNew >= IndexFetchEnd)
            {
                //Add last portion and finish
                Result.append(HeadersIn.mid(IndexCurrent,IndexFetchEnd - IndexCurrent + 1));
                break;
            }

            Result.append(HeadersIn.mid(IndexCurrent,IndexCurrentNew - IndexCurrent + 1));

            IndexCurrent = IndexCurrentNew + 1;
        }

        return Result;


    }

    int WriteFunction(char* data, size_t size, size_t nmemb, WriteDataClass* writedata)
    {
        int result = 0;
        if(writedata->WriteToString)
        {
            writedata->Data.append(data,size * nmemb);
        }

        if(writedata->WriteToFile)
        {
            writedata->File.write(data,size * nmemb);
        }

        result = size * nmemb;

        return result;
    }

    int ReadFunction(char* data, size_t size, size_t nmemb, ReadDataClass *readdata)
    {
      //qDebug()<<"ReadFunction"<<size<<nmemb;
      if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1))
      {
        return 0;
      }

      if(readdata->ReadFromString)
      {
          //qDebug()<<"ReadFromString"<<QString::fromUtf8(readdata->Data);
          char *DataPointer = readdata->Data.data() + readdata->BytesReadData;
          int MaxAvailable = readdata->Data.size() - readdata->BytesReadData;
          if(MaxAvailable <= 0)
              return 0;
          int TransferSize = size * nmemb;
          if(TransferSize > MaxAvailable)
              TransferSize = MaxAvailable;
          //qDebug()<<"Transfer"<<QString::fromUtf8(QByteArray(DataPointer,TransferSize));
          memcpy(data, DataPointer, TransferSize);
          readdata->BytesReadData += TransferSize;
          return TransferSize;

      }

      if(readdata->ReadFromFile)
      {
          return readdata->File.read(data, size * nmemb);
      }

      if(readdata->ReadFromBlob)
      {

          int ReadCurrent = 0;
          int ReadAll = size * nmemb;
          //qDebug()<<">>>Ask for read, size:"<<ReadAll;
          char* Data = data;
          while(ReadCurrent < ReadAll)
          {
              if(readdata->Blobs.ReadIndex >= readdata->Blobs.Blobs.length())
              {
                  //qDebug()<<"<<<Blobs finished, read 1"<<ReadCurrent;
                  return ReadCurrent;
              }

              BlobClass &b = readdata->Blobs.Blobs[readdata->Blobs.ReadIndex];
              if(b.IsFile)
              {
                  //qDebug()<<"Read from file"<<b.File->fileName()<<"size"<<ReadAll - ReadCurrent;
                  int Read = b.File->read(Data, ReadAll - ReadCurrent);
                  if(Read<0)
                      Read = 0;
                  /*qDebug()<<"Bytes read from file size"<<Read;
                  {
                      QFile f;
                      f.setFileName("c:/bas/tst.dat");
                      f.open(QIODevice::WriteOnly | QIODevice::Append);
                      f.write(Data,Read);
                      f.close();
                  }
                  qDebug()<<"Bytes read from file data"<<QByteArray(Data,Read).size();*/

                  Data += Read;
                  //qDebug()<<"ReadCurrent <"<<ReadCurrent;
                  ReadCurrent += Read;
                  //qDebug()<<"ReadCurrent >"<<ReadCurrent;

                  if(!Read)
                  {
                      //qDebug()<<"Read from file finished"<<b.File->fileName();

                      b.File.reset();
                      readdata->Blobs.ReadIndex++;
                  }
              }else if(b.IsString)
              {
                  char *DataPointer = b.Data.data() + b.BytesReadData;
                  int MaxAvailable = b.Data.size() - b.BytesReadData;
                  if(MaxAvailable > 0)
                  {
                      int TransferSize = ReadAll - ReadCurrent;
                      if(TransferSize > MaxAvailable)
                          TransferSize = MaxAvailable;

                      //qDebug()<<"Read from string"<<QString::fromUtf8(QByteArray(DataPointer,TransferSize));



                      memcpy(Data, DataPointer, TransferSize);

                      /*{
                          QFile f;
                          f.setFileName("c:/bas/tst.dat");
                          f.open(QIODevice::WriteOnly | QIODevice::Append);
                          f.write(Data,TransferSize);
                          f.close();
                      }*/

                      b.BytesReadData += TransferSize;


                      Data += TransferSize;
                      //qDebug()<<"ReadCurrent <"<<ReadCurrent;
                      ReadCurrent += TransferSize;
                      //qDebug()<<"ReadCurrent >"<<ReadCurrent;
                  }else
                  {
                      readdata->Blobs.ReadIndex++;
                  }
              }else
              {
                  readdata->Blobs.ReadIndex++;
              }

          }
          //qDebug()<<"<<<Blobs finished, read 2"<<ReadCurrent;
          return ReadCurrent;
      }



      return 0;
    }

    int ProgressFunction(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
    {
        if(*((bool *)clientp))
            return 1;
        else
            return 0;
    }


    int TraceFunction(CURL *handle, curl_infotype type, char *data, size_t size, TraceDataClass *TraceData)
    {
        if(type != CURLINFO_SSL_DATA_OUT && type != CURLINFO_SSL_DATA_IN) //Don't save SSL data as it is binary
        {
            if(TraceData->SaveHeadersInData && type == CURLINFO_HEADER_IN)
            {
                TraceData->HeadersInData.append(data, size);
            }
            if(TraceData->SaveTraceData)
            {
                TraceData->TraceData.append(data, size);
            }
        }

        return 1;
    }

    QByteArray GenerateError(const QString& Error)
    {
        QVariantMap res;

        res.insert("success",false);

        res.insert("error",Error);

        QJsonObject object = QJsonObject::fromVariantMap(res);

        QJsonDocument document;
        document.setObject(object);

        return document.toJson();

    }

    void CurlEasyCleanup(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)
    {
        CurlDataClass* CurlData = (CurlDataClass*)ThreadData;

        if(CurlData && CurlData->Handler && CurlData->EndTimeout > 0 /* if timeout is currently active */)
        {
            curl_easy_cleanup(CurlData->Handler);
            CurlData->Handler = 0;
            CurlData->EndTimeout = 0;
        }

    }

    void CurlEasyPerform(char *InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool *NeedToStop, bool* WasError)    {


        CurlDataClass* CurlData = (CurlDataClass*)ThreadData;

        CURLcode code;
        CURL *curl;
        WriteDataClass WriteData;
        ReadDataClass ReadData;
        bool IsTrace = false;
        bool IsFetch = false;
        TraceDataClass TraceData;
        bool SaveSession = false;
        int Timeout = -1;

        QList<curl_slist *> CurlLists;
        {
            //Parse input params

            QJsonDocument InputObject;
            QJsonParseError err;
            InputObject = QJsonDocument::fromJson(QByteArray(InputJson),&err);
            if(err.error)
            {
                QByteArray ResArray = GenerateError("Failed to parse json");

                char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
                memcpy(ResMemory, ResArray.data(), ResArray.size() );
                return;
            }

            if(!InputObject.object().contains("options"))
            {
                QByteArray ResArray = GenerateError("No options param");

                char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
                memcpy(ResMemory, ResArray.data(), ResArray.size() );
                return;
            }

            if(InputObject.object().contains("write_to_string"))
            {
                WriteData.WriteToString = InputObject.object()["write_to_string"].toBool();
            }

            if(InputObject.object().contains("base64_write"))
            {
                WriteData.Base64Write = InputObject.object()["base64_write"].toBool();
            }

            if(InputObject.object().contains("write_to_file"))
            {
                WriteData.WriteToFile = true;
                QString FilePath = InputObject.object()["write_to_file"].toString();
                WriteData.File.setFileName(FilePath);
                if(!WriteData.File.open(QIODevice::WriteOnly))
                {
                    WriteData.WriteToFile = false;
                }

            }

            if(InputObject.object().contains("base64_read"))
            {
                ReadData.Base64Read = InputObject.object()["base64_read"].toBool();
            }

            if(InputObject.object().contains("trace"))
            {
                IsTrace = InputObject.object()["trace"].toBool();
            }

            if(InputObject.object().contains("is_fetch"))
            {
                IsFetch = InputObject.object()["is_fetch"].toBool();
            }

            if(InputObject.object().contains("save_session"))
            {
                SaveSession = InputObject.object()["save_session"].toBool();
            }

            if(InputObject.object().contains("timeout"))
            {
                Timeout = InputObject.object()["timeout"].toInt();
            }

            if(InputObject.object().contains("read_from_string"))
            {
                //qDebug()<<"read_from_string1"<<InputObject.object()["read_from_string"].toString();
                ReadData.ReadFromString = true;
                if(ReadData.Base64Read)
                {
                    QByteArray data = QByteArray::fromBase64(InputObject.object()["read_from_string"].toString().toUtf8());
                    ReadData.Data = data;
                }else
                {
                    ReadData.Data = InputObject.object()["read_from_string"].toString().toUtf8();
                }

                //qDebug()<<"read_from_string2"<<QString::fromUtf8(ReadData.Data);

            }

            if(InputObject.object().contains("read_from_file"))
            {
                ReadData.ReadFromFile = true;
                QString FilePath = InputObject.object()["read_from_file"].toString();
                ReadData.File.setFileName(FilePath);
                if(!ReadData.File.open(QIODevice::ReadOnly))
                {
                    ReadData.ReadFromFile = false;
                }

            }

            if(InputObject.object().contains("read_from_blob"))
            {
                ReadData.ReadFromBlob = true;

                for(QJsonValueRef val:InputObject.object()["read_from_blob"].toArray())
                {
                    QJsonObject obj = val.toObject();
                    if(obj.contains("type"))
                    {
                        QString StringType = obj["type"].toString();
                        if(StringType == "string")
                        {
                            if(obj.contains("data"))
                            {
                                BlobClass b;
                                b.Data = obj["data"].toString().toUtf8();
                                b.IsString = true;
                                ReadData.Blobs.Blobs.append(b);
                            }

                        }
                        if(StringType == "base64")
                        {
                            if(obj.contains("data"))
                            {
                                BlobClass b;
                                b.Data = QByteArray::fromBase64(obj["data"].toString().toUtf8());
                                b.IsString = true;
                                ReadData.Blobs.Blobs.append(b);
                            }

                        }
                        if(StringType == "file")
                        {
                            if(obj.contains("filename"))
                            {
                                BlobClass b;
                                b.File.reset(new QFile());
                                b.File->setFileName(obj["filename"].toString());
                                if(b.File->open(QIODevice::ReadOnly))
                                {
                                    b.IsFile = true;
                                    ReadData.Blobs.Blobs.append(b);
                                }
                            }

                        }
                    }
                }


            }
            //qDebug()<<"++Create";

            bool IsInitialized = false;
            if(Timeout >= 0 && SaveSession)
            {
                if(CurlData && CurlData->Handler)
                {
                    //Use saved session
                    curl_easy_reset(CurlData->Handler);
                    curl = CurlData->Handler;
                    IsInitialized = true;
                }
            }else
            {
                if(CurlData && CurlData->Handler)
                {
                    //Delete previous session, this session can't reuse it
                    curl_easy_cleanup(CurlData->Handler);
                    CurlData->Handler = 0;
                }
            }

            if(CurlData)
            {
                //Don't delete while performing request
                CurlData->EndTimeout = 0;
            }

            if(!IsInitialized)
            {
                curl = curl_easy_init();
            }

            //Init curl


            QJsonObject Options = InputObject.object()["options"].toObject();
            for(QString key:Options.keys())
            {
                int keyint = ParseKey(key);
                if(keyint >= 0)
                {
                    QVariant v = Options[key].toVariant();
                    //qDebug()<<v.type();

                    switch(v.type())
                    {
                        case QVariant::String:
                        {
                            //qDebug()<<"String"<<v.toString();
                            curl_easy_setopt(curl, (CURLoption)keyint, v.toString().toUtf8().data());
                            break;
                        }
                        case QVariant::Int:
                        {
                            //qDebug()<<"Int"<<v;
                            curl_easy_setopt(curl, (CURLoption)keyint, (long)v.toInt());
                            break;
                        }
                        case QVariant::Bool:
                        {
                            //qDebug()<<"Bool"<<v;
                            curl_easy_setopt(curl, (CURLoption)keyint, v.toBool());
                            break;
                        }
                        case QVariant::Double:
                        {
                            //qDebug()<<"Double"<<(long)v.toDouble();
                            curl_easy_setopt(curl, (CURLoption)keyint, (long)v.toDouble());
                            break;
                        }
                        case QVariant::List:
                        {
                            //qDebug()<<"StringList"<<v.toStringList();
                            curl_slist *list = NULL;
                            for(QString str: v.toStringList())
                            {
                                list = curl_slist_append(list, str.toUtf8().data());
                            }
                            CurlLists.append(list);

                            curl_easy_setopt(curl, (CURLoption)keyint, list);
                            break;
                        }
                    }
                }
            }
        }

        //Set write function
        curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,&WriteFunction);
        curl_easy_setopt(curl,CURLOPT_WRITEDATA,&WriteData);

        //Set read function
        if(ReadData.ReadFromFile || ReadData.ReadFromString || ReadData.ReadFromBlob)
        {
            //qDebug()<<"Create read function";
            curl_easy_setopt(curl,CURLOPT_READFUNCTION,&ReadFunction);
            curl_easy_setopt(curl,CURLOPT_READDATA,&ReadData);
            curl_easy_setopt(curl,CURLOPT_UPLOAD, 1L);
        }

        if(IsTrace || IsFetch)
        {
            TraceData.SaveHeadersInData = IsFetch;
            TraceData.SaveTraceData = IsTrace;
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, &TraceFunction);
            curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &TraceData);
        }

        //Set progress function
        curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION,&ProgressFunction);
        curl_easy_setopt(curl,CURLOPT_PROGRESSDATA,NeedToStop);
        curl_easy_setopt(curl,CURLOPT_NOPROGRESS,0L);

        //Set read function

        code = curl_easy_perform(curl);

        if(WriteData.WriteToFile)
        {
            WriteData.File.close();
        }

        //qDebug()<<"--Cleanup";
        if(Timeout >= 0 && SaveSession)
        {
            CurlData->Handler = curl;

            CurlData->EndTimeout = QDateTime::currentMSecsSinceEpoch() + Timeout;

        }else
        {
            curl_easy_cleanup(curl);
        }
        for(curl_slist * list:CurlLists)
        {
            //qDebug()<<"ListRemove";
            curl_slist_free_all(list);
        }

        QVariantMap res;

        res.insert("code",StrinfigyReturnCode((int)code));


        res.insert("success",true);


        if(code == CURLE_OK)
        {
            res.insert("error","");
        }else
        {
            res.insert("error",curl_easy_strerror(code));
        }


        if(WriteData.Base64Write)
        {
            res.insert("result",QString::fromUtf8(WriteData.Data.toBase64()));
        }else
        {
            res.insert("result",QString::fromUtf8(WriteData.Data));
        }

        if(IsTrace)
        {
            res.insert("trace",QString::fromUtf8(TraceData.TraceData));
        }

        if(IsFetch)
        {
            res.insert("fetchlist",GetFetchData(QString::fromUtf8(TraceData.HeadersInData)));
        }

        QJsonObject object = QJsonObject::fromVariantMap(res);

        QJsonDocument document;
        document.setObject(object);

        QByteArray ResArray = document.toJson();

        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size() );
    }


    void Decoder(char *InputJson, ResizeFunction AllocateSpace, void *AllocateData, void *DllData, void *ThreadData, unsigned int ThreadId, bool *NeedToStop, bool *WasError)
    {
        QJsonDocument InputDocument;
        QJsonParseError err;
        InputDocument = QJsonDocument::fromJson(QByteArray(InputJson), &err);
        if(err.error)
        {
            QByteArray ResArray = GenerateError("Failed to parse json");

            char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
            memcpy(ResMemory, ResArray.data(), ResArray.size() );
            return;
        }
        QJsonObject InputObject = InputDocument.object();

        QByteArray Charset = InputObject["charset"].toString().toUtf8();
        QString Encoding = InputObject["encoding"].toString().toLower();
        QByteArray Data = InputObject["data"].toString().toUtf8();
        QByteArray DecodedData;
        QString Result;

        if(Encoding == QString("q"))
        {
            QByteArray src = Data;
            int len = src.length();
            for (int i = 0; i < len; i++)
            {
                if (src[i] == '=')
                {
                    if (i+2 < len)
                    {
                        DecodedData += QByteArray::fromHex(src.mid(i+1,2));
                        i += 2;
                    }
                }
                else
                {
                    DecodedData += src[i];
                }
            }
        }
        else if (Encoding == QString("b"))
        {
            DecodedData = QByteArray::fromBase64(Data);
        }
        else
        {
            DecodedData = Data;
        }
        QTextCodec *codec = QTextCodec::codecForName(Charset);
        if (codec)
        {
            Result = codec->toUnicode(DecodedData);
        }else
        {
            Result = QString::fromUtf8(DecodedData);
        }


        QVariantMap res;

        res.insert("success", true);

        res.insert("result", Result);

        QJsonObject object = QJsonObject::fromVariantMap(res);

        QJsonDocument document;
        document.setObject(object);

        QByteArray ResArray = document.toJson();

        char * ResMemory = AllocateSpace(ResArray.size(),AllocateData);
        memcpy(ResMemory, ResArray.data(), ResArray.size() );
    }



}
