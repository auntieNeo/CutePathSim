#ifndef INPUT_DIALOGS_H_
#define INPUT_DIALOGS_H_

#include <QDialog>
#include <QValidator>
#include <QVariant>

#include "ui_stringDialog.h"

namespace CutePathSim
{
  QString promptString(const QString &prompt);

  int promptNatural(const QString &prompt);

  class StringDialog : public QDialog, private Ui_StringDialog
  {
    Q_OBJECT

    public:
      StringDialog(QWidget *parent = 0);
      virtual ~StringDialog();

      void setPrompt(const QString &prompt) { m_promptLabel->setText(prompt); }
      QString getText() const { return m_stringEdit->text(); }
      void setValidator(const QValidator *v) { m_stringEdit->setValidator(v); }
  };

  class NaturalValidator : public QValidator
  {
    Q_OBJECT

    public:
      NaturalValidator(QObject *parent = 0);
      virtual ~NaturalValidator();

      State validate(QString & input, int &/*pos*/) const;
  };
}

#endif
