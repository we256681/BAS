/* This file is part of QtCurl
 *
 * Copyright (C) 2013-2014 Pavel Shmoylov <pavlonion@gmail.com>
 *
 * Thanks to Sergey Romanenko for QStringList options support and for
 * trace function.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software Foundation.
 *
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB. If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef QTCURL_H
#define QTCURL_H

#include <curl/curl.h>
#include <QHash>
#include <QVariant>
#include <QLinkedList>
#include <QFile>



struct BufferAndFilter{std::string *Buffer; const QString* Filter;};
struct TraceAndFilter{QByteArray *Trace; const QString* TraceFilter;};

class QtCUrl {
    QString Filter;
    bool AutoDelete;
    bool DoTrace;
    QString TraceFilter;
    QString FileName;
    QFile File;
    QByteArray Trace;
public:

    typedef QHash<CURLoption, QVariant> OptionItems;
    typedef QList<OptionItems> Options;
    typedef QHashIterator<CURLoption, QVariant> OptionItemsIterator;
	typedef int (*WriterPtr)(char*, size_t, size_t, std::string*);
    typedef int (*WriterFilePtr)(char*, size_t, size_t, QFile*);

    typedef int (*HeaderPtr)(char*, size_t, size_t, BufferAndFilter*);
    typedef int (*ProgressPtr)(void *clientp,   curl_off_t dltotal,   curl_off_t dlnow,   curl_off_t ultotal,   curl_off_t ulnow);


	class Code {
	public:
		Code(CURLcode code = CURLE_OK): _code(code) {}
		QString text() { return curl_easy_strerror(_code); }
		inline CURLcode code() { return _code; }
		inline bool isOk() { return _code == CURLE_OK; }

	private:
		CURLcode _code;
	};

	QtCUrl();
	virtual ~QtCUrl();
    void SetFilter(const QString& Filter);
    void SetTraceFilter(const QString& TraceFilter);


    void MarkAutoDelete();
    void SetDoTrace();


    void exec(Options& options);
	QByteArray buffer() const {
		return QByteArray(_buffer.c_str(), _buffer.length());
	}
	inline Code lastError() { return _lastCode; }
	QString errorBuffer() { return _errorBuffer; }

    QString GetLog();
    QByteArray GetLogByteArray();
    QByteArray GetTrace();
    QByteArray GetDataByteArray();
    void SetFileName(const QString& FileName);


protected:
    void setOptions(OptionItems& opt);

private:
	CURL* _curl;
	char* _errorBuffer;
	std::string _buffer;
    std::string _buffer2;
    BufferAndFilter _buffer_and_filter;
    TraceAndFilter _trace_and_filter;
	Code _lastCode;
	QLinkedList<curl_slist*> _slist;
};

Q_DECLARE_METATYPE(QtCUrl::WriterPtr)
Q_DECLARE_METATYPE(QtCUrl::WriterFilePtr)
Q_DECLARE_METATYPE(QtCUrl::HeaderPtr)
Q_DECLARE_METATYPE(QtCUrl::ProgressPtr)
Q_DECLARE_METATYPE(std::string*)
Q_DECLARE_METATYPE(BufferAndFilter*)
Q_DECLARE_METATYPE(char*)
Q_DECLARE_METATYPE(bool*)
Q_DECLARE_METATYPE(QFile*)

#endif // QTCURL_H
