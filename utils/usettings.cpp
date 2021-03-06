#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "../utils/usettings.h"
#include "../utils/uexception.h"
//------------------------------------------------------------------------------
using namespace utils;
//------------------------------------------------------------------------------
USettings::USettings(QObject* parent)
    : QObject(parent)
{
}
//------------------------------------------------------------------------------
void USettings::Load(const QString& fname, const QStringList& required)
{
    QFile file(fname);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw UException("Error opening file " + fname);
    }

    QTextStream in(&file);
    QString line = "";
    while (!line.isNull()) {
        line = in.readLine().trimmed();
        if (line.isEmpty() || line[0] == '#') {
            continue;
        }
        QStringList acc = line.split("=");
        if (acc.size() != 2) {
            continue;
        }
        Parameters.insert(acc[0].trimmed(), acc[1].trimmed());
    }

    for (auto i = required.begin(); i != required.end(); i++) {
        if (Parameters.find(*i) == Parameters.end()) {
            throw UException("Required parameter " + *i + " not found");
        }
    }
}
//------------------------------------------------------------------------------
UFromStringFormat USettings::GetParameter(const QString& parameter)
{
    if (Parameters.find(parameter) == Parameters.end()) {
        throw UException(QString("Parameter '%1'not found in config")
                .arg(parameter));
    }
    return FromString(Parameters[parameter]);
}
//------------------------------------------------------------------------------
void USettings::SetParameter(const QString& parameter, const QString& value)
{
    if (parameter.isEmpty()) {
        return;
    }

    if (Parameters.keys().contains(parameter)) {
        Parameters.remove(parameter);
    }

    Parameters[parameter] = value;
}
//------------------------------------------------------------------------------
void USettings::DefineParams(const TParametersHash& paramsList)
{
    Parameters = paramsList;
}
//------------------------------------------------------------------------------
void USettings::Save(const QString& fname, bool keepOrigin)
{
    QFile file(fname);
    if (keepOrigin && file.exists()) {
        return;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate
                   | QIODevice::Text)) {
        throw UException("Error opening file for writing " + fname);
    }

    QTextStream stream(&file);
    QStringList vars(Parameters.uniqueKeys());
    vars.sort();
    auto it = vars.constBegin();
    for (; it != vars.constEnd(); ++it) {
        stream << *it << "=" << Parameters[*it] << "\n";
    }

    file.flush();
}
//------------------------------------------------------------------------------
