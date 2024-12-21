#include "utils.h"
#include <QRandomGenerator>

std::string generateRandomString(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string result;
    for (size_t i = 0; i < length; ++i) {
        result += charset[QRandomGenerator::global()->bounded(static_cast<int>(sizeof(charset) - 1))];
    }
    return result;
}

