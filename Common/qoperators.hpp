#ifndef QOPERATORS_H
#define QOPERATORS_H

#include <QMap>
#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include "../Common/order.h"


QDataStream &operator<<( QDataStream &lhs, const Order &rhs)
{
    lhs << rhs.ID;
    lhs << rhs.Text;
    lhs << rhs.Data;
    lhs << rhs.Username;
    lhs << rhs.Weight;
    lhs << rhs.Address;
    lhs << rhs.Product;
    lhs << rhs.Status;
    lhs << rhs.Manager;
    return lhs;
}

QDataStream &operator>>(QDataStream &lhs, Order &rhs)
{
    lhs >> rhs.ID;
    lhs >> rhs.Text;
    lhs >> rhs.Data;
    lhs >> rhs.Username;
    lhs >> rhs.Weight;
    lhs >> rhs.Address;
    lhs >> rhs.Product;
    lhs >> rhs.Status;
    lhs >> rhs.Manager;
    return lhs;
}

#endif // QOPERATORS_H
