#pragma once

#include <QMessageBox>

namespace DialogueFromVideo {

class RestartMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    explicit RestartMessageBox(QWidget* parent = nullptr);

    void restartApplication();
};

} // namespace DialogueFromVideo
