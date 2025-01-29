#pragma once

#include <QLabel>

/// A label that can shrink
/// Inspired by https://forum.qt.io/post/168188
class ElideLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY( Qt::TextElideMode elideMode READ elideMode WRITE setElideMode )

  public:
    ElideLabel( QWidget *parent = 0 );
    ElideLabel( const QString &text, QWidget *parent = 0 );
    ~ElideLabel() override = default;

    void setElideMode( Qt::TextElideMode mode );
    Qt::TextElideMode elideMode() const;

    QSize minimumSizeHint() const override;

  protected:
    virtual void paintEvent( QPaintEvent *event );

  private:
    Qt::TextElideMode m_elideMode;
};