#include "BrowseLineEdit.hpp"

#include <QWidgetAction>

BrowseLineEdit::BrowseLineEdit( std::function<void()> aBrowseCallback, QWidget *aParent )
    : BrowseLineEdit( aParent )
{
    SetCallback( std::move( aBrowseCallback ) );
}

BrowseLineEdit::BrowseLineEdit( QWidget *aParent )
    : QLineEdit( aParent )
    , mPBBrowse( new QPushButton( "..." ) )
{
    auto *lWABrowse = new QWidgetAction( this );
    lWABrowse->setDefaultWidget( mPBBrowse );
    this->addAction( lWABrowse, QLineEdit::TrailingPosition );
}

void BrowseLineEdit::SetCallback( std::function<void()> aBrowseCallback )
{
    disconnect( mPBBrowse );
    mBrowseCallback = std::move( aBrowseCallback );
    connect( mPBBrowse, &QPushButton::clicked, this, [this]() { mBrowseCallback(); } );
}
