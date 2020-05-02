#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QOpenGLFunctions_4_3_Core>
#include <controllers/playercontroller.h>
#include <player_core/player.h>
#include "global_types/musicfile.h"
#define OGLF QOpenGLFunctions_4_3_Core


class Visualization{
public:
  virtual void init() = 0;
  virtual void deInit() = 0;
  virtual void update() = 0;
  virtual void setPlayerController(PlayerController * ctr) = 0;
};

#endif // VISUALIZATION_H
