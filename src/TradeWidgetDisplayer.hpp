#pragma once

#include <QVBoxLayout>
#include <QWidget>

class TradeWidgetDisplayer : public QWidget
{
    Q_OBJECT;

  public:
    explicit TradeWidgetDisplayer();

    void AddTradeWidget( QWidget &aWidget );

  private:
    QVBoxLayout *mMainLayout = nullptr;
};
