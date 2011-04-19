#include "inputDialogs.h"

namespace CutePathSim
{
  StringDialog::StringDialog(QWidget *parent) : QDialog(parent)
  {
    setupUi(this);
  }

  StringDialog::~StringDialog()
  {
  }

  NaturalValidator::NaturalValidator(QObject *parent) : QValidator(parent)
  {
  }

  NaturalValidator::~NaturalValidator()
  {
  }

  QValidator::State NaturalValidator::validate(QString & input, int &/*pos*/) const
  {
    if(input.isEmpty())
      return Intermediate;

    bool ok;
    int number = QVariant(input).toInt(&ok);
    if(ok)
    {
      if(number > 0)
      {
        return Acceptable;
      }
    }
    return Invalid;
  }

  QString promptString(const QString &prompt)
  {
    StringDialog dialog;
    dialog.setPrompt(prompt);
    dialog.exec();
    if(dialog.result() == QDialog::Accepted)
    {
      return dialog.getText();
    }
    return QString();
  }

  int promptNatural(const QString &prompt)
  {
    StringDialog dialog;
    dialog.setPrompt(prompt);
    NaturalValidator v;
    dialog.setValidator(&v);
    dialog.exec();
    if(dialog.result() == QDialog::Accepted)
    {
      return QVariant(dialog.getText()).toInt();
    }
    return -1;
  }
}
