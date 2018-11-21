#ifndef MINEQMESSAGEWIDGET_H
#define MINEQMESSAGEWIDGET_H

#include <QVBoxLayout>

#include <QWidget>

enum class MessageInfoType
{
    _undefined = 0,
    Default,
    Critical,
    Information,
    Question
};

enum class MineqButton
{
    Cancel,
    Yes,
    No
};

class MineqMessageWidget
        : public QWidget
{
    Q_OBJECT
public:
    explicit MineqMessageWidget(QWidget *parent = nullptr);
    ~MineqMessageWidget();

    void CriticalMessage(QString msg = QString::null);

    void InformationMessage(QString msg = QString::null);

    void QuestionMessage(QString callbackParam = QString::null, QString msg = QString::null);

    void DefaultMessage(QString msg = QString::null);

private:
    void InitializeWidget(QVBoxLayout& vertLayout
                          , MessageInfoType msgType
                          , QString text = QString::null);

    void SetDefaultText(MessageInfoType msgType, QString& text);

signals:
    void mineqMesgClicked(QString val, MineqButton but);

private slots:
    void slot_on_no_button_clicked();
    void slot_on_yes_button_clicked();
    void slot_on_cancel_button_clicked();

private:
    QString m_workingDir;
    QString m_callBackParam;
};

#endif // MINEQMESSAGEWIDGET_H
