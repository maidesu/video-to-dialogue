#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>

namespace DialogueFromVideo {

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

public slots:
    void fileChangedHandler(const QStringList& subStreams, const QStringList& audioStreams)
    {

    }

private:
    QComboBox* m_subComboBox;
    QComboBox* m_audioComboBox;
    QGroupBox* m_infoGroupBox;
    QVBoxLayout* m_layout;

signals:

};

} // namespace DialogueFromVideo
