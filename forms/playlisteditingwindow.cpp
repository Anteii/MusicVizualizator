#include "playlisteditingwindow.h"
#include "ui_playlisteditingwindow.h"

#include <QMessageBox>

PlaylistEditingWindow::PlaylistEditingWindow(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PlaylistConfigurationWindow)
{
  ui->setupUi(this);
  ui->trackList->setDragEnabled(true);
  ui->trackList->setAcceptDrops(true);
  ui->trackList->setDropIndicatorShown(true);
  ui->trackList->setDefaultDropAction(Qt::MoveAction);
  setWindowTitle("Playlist");
  this->setWindowIcon(QIcon(":/Windows/resources/icons/windows/playlistEditingWindow.png"));
}

PlaylistEditingWindow::~PlaylistEditingWindow()
{
  delete ui;
}

void PlaylistEditingWindow::addTrack(TrackInfo track, bool checked)
{
  QListWidgetItem *item = new QListWidgetItem(
        QString((track.getName() + "." + track.getExt()).c_str()), ui->trackList
  );
  item->setFlags(item->flags() | (Qt::ItemIsUserCheckable));
  item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
  ui->trackList->addItem(item);
}

void PlaylistEditingWindow::setPlaylist(PlayList * _pl)
{
  pl = _pl;
  PlayList * alls = PlayList::getBaseTrackPlaylist();
  if (pl != NULL)
    loadTracks(pl);
  for(int i = 0; i < alls->size(); ++i){
      try {
        bool flag = false;
        TrackInfo t1 = alls->getCurrentSong();
        if (pl != NULL){
          flag = pl->contains(t1);
        }
        if (!flag)
          addTrack(t1, false);
      } catch (std::logic_error err) {
        continue;
      }
      alls->setPosition(alls->getPosition() + 1);
    }
  ui->playlistName->setPlaceholderText("Playlist name");
  if (pl != NULL){
      ui->playlistName->setText(QString(pl->getName().c_str()));
    }
}

void PlaylistEditingWindow::loadTracks(PlayList *pl)
{
  for(int i = 0; i < pl->size(); ++i){
      try {
        TrackInfo t1 = pl->getCurrentSong();
        addTrack(t1, true);
      } catch (std::logic_error err) {
        continue;
      }
      pl->setPosition(pl->getPosition() + 1);
    }
}

void PlaylistEditingWindow::setAllChecked()
{
  for(int i = 0; i < ui->trackList->count(); ++i){
      ui->trackList->item(i)->setCheckState(Qt::Checked);
    }
}

void PlaylistEditingWindow::accept()
{
  if (ui->playlistName->text() == "") {
      QMessageBox::warning(this, "Be carefull!", "Playlist name field is empty");
      return;
    }
  std::string name = ui->playlistName->text().toStdString();
  QString path = FileAssistant::getPlaylistPath(QString(name.c_str()));
  qDebug() << path;
  QList<QString> temp;
  for(int i = 0; i < ui->trackList->count(); ++i){
      auto item = ui->trackList->item(i);
      if (item->checkState() == Qt::Checked){
        temp.append(item->text());
        qDebug() << item->text();
      }
    }
  if (temp.size() == 0){
      QMessageBox::warning(this, "Be carefull!", "Can't create empty playlist");
      return;
    }
  FileAssistant::WriteFile(path, &temp);
  QDialog::accept();
}
