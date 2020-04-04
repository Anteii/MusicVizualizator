#include "graphic.h"
#include <QDebug>

Graphic::Graphic(QWidget *parent) : QOpenGLWidget(parent)
{
  initUpdaterThread();
}

void Graphic::setEffect(Visualization *ef)
{
  qDebug() << "SetEffect";
  effect = ef;
}

QOpenGLFunctions_4_5_Core *Graphic::getOGLF()
{
  return (OGLF*)this;
}

QOpenGLContext *Graphic::getContext()
{
  return context();
}

void Graphic::initEffect()
{
  if (effect != NULL){
      effect->init();
    }
  isInitedEffect = true;
}

void Graphic::deInitEffect()
{
  if (effect != NULL){
      effect->deInit();
    }
  isInitedEffect = false;
}

void Graphic::initUpdaterThread()
{

}

void Graphic::renderText(double x, double y, double z, const QString & str, const QFont & font = QFont(), int listBase = 2000)
{
      int height = this->height();

      GLdouble model[4][4], proj[4][4];
      GLint view[4];
      glGetDoublev(GL_MODELVIEW_MATRIX, &model[0][0]);
      glGetDoublev(GL_PROJECTION_MATRIX, &proj[0][0]);
      glGetIntegerv(GL_VIEWPORT, &view[0]);
      GLdouble textPosX = 0, textPosY = 0, textPosZ = 0;

      project(x, y, z,
                  &model[0][0], &proj[0][0], &view[0],
                  &textPosX, &textPosY, &textPosZ);

      textPosY = height - textPosY; // y is inverted

      QPainter painter(this);

      painter.setPen(Qt::yellow);
      painter.setFont(QFont("Helvetica", 8));
      painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
      painter.drawText(textPosX, textPosY, str); // z = pointT4.z + distOverOp / 4
      painter.end();
}

GLint Graphic::project(GLdouble objx, GLdouble objy, GLdouble objz, const GLdouble model[], const GLdouble proj[], const GLint viewport[], GLdouble *winx, GLdouble *winy, GLdouble *winz)
{
  GLdouble in[4], out[4];

      in[0] = objx;
      in[1] = objy;
      in[2] = objz;
      in[3] = 1.0;
      transformPoint(out, model, in);
      transformPoint(in, proj, out);

      if (in[3] == 0.0)
          return GL_FALSE;

      in[0] /= in[3];
      in[1] /= in[3];
      in[2] /= in[3];

      *winx = viewport[0] + (1 + in[0]) * viewport[2] / 2;
      *winy = viewport[1] + (1 + in[1]) * viewport[3] / 2;

      *winz = (1 + in[2]) / 2;
      return GL_TRUE;
}

void Graphic::transformPoint(GLdouble out[], const GLdouble m[], const GLdouble in[])
{
#define M(row,col)  m[col*4+row]
    out[0] =
        M(0, 0) * in[0] + M(0, 1) * in[1] + M(0, 2) * in[2] + M(0, 3) * in[3];
    out[1] =
        M(1, 0) * in[0] + M(1, 1) * in[1] + M(1, 2) * in[2] + M(1, 3) * in[3];
    out[2] =
        M(2, 0) * in[0] + M(2, 1) * in[1] + M(2, 2) * in[2] + M(2, 3) * in[3];
    out[3] =
        M(3, 0) * in[0] + M(3, 1) * in[1] + M(3, 2) * in[2] + M(3, 3) * in[3];
#undef M
}

void Graphic::initializeGL()
{
  initializeOpenGLFunctions();
  qDebug() << "Initialize";
}

void Graphic::resizeGL(int h, int w)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Graphic::paintGL()
{
  qDebug() << "paintGL";
  if (!isInitedEffect && effect != NULL){
      initEffect();
    }
  //renderText(0.5, 0.5, 0.5, QString("asap"));
  if (isInitedEffect && effect != NULL){
      effect->update();
    }

}

