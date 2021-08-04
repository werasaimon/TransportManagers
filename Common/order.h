#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QDataStream>

class Order
{
public:
    Order();
    Order(const Order& _order);

     // Перегрузка оператора присваивания
     Order& operator= (const Order &_order);


//     QDataStream& operator<<(QDataStream& lhs, const Order& rhs)
//     {
//         lhs << rhs.ID;
//         lhs << rhs.Text;
//         lhs << rhs.Data;
//         lhs << rhs.Username;
//         lhs << rhs.Weight;
//         lhs << rhs.Address;
//     }
     //----------------//

     int ID;
     QString Text;
     QString Data;
     QString Username;
     int Weight;
     QString Address;
     QString Product;
     QString Status;

};



class Orderfulfillment : public Order
{
public:
    Orderfulfillment();
};


#endif // ORDER_H
