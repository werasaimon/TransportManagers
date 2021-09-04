#include "order.h"
#include <QDataStream>

Order::Order()
{

}

Order::Order(const Order &_order)
{
    this->ID = _order.ID;
    this->Text = _order.Text;
    this->Data = _order.Data;
    this->Username = _order.Username;
    this->Weight = _order.Weight;
    this->Address = _order.Address;
    this->Product = _order.Product;
    this->Status = _order.Status;
    this->Manager = _order.Manager;

}

Order &Order::operator=(const Order &_order)
{
    this->ID = _order.ID;
    this->Text = _order.Text;
    this->Data = _order.Data;
    this->Username = _order.Username;
    this->Weight = _order.Weight;
    this->Address = _order.Address;
    this->Product = _order.Product;
    this->Status = _order.Status;
    this->Manager = _order.Manager;
}



//----------------------------------//

Orderfulfillment::Orderfulfillment()
{

}

