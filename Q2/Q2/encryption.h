#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <QElapsedTimer>

qint64 measureAESTime(const std::string &message, bool isEncryption);
qint64 measureRSATime(const std::string &message, bool isEncryption);

#endif // ENCRYPTION_H
