/// @file	src/Settings.h
///
/// @brief	Declares the settings class

#pragma once

#include <QWidget>

class BrowseLineEdit;
class QLineEdit;

class Settings : public QWidget
{
    Q_OBJECT;

  public:
    Settings();
    static QString GetPoe1Client();
    static QString GetPoe2Client();

  private:
    void SetupUi();
    void SaveSettings();
    static void Browse( QLineEdit *aLineEdit );

    BrowseLineEdit *mLEPoe1Client = nullptr, *mLEPoe2Client = nullptr;
};
