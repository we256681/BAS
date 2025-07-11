#ifndef REPLYTIMEOUT_H
#define REPLYTIMEOUT_H

#include <QObject>
#include <QBasicTimer>
#include <QNetworkReply>

class ReplyTimeout : public QObject
{
    Q_OBJECT
    QBasicTimer m_timer;
public:
    explicit ReplyTimeout(QNetworkReply* reply, const int timeout);
    static void set(QNetworkReply* reply, const int timeout);

protected:
  void timerEvent(QTimerEvent * ev);

signals:

};

#endif // REPLYTIMEOUT_H

