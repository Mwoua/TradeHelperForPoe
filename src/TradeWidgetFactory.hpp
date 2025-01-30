#pragma once

#include "Defines.hpp"

#include <QObject>

#include <vector>

class QWidget;
class TradeWidgetDisplayer;

class TradeWidgetFactory : public QObject
{
    Q_OBJECT

  public:
    static void Create( TradeWidgetDisplayer &aTradeWidgetDisplayer, Trade aTrade, PoeVersion aVersion );
};
