#include "Settings.hpp"

#include "Defines.hpp"
#include "TradeWidget.hpp"
#include "Utils/BrowseLineEdit.hpp"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QSettings>
#include <QSizeGrip>

constexpr auto SETTINGS_POE1_CLIENT         = "POE1_CLIENT";
constexpr auto SETTINGS_POE2_CLIENT         = "POE2_CLIENT";
constexpr auto SETTINGS_TRADE_WIDTH         = "TRADE_WIDTH";
constexpr auto SETTINGS_TRADE_POS_X         = "TRADE_POS_X";
constexpr auto SETTINGS_TRADE_POS_Y         = "TRADE_POS_Y";
constexpr auto SETTINGS_TRADE_TOP_TO_BOTTOM = "TRADE_TOP_TO_BOT";
constexpr auto SETTINGS_BUSY_STRING         = "BUSY";
constexpr auto SETTINGS_CUSTOM1             = "CUSTOM1";
constexpr auto SETTINGS_CUSTOM2             = "CUSTOM2";
constexpr auto SETTINGS_THANKS              = "THANKS";

constexpr auto DEFAULT_BUSY   = "Hello, I'm currently busy. Can you wait a few minutes? Thanks";
constexpr auto DEFAULT_THANKS = "Thank you for trading. Have a good day exile";

class ResizeWidget : public QWidget
{
  public:
    explicit ResizeWidget( QWidget &aWidgetToResizeAndPosition )
    {
        aWidgetToResizeAndPosition.setFixedWidth( QWIDGETSIZE_MAX ); // remove constraints
        auto *lLayout = new QVBoxLayout( this );
        lLayout->setContentsMargins( 0, 0, 0, 0 );
        lLayout->setSpacing( 0 );
        lLayout->addWidget( &aWidgetToResizeAndPosition );
        lLayout->addWidget( new QSizeGrip( this ), 0, Qt::AlignBottom | Qt::AlignRight );
        aWidgetToResizeAndPosition.setEnabled( false );
        setFixedHeight( sizeHint().height() );
        setWindowFlags( Qt::WindowStaysOnTopHint | Qt::Window | Qt::FramelessWindowHint );
    }

    void mousePressEvent( QMouseEvent *e ) override { mOrigin = e->globalPos(); }
    void mouseMoveEvent( QMouseEvent *e ) override
    {
        if( ( mOrigin != QPoint( 0, 0 ) ) && ( e->buttons() & Qt::LeftButton ) ) // dragging
        {
            move( pos() + ( e->globalPos() - mOrigin ) );
            mOrigin = e->globalPos();
        }
    }
    void mouseReleaseEvent( QMouseEvent *event ) override { mOrigin = QPoint( 0, 0 ); }

  private:
    QPoint mOrigin{};
};

Settings::Settings()
{
    setWindowTitle( "Settings" );
    setAttribute( Qt::WA_DeleteOnClose );
    SetupUi();
    DisplayExampleWidget();
}

QString Settings::GetPoe1Client()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_POE1_CLIENT, "" ).toString();
}

QString Settings::GetPoe2Client()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_POE2_CLIENT, "" ).toString();
}

int Settings::TradeWidgetWidth()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_TRADE_WIDTH, "" ).toInt();
}

int Settings::TradeWidgetX()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_TRADE_POS_X, "" ).toInt();
}

int Settings::TradeWidgetY()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_TRADE_POS_Y, "" ).toInt();
}

QString Settings::BusyString()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_BUSY_STRING, DEFAULT_BUSY ).toString();
}

QString Settings::CustomMessage1()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_CUSTOM1, "" ).toString();
}

QString Settings::CustomMessage2()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_CUSTOM2, "" ).toString();
}

QString Settings::Thanks()
{
    const QSettings lSettings;
    return lSettings.value( SETTINGS_THANKS, DEFAULT_THANKS ).toString();
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

    auto *lButtons = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
    lLayout->addRow( lButtons );
    connect( lButtons->button( QDialogButtonBox::Ok ), &QPushButton::clicked, this, &Settings::SaveSettings );
    connect( lButtons, &QDialogButtonBox::rejected, this, &QWidget::close );
}

void Settings::SaveSettings()
{
    QSettings lSettings;
    lSettings.setValue( SETTINGS_POE1_CLIENT, mLEPoe1Client->text() );
    lSettings.setValue( SETTINGS_POE2_CLIENT, mLEPoe2Client->text() );
    lSettings.setValue( SETTINGS_TRADE_WIDTH, mPositionTradeWidget->layout()->itemAt( 0 )->widget()->width() );
    lSettings.setValue( SETTINGS_TRADE_POS_X, mPositionTradeWidget->x() );
    lSettings.setValue( SETTINGS_TRADE_POS_Y, mPositionTradeWidget->y() );

    // constexpr auto SETTINGS_TRADE_TOP_TO_BOTTOM = "TRADE_TOP_TO_BOT";
    // constexpr auto SETTINGS_BUSY_STRING         = "BUSY";
    // constexpr auto SETTINGS_CUSTOM1             = "CUSTOM1";
    // constexpr auto SETTINGS_CUSTOM2             = "CUSTOM2";
    // constexpr auto SETTINGS_THANKS              = "THANKS";
    
    close();
}

void Settings::Browse( QLineEdit *aLineEdit )
{
    const QString lSavePath = QFileDialog::getOpenFileName( nullptr, tr( "Open file" ), "", tr( "Client.txt (*.txt)" ) );
    if( lSavePath.size() > 0 )
    {
        aLineEdit->setText( lSavePath );
    }
}

void Settings::DisplayExampleWidget()
{
    Trade lSampleTrade{ .mIncoming     = true,
                        .mItem         = "Awesome object on sale",
                        .mLeague       = "LeagueName",
                        .mUser         = "UserName",
                        .mPrice        = "Price",
                        .mStash        = "Stash",
                        .mPositionLeft = "0",
                        .mPositionTop  = "0",
                        .mComment      = "Comment" };

    auto *lSampleTradeWidget = new TradeWidget( std::move( lSampleTrade ), PoeVersion::Poe1 );
    mPositionTradeWidget     = new ResizeWidget( *lSampleTradeWidget );
    mPositionTradeWidget->move( TradeWidgetX(), TradeWidgetY() );

    mPositionTradeWidget->show();
    connect( this, &QWidget::destroyed, mPositionTradeWidget, [this]() { mPositionTradeWidget->deleteLater(); } );
}
