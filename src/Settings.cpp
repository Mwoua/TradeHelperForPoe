#include "Settings.hpp"

#include "BrowseLineEdit.hpp"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QSettings>

constexpr auto SETTINGS_POE1_CLIENT = "POE1_CLIENT";
constexpr auto SETTINGS_POE2_CLIENT = "POE2_CLIENT";

Settings::Settings()
{
    setWindowTitle( "Settings" );
    setAttribute( Qt::WA_DeleteOnClose );
    SetupUi();
}

QString Settings::GetPoe1Client()
{
    QSettings lSettings;
    return lSettings.value( SETTINGS_POE1_CLIENT, "" ).toString();
}

QString Settings::GetPoe2Client()
{
    QSettings lSettings;
    return lSettings.value( SETTINGS_POE2_CLIENT, "" ).toString();
}

void Settings::SetupUi()
{
    auto *lLayout = new QFormLayout( this );

    mLEPoe1Client = new BrowseLineEdit();
    mLEPoe1Client->SetCallback( [this]() { Settings::Browse( mLEPoe1Client ); } );
    mLEPoe1Client->setText( GetPoe1Client() );
    mLEPoe1Client->setPlaceholderText( "Path to client.txt" );
    lLayout->addRow( "Poe1 client.txt", mLEPoe1Client );

    mLEPoe2Client = new BrowseLineEdit();
    mLEPoe2Client->SetCallback( [this]() { Settings::Browse( mLEPoe2Client ); } );
    mLEPoe2Client->setText( GetPoe2Client() );
    mLEPoe2Client->setPlaceholderText( "Path to client.txt" );
    lLayout->addRow( "Poe2 client.txt", mLEPoe2Client );

    auto *lButtons = new QDialogButtonBox( QDialogButtonBox::Apply | QDialogButtonBox::Cancel );
    lLayout->addRow( lButtons );
    connect( lButtons->button( QDialogButtonBox::Apply ), &QPushButton::clicked, this, &Settings::SaveSettings );
    connect( lButtons, &QDialogButtonBox::rejected, this, &QWidget::close );
}

void Settings::SaveSettings()
{
    QSettings lSettings;
    lSettings.setValue( SETTINGS_POE1_CLIENT, mLEPoe1Client->text() );
    lSettings.setValue( SETTINGS_POE2_CLIENT, mLEPoe2Client->text() );
}

void Settings::Browse( QLineEdit *aLineEdit )
{
    const QString lSavePath = QFileDialog::getOpenFileName( nullptr, tr( "Open file" ), "", tr( "Client.txt (*.txt)" ) );
    if( lSavePath.size() > 0 )
    {
        aLineEdit->setText( lSavePath );
    }
}
