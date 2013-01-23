#include "protocol.h"

Protocol::Protocol() {
    _tagList.clear();
}

Protocol::~Protocol() {

}

QByteArray Protocol::decode(QByteArray data) {
    return data;
}

QByteArray Protocol::encode(QByteArray data) {
    return data;
}

QStringList Protocol::getTagList() {
    return _tagList;
}
