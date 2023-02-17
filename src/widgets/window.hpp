#pragma once

#include <QWidget>
#include <QVBoxLayout>

namespace DialogueFromVideo {

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

private:
    QVBoxLayout* m_layout;

signals:

};

} // namespace DialogueFromVideo
