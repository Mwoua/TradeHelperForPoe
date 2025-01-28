#pragma once

#include "Defines.hpp"

#include <QWidget>

class TradeWidget : public QWidget
{
    Q_OBJECT;

  public:
    TradeWidget( Trade aTrade, PoeVersion aVersion );
};