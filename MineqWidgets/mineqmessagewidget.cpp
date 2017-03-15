
#include <QLabel>
#include <QPixmap>
#include <QPushButton>

#include <QHBoxLayout>

#include "mineqmessagewidget.h"

MineqMessageWidget::MineqMessageWidget(QString workingDir, QWidget *parent)
    : m_workingDir(workingDir)
    , QWidget(parent)
{

}

MineqMessageWidget::~MineqMessageWidget()
{
    this->disconnect();

    QLayoutItem* item;
    while ( ( item = this->layout()->takeAt( 0 ) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    delete this->layout();
}

void MineqMessageWidget::CriticalMessage(QString msg)
{
    QVBoxLayout* vertLayout = new QVBoxLayout();

    InitializeWidget(*vertLayout, MessageInfoType::Critical, msg);

    this->setLayout(vertLayout);
}

void MineqMessageWidget::InformationMessage(QString msg)
{
    QVBoxLayout* vertLayout = new QVBoxLayout();

    InitializeWidget(*vertLayout, MessageInfoType::Information, msg);

    this->setLayout(vertLayout);
}

void MineqMessageWidget::QuestionMessage(QString callbackParam, QString msg)
{
    QVBoxLayout* vertLayout = new QVBoxLayout();

    InitializeWidget(*vertLayout, MessageInfoType::Question, msg);

    QHBoxLayout* horLayout = new QHBoxLayout();

    QString buttonStyleSheet("QPushButton"
                             "{"
                                    "border: none;"
                                    "color: #FFFFFF;"
                                    "background-color: #6A9EEC;"
                                    "width : 150;"
                                    "height: 50;"
                             "}");
    QFont butFont;
    butFont.setFamily("Ubuntu");
    butFont.setPointSize(18);

    QPushButton* noButton = new QPushButton("NO");
    noButton->setStyleSheet(buttonStyleSheet);
    noButton->setFont(butFont);
    noButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(noButton
            , SIGNAL(clicked())
            , this
            , SLOT(slot_on_no_button_clicked()));

    horLayout->addWidget(noButton);

    QPushButton* yesButton = new QPushButton("YES");
    yesButton->setStyleSheet(buttonStyleSheet);
    yesButton->setFont(butFont);
    yesButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(yesButton
            , SIGNAL(clicked())
            , this
            , SLOT(slot_on_yes_button_clicked()));
    horLayout->addWidget(yesButton);

    vertLayout->addLayout(horLayout);

    this->setLayout(vertLayout);

    m_callBackParam = callbackParam;
}

void MineqMessageWidget::DefaultMessage(QString msg)
{
    QVBoxLayout* vertLayout = new QVBoxLayout();

    InitializeWidget(*vertLayout, MessageInfoType::Default, msg);

    this->setLayout(vertLayout);
}

void MineqMessageWidget::InitializeWidget(QVBoxLayout &vertLayout
                                          , MessageInfoType msgType
                                          , QString text)
{
    if(text.isNull()) {
        SetDefaultText(msgType, text);
    }

    QString strLabelStyle ("QLabel"
                           "{"
                                "border: none;"
                                "qproperty-alignment: AlignCenter;"
                                "font: 16pt;"
                           "}");

    QLabel* imageLabel = new QLabel();
    imageLabel->setStyleSheet(strLabelStyle);
    QPixmap pix;

    switch(msgType) {
    case MessageInfoType::Critical:
        pix = QPixmap(m_workingDir + "/images/ic_error.png");
        break;
    case MessageInfoType::Information:
        pix = QPixmap(m_workingDir + "/images/ic_info.png");
        break;
    case MessageInfoType::Question:
        pix = QPixmap(m_workingDir + "/images/ic_question.png");
        break;
    case MessageInfoType::Default:
        pix = QPixmap(m_workingDir + "/images/ic_key.png");
    default:
        break;
    }

    imageLabel->setPixmap(pix);
    vertLayout.addWidget(imageLabel);

    QLabel* msgLabel = new QLabel();
    msgLabel->setStyleSheet(strLabelStyle);
    msgLabel->setText(text);
    vertLayout.addWidget(msgLabel);
}

void MineqMessageWidget::SetDefaultText(MessageInfoType msgType, QString &text)
{
    switch(msgType) {
    case MessageInfoType::Critical:
        text = "Asset configuration failed.\nPlease remove USB key and try agin.";
        break;
    case MessageInfoType::Information:
        text = "Asset configuration succeded.\nPlease remove USB key to reboot and apply configuration.";
        break;
    case MessageInfoType::Question:
        text = "The asset is currently configured.\n\nDo you want to reconfigure it?";
        break;
    case MessageInfoType::Default:
        text = "Please insert USB key to start asset configuration.";
        break;
    default:
        text = QString::null;
        break;
    }
}

void MineqMessageWidget::slot_on_no_button_clicked()
{
    emit mineqMesgClicked(m_callBackParam, MineqButton::No);
}

void MineqMessageWidget::slot_on_yes_button_clicked()
{
    emit mineqMesgClicked(m_callBackParam, MineqButton::Yes);
}

void MineqMessageWidget::slot_on_cancel_button_clicked()
{

}
