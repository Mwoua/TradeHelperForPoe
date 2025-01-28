#pragma once

#include "Defines.hpp"

#include <QElapsedTimer>
#include <QWidget>

class QPushButton;
class QLabel;
class QTimer;
class QHBoxLayout;

class TradeWidget : public QWidget
{
    Q_OBJECT;

  public:
    TradeWidget( Trade aTrade, PoeVersion aVersion );

  private:
    void SetupUi( const Trade &aTrade );
    QLayout *SetupUiFirstRow( const Trade &aTrade );
    QWidget *SetupUiSecondRow( const Trade &aTrade );
    QWidget *BuildTradeItemWidget( const Trade &aTrade );
    QWidget *BuildTradePriceWidget( const Trade &aTrade );

    void UpdateTime();
    void ManageCollaspeState();

    const PoeVersion mPoeVersion;

    QPushButton *mCollapseButton = nullptr;
    QLabel *mLbTime              = nullptr;
    QTimer *mTimer               = nullptr;
    QElapsedTimer mElapsedTimer;
    QWidget *mSecondRow = nullptr;
};