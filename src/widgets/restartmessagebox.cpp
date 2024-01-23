#include <widgets/restartmessagebox.hpp>

#include <common/exitcode.hpp>

#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

namespace DialogueFromVideo {

RestartMessageBox::RestartMessageBox(QWidget* parent)
    : QMessageBox(parent)
{
    setWindowTitle(tr("Pending changes"));
    setIcon(QMessageBox::Information);

    setText(tr("For the following settings to take place the application needs to be restarted."));

    setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    connect(this, &QMessageBox::accepted, this, &RestartMessageBox::restartApplication);
}

void RestartMessageBox::restartApplication()
{
    qApp->exit(ExitCode::EXIT_CODE_REBOOT);
}

} // namespace DialogueFromVideo
