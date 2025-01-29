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
    void SetupUi();
    QLayout *SetupUiFirstRow();
    QWidget *SetupUiSecondRow();
    QWidget *BuildTradeItemWidget();
    QWidget *BuildTradePriceWidget();

    void UpdateTime();
    void ManageCollaspeState();

    const PoeVersion mPoeVersion;
    const Trade mTrade;

    QPushButton *mCollapseButton = nullptr;
    QLabel *mLbTime              = nullptr;
    QTimer *mTimer               = nullptr;
    QElapsedTimer mElapsedTimer;
    QWidget *mSecondRow = nullptr;
};