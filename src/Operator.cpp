#include "Operator.h"

NS_MARATON_BEGIN

Operator::Operator( std::string address , int port )
{
    this->address_ = address;
    this->port_    = port;
}

Operator::~Operator( )
{
}

std::string Operator::ip( )
{
    return this->ip_;
}

std::string Operator::address( )
{
    return this->address_;
}

int Operator::port( )
{
    return this->port_;
}

NS_MARATON_END
