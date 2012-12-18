#pragma once
#include <exception>
#include <QString>
#include <QByteArray>
//------------------------------------------------------------------------------
namespace utils
{
//------------------------------------------------------------------------------
class UException : public std::exception
{
public:
    UException(const QString& message) throw();
    const char* what() const throw();
    ~UException() throw();
private:
    QByteArray Message;
};
//------------------------------------------------------------------------------
}
//------------------------------------------------------------------------------
// TODO: remove this typedef
typedef utils::UException UException;
//------------------------------------------------------------------------------
