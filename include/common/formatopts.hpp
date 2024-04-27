#pragma once

#include <QString>
#include <QStringList>
#include <QMap>
#include <QList>

namespace DialogueFromVideo :: FormatOptions {

struct Option
{
    int id;
    QString key, value;
};

QMap<QString, QList<Option>> lossyOptions();

QMap<QString, QList<Option>> losslessOptions();

} // namespace DialogueFromVideo
