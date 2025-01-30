#pragma once

#include <QLineEdit>
#include <QPushButton>

#include <functional>

/// @brief A QLineEdit with a "..." button to browse for file
class BrowseLineEdit : public QLineEdit
{
    Q_OBJECT

  public:
    explicit BrowseLineEdit( std::function<void()> aBrowseCallback, QWidget *aParent = nullptr );
    explicit BrowseLineEdit( QWidget *aParent = nullptr );
    void SetCallback( std::function<void()> aBrowseCallback );

  private:
    std::function<void()> mBrowseCallback;
    QPushButton *mPBBrowse = nullptr;
};

