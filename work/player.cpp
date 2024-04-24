#include "player.h"
#include "ui_player.h"
#include "login.h"
Player::Player(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Player)
{
    ui->setupUi(this);
    player = new QMediaPlayer;
    readMusicfile();
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(musicPlayerDurationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)),this, SLOT(mediaPlayerPositionChanged(qint64)));

}

Player::~Player()
{
     delete ui;
}

void Player::readMusicfile(){
    MusicPath = QFileDialog::getExistingDirectory();
    QDir dir(MusicPath);
    QStringList nameFileters;
    nameFileters<<"*.mp3";
    QStringList files=dir.entryList(nameFileters,QDir::Files|QDir::Readable,QDir::Name);
    ui->musiclist->addItems(files);
    ui->musiclist->sortItems();
    ui->current_music->setText(ui->musiclist->item(0)->text());
    player->setMedia(QUrl::fromLocalFile(MusicPath+'/'+ui->musiclist->item(0)->text()));
    player->stop();
}


void Player::on_pushButton_2_clicked()
{
    if(is_play){
        is_play=false;
        player->pause();
    }else{
        is_play=true;
        player->play();
    }
}

void Player::on_pushButton_clicked()
{
    int currentrow=ui->musiclist->currentRow();
    int rowcount=ui->musiclist->count();
    ui->musiclist->setCurrentRow((currentrow-1+rowcount)%rowcount);
}

void Player::on_pushButton_3_clicked()
{
    int currentrow=ui->musiclist->currentRow();
    int rowcount=ui->musiclist->count();
    ui->musiclist->setCurrentRow((currentrow+1)%rowcount);
}

void Player::on_musiclist_currentRowChanged(int currentRow)
{
    ui->current_music->setText(ui->musiclist->item(currentRow)->text());
    player->setMedia(QUrl::fromLocalFile(MusicPath+'/'+ui->musiclist->item(currentRow)->text()));
    player->play();
    is_play=true;
}


void Player::musicPlayerDurationChanged(qint64 duration)
{
    ui->horizontalSlider->setRange(0, duration / 1000);
    int second = duration / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaDuration;
    mediaDuration.clear();

    if (minute >= 10)
        mediaDuration = QString::number(minute, 10);
    else
        mediaDuration = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaDuration = mediaDuration+ ":" + QString::number(second, 10);
    else
        mediaDuration = mediaDuration+ ":0" + QString::number(second, 10);

    /* 显示媒体总长度时间 */
    ui->label_4->setText(mediaDuration);
}

void Player::mediaPlayerPositionChanged(qint64 position)
{
    if (!ui->horizontalSlider->isSliderDown())
        ui->horizontalSlider->setValue(position/1000);

    int second = position / 1000;
    int minute = second / 60;
    second %= 60;

    QString mediaPosition;
    mediaPosition.clear();

    if (minute >= 10)
        mediaPosition = QString::number(minute, 10);
    else
        mediaPosition = "0" + QString::number(minute, 10);

    if (second >= 10)
        mediaPosition = mediaPosition+ ":" + QString::number(second, 10);
    else
    mediaPosition = mediaPosition+ ":0" + QString::number(second, 10);

    /* 显示现在播放的时间 */
    ui->label_3->setText(mediaPosition);
    //自动播放下一曲
    if(player->position()==player->duration() && player->position()!=0 && player->duration()!=0){
        on_pushButton_3_clicked();
    }
}

void Player::on_horizontalSlider_sliderReleased()
{
    /* 设置媒体播放的位置 */
    player->setPosition(ui->horizontalSlider->value() * 1000);
}
