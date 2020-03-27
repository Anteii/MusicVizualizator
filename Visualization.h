#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include <QOpenGLFunctions_4_5_Core>
#define OGLF QOpenGLFunctions_4_5_Core

class Visualization{
public:
  virtual void init() = 0;
  virtual void update() = 0;
  virtual void setOGLFunctionPointer(OGLF *) = 0;
};

#endif // VISUALIZATION_H