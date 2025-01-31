#pragma once

#include <QWidget>

class BrowseLineEdit;
class QLineEdit;

/// Settings widget
class Settings : public QWidget
{
    Q_OBJECT

  public:
    Settings();

    static QString GetPoe1Client();
    static QString GetPoe2Client();
    static int TradeWidgetWidth();
    static int TradeWidgetX();
    static int TradeWidgetY();
    static QString BusyString();
    static QString CustomMessage1();
    static QString CustomMessage2();
    static QString Thanks();

  signals:
    void SettingsChanged();

  private:
    void SetupUi();
    void SaveSettings();
    static void Browse( QLineEdit *aLineEdit );
    void DisplayExampleWidget();

    BrowseLineEdit *mLEPoe1Client = nullptr, *mLEPoe2Client = nullptr;
    QWidget *mPositionTradeWidget = nullptr;
    QLineEdit *mLEThank = nullptr, *mLEBusy = nullptr;
    QLineEdit *mLECustom1 = nullptr, *mLECustom2 = nullptr;
};
