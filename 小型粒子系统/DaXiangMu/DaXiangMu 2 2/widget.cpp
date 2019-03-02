#include "widget.h"
#include "ui_widget.h"
#include<QtOpenGL/QtOpenGL>
#include<QtOpenGL/glut.h>
#include<GL/glu.h>
#include<iostream>
#include<QPoint>
using namespace std;
Widget::Widget(QWidget *parent):
    QOpenGLWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    lookangle = 40;
    play = true;

    lookX = -5;
    lookY = 30;
    lookZ = 20;
    Dx=-5;
    Dy=0;
    Dz=0;

    slowdown = 2.0f;
    Gx = 0;
    Gy = -0.8;
    Gz = 0.8;

    xspeed = 1.0;
    yspeed = 46;
    temptime = 0.001f;
    R=0;
    G=0;
    B=0.9;
    ui->scrollAreaWidgetContents->setMouseTracking(true);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadGLTextures(GLuint *temp,const char *filename)
{
    QImage tex,buf;
    if(!buf.load(filename))
    {
        qWarning("55");
        QImage dummy(128,128,QImage::Format::Format_RGB32);
        dummy.fill(QColor(Qt::green));
        buf = dummy;
    }
    tex = buf;
        glGenTextures(1, temp);
        glBindTexture(GL_TEXTURE_2D, *temp);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void Widget::initializeGL()
{
       initializeOpenGLFunctions();

       glEnable(GL_TEXTURE_2D);
       glShadeModel(GL_SMOOTH);
       glClearColor(0.0,0.0,0.0,0.0);
       glClearDepthf(1.0);

       glEnable(GL_DEPTH_TEST);
       glDepthFunc(GL_LEQUAL);


       setWindowTitle("Lizi");
       setGeometry(400,200,700,700);

       loadGLTextures(&picturewater,":/shuidi.bmp");
       loadGLTextures(&picturevolcano,":/fire.bmp");
       loadGLTextures(&picturesnow,":/snow.bmp");
       loadGLTextures(&picturshan,":/shanti3.bmp");
       loadGLTextures(&pictureshan2,":/shanti1.bmp");
       loadGLTextures(&picturefire,":/fire.bmp");
       loadGLTextures(&picturemuto,":/muto2.bmp");


}

void Widget::paintGL()
{
     glClear(GL_DEPTH_BUFFER_BIT);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(lookangle, 1, 1, 200);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     gluLookAt(lookX, lookY, lookZ,Dx, Dy, Dz,0, 1, 0);

     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA,GL_ONE);

     glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
     glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);


     if(volcano)
     {
         drawshan();
         lizivolcano();
     }
     if(snow)
     {
         lizisnow();
     }
     if(water)
     {
         liziwater();
     }
    if(through)
    {
        lizigothrough();
    }
    if(waterfall)
    {
        drawshan2();
        liziwaterfall();
        drawshan3();
    }
    if(fire)
    {
        drawmuto();
        lizifire();
    }
    if(play)
    {
         update();
    }
    else
    {
        if(water)
        {
            liziwater();
        }
        if(volcano)
        {
            drawmuto();
            lizivolcano();
        }
        if(snow)
        {
            lizisnow();
        }
        if(through)
        {
            lizigothrough();
        }
        if(waterfall)
        {
            drawshan2();
            liziwaterfall();
            drawshan3();
        }
        if(fire)
        {
            drawmuto();
            lizifire();
        }
    }
}

void Widget::resizeGL(int w,int h)
{
    glViewport(0,0,w,h);
}

void Widget::liziwater()
{
     glClear(GL_DEPTH_BUFFER_BIT);
     glBindTexture(GL_TEXTURE_2D, picturewater);

     for(Num=0;Num<MAXLIZI;Num++)
     {
         if(lizi[Num].life>=0.0f)
         {
             float x = lizi[Num].x;
             float y = lizi[Num].y;
             float z = lizi[Num].z-30.0f;
             glColor4f(lizi[Num].r,lizi[Num].g,lizi[Num].b,lizi[Num].life);
             glBegin((GL_TRIANGLE_STRIP));
             glTexCoord2d(1,1);
             glVertex3f(x+size,y+size,z);
             glTexCoord2d(0,1);
             glVertex3f(x-size,y+size,z);
             glTexCoord2d(1,0);
             glVertex3f(x-size,y-size,z);
             glTexCoord2d(0,0);
             glVertex3f(x+size,y-size,z);
             glEnd();
             lizi[Num].x+=lizi[Num].xi/(slowdown*100);
             lizi[Num].y+=lizi[Num].yi/(slowdown*100);
             lizi[Num].z+=lizi[Num].zi/(slowdown*100);
             lizi[Num].xi+=lizi[Num].xg;
             lizi[Num].yi+=lizi[Num].yg;
             lizi[Num].zi+=lizi[Num].zg;
             lizi[Num].life -= lizi[Num].dying;
             if(lizi[Num].life <0.0f)
             {
                 lizi[Num].life=1.0f;
                 lizi[Num].dying = temptime+float(rand()%100)/1000.0f;
                 lizi[Num].x = Mx;
                 lizi[Num].y = My;
                 lizi[Num].z = 0.0f;
                 lizi[Num].xi=xspeed+float((rand()%60)-32.0f);
                 lizi[Num].yi=yspeed;
                 lizi[Num].zi=float((rand()%60)-32.0f);
                 lizi[Num].r = R;
                 lizi[Num].g = G;
                 lizi[Num].b = B;
             }
         }
     }
}

void Widget::lizivolcano()
{

    glBindTexture(GL_TEXTURE_2D,picturevolcano);
    for(Num=0;Num<MAXLIZI;Num++)
    {
        if(lizi[Num].life>=0.0f)
        {
            float x = lizi[Num].x;
            float y = lizi[Num].y;
            float z = lizi[Num].z+-30.0f;
            glColor4f(lizi[Num].r,lizi[Num].g,lizi[Num].b,lizi[Num].life);

            glBegin((GL_TRIANGLE_STRIP));
            glTexCoord2d(1,1);
            glVertex3f(x+size,y+size,z);

            glTexCoord2d(0,1);
            glVertex3f(x-size,y+size,z);

            glTexCoord2d(1,0);
            glVertex3f(x+size,y-size,z);

            glTexCoord2d(0,0);
            glVertex3f(x-size,y-size,z);
            glEnd();

            glBegin((GL_TRIANGLE_STRIP));
            glTexCoord2d(1,1);
            glVertex3f(x+size+0.1,y+size+0.1,z);

            glTexCoord2d(0,1);
            glVertex3f(x-size-0.1,y+size+0.1,z);

            glTexCoord2d(1,0);
            glVertex3f(x+size+0.1,y-size-0.1,z);
            glTexCoord2d(0,0);
            glVertex3f(x-size-0.1,y-size-0.1,z);
            glEnd();

            lizi[Num].x+=lizi[Num].xi/(slowdown*100);
            lizi[Num].y+=lizi[Num].yi/(slowdown*100);
            lizi[Num].z+=lizi[Num].zi/(slowdown*100);

            lizi[Num].xi += lizi[Num].xg;
            lizi[Num].yi += lizi[Num].yg;
            lizi[Num].zi += lizi[Num].zg;

            lizi[Num].life -= lizi[Num].dying;

            if(lizi[Num].life <0.0f)
            {
                lizi[Num].life=1.0f;
                lizi[Num].dying = temptime+float(rand()%100)/100.0f;
                lizi[Num].x = Mx;
                lizi[Num].y = My;
                lizi[Num].z = 0.0f;

                lizi[Num].xi=xspeed+float((rand()%60)-32.0f);
                lizi[Num].yi=yspeed;
                lizi[Num].zi=float((rand()%60)-32.0f);

                lizi[Num].r = R;
                lizi[Num].g = G;
                lizi[Num].b = B;
            }
        }
    }
}

void Widget::lizisnow()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D,picturesnow);
    for(Num=0;Num<MAXLIZI;Num++)
    {
        if(lizi[Num].life>=0.0f)
        {
            float x = lizi[Num].x;
            float y = lizi[Num].y;
            float z = lizi[Num].z+-30.0f;
            glColor4f(lizi[Num].r,lizi[Num].g,lizi[Num].b,lizi[Num].life);
            glBegin((GL_TRIANGLE_STRIP));
            glTexCoord2d(1,1);
            glVertex3f(x+size,y+size,z);

            glTexCoord2d(0,1);
            glVertex3f(x-size,y+size,z);

            glTexCoord2d(1,0);
            glVertex3f(x+size,y-size,z);

            glTexCoord2d(0,0);
            glVertex3f(x-size,y-size,z);
            glEnd();

            lizi[Num].x+=lizi[Num].xi/(slowdown*100);
            lizi[Num].y+=lizi[Num].yi/(slowdown*100);
            lizi[Num].z+=lizi[Num].zi/(slowdown*100);

            lizi[Num].xi += lizi[Num].xg;
            lizi[Num].yi += lizi[Num].yg;
            lizi[Num].zi += lizi[Num].zg;

            lizi[Num].life -= lizi[Num].dying;

            if(lizi[Num].life <0.0f)
            {
                lizi[Num].life=1.0f;
                lizi[Num].dying = temptime+float(rand()%100)/1000.0f;

                lizi[Num].x = rand()%40-25;
                lizi[Num].y = 0;
                lizi[Num].z = 0.0f;

                lizi[Num].xi=  xspeed+float((rand() % 30) - 10.0f) * 5.0f;
                lizi[Num].yi = yspeed+float((rand() % 30) - 26.0f) * 5.0f;
                lizi[Num].zi= float((rand()%10)-5.0f);

                lizi[Num].r = R;
                lizi[Num].g = G;
                lizi[Num].b = B;
            }
        }
    }
}

void Widget::lizigothrough()
{

    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_TEXTURE_2D);

    for(Num=0;Num<MAXLIZI;Num++)
    {
        if(lizi[Num].life>=0.0f)
        {
            float x = lizi[Num].x;
            float y = lizi[Num].y;
            float z = lizi[Num].z+-30.0f;
            glColor4f(lizi[Num].r,lizi[Num].g,lizi[Num].b,lizi[Num].life);

            glBegin((GL_TRIANGLE_STRIP));
            glVertex3f(x+size,y+size,z);
            glVertex3f(x-size,y+size,z);
            glVertex3f(x+size,y-size,z);
            glVertex3f(x-size,y-size,z);
            glEnd();
            lizi[Num].x+=lizi[Num].xi/(slowdown*100);
            lizi[Num].y+=lizi[Num].yi/(slowdown*100);
            lizi[Num].z+=lizi[Num].zi/(slowdown*100);
            lizi[Num].xi += lizi[Num].xg;
            lizi[Num].yi += lizi[Num].yg;
            lizi[Num].zi += lizi[Num].zg;
            lizi[Num].life -= lizi[Num].dying;
            if(lizi[Num].life <0.0f)
            {
                lizi[Num].life=1.0f;
                lizi[Num].dying = temptime+float(rand()%100)/1000.0f;
                lizi[Num].x = Mx;
                lizi[Num].y = My;
                lizi[Num].z = 0.0f;

                lizi[Num].xi=xspeed+float((rand()%60)-32.0f);
                lizi[Num].yi=yspeed;
                lizi[Num].zi=float((rand()%60)-32.0f);
                /*lizi[Num].r = R;
                lizi[Num].g = G;
                lizi[Num].b = B;*/
                lizi[Num].r = float(rand()%10)/100.0f+R;
                lizi[Num].g = float(rand()%10)/100.0f+G;
                lizi[Num].b = float(rand()%10)/100.0f+B;
            }
        }
    }
    glEnable(GL_TEXTURE_2D);
}

void Widget::liziwaterfall()
{
    for(Num=0;Num<MAXLIZI;Num++)
    {
        if(lizi[Num].life>0)
        {
            float x = lizi[Num].x;
            float y = lizi[Num].y;
            float z = lizi[Num].z+-30.0f;
            glColor4f(lizi[Num].r,lizi[Num].g,lizi[Num].b,lizi[Num].life);
            glBegin((GL_TRIANGLE_STRIP));
            glTexCoord2d(1,1);
            glVertex3f(x+size,y+size,z);

            glTexCoord2d(0,1);
            glVertex3f(x-size,y+size,z);

            glTexCoord2d(1,0);
            glVertex3f(x+size,y-size,z);

            glTexCoord2d(0,0);
            glVertex3f(x-size,y-size,z);
            glEnd();

            lizi[Num].x+=lizi[Num].xi/(slowdown*100);
            lizi[Num].y+=lizi[Num].yi/(slowdown*100);
            lizi[Num].z+=lizi[Num].zi/(slowdown*100);

            lizi[Num].xi += lizi[Num].xg;
            lizi[Num].yi += lizi[Num].yg;
            lizi[Num].zi += lizi[Num].zg;

            lizi[Num].life -= lizi[Num].dying;

            if(lizi[Num].life <=0.0f||lizi[Num].y<=-70)
            {
                lizi[Num].life=1.0f;
                lizi[Num].dying = temptime+float(rand()%100)/1000.0f;

                lizi[Num].x =rand()%5-2.5;
                lizi[Num].y = -20+(float(rand()%1000))/500.0f;
                lizi[Num].z = 0.0f;

                lizi[Num].xi=  xspeed+float((rand() % 30) - 10.0f) * 5.0f;
                lizi[Num].yi = yspeed+float((rand() % 30) - 26.0f) * 5.0f;
                lizi[Num].zi= float((rand()%10)-5.0f);

                lizi[Num].r = R;
                lizi[Num].g = G;
                lizi[Num].b = B;
            }
        }
    }
}

void Widget::lizifire()
{
    glBindTexture(GL_TEXTURE_2D,picturefire);
    for(Num=0;Num<MAXLIZI;Num++)
    {
        if(lizi[Num].life>=0.0f)
        {
            float x = lizi[Num].x;
            float y = lizi[Num].y;
            float z = lizi[Num].z+-30.0f;
            glColor4f(lizi[Num].r,lizi[Num].g,lizi[Num].b,lizi[Num].life);
            glBegin((GL_TRIANGLE_STRIP));
            glTexCoord2d(1,1);
            glVertex3f(x+size,y+size,z);

            glTexCoord2d(0,1);
            glVertex3f(x-size,y+size,z);

            glTexCoord2d(1,0);
            glVertex3f(x+size,y-size,z);

            glTexCoord2d(0,0);
            glVertex3f(x-size,y-size,z);
            glEnd();

            lizi[Num].x+=lizi[Num].xi/(slowdown*100);
            lizi[Num].y+=lizi[Num].yi/(slowdown*100);
            lizi[Num].z+=lizi[Num].zi/(slowdown*100);

            lizi[Num].xi += lizi[Num].xg;
            lizi[Num].yi += lizi[Num].yg;
            lizi[Num].zi += lizi[Num].zg;

            lizi[Num].life -= lizi[Num].dying;

            if(lizi[Num].life <0.0f)
            {
                lizi[Num].life=1.0f;
                lizi[Num].dying = temptime+float(rand()%100)/1000.0f;

                lizi[Num].x = rand()%6-3;
                lizi[Num].y = -30+rand()%2-1;
                lizi[Num].z = rand()%2-1;;

                lizi[Num].xi=  xspeed+float((rand() % 30) - 10.0f) * 5.0f;
                lizi[Num].yi = yspeed+float((rand() % 30) - 26.0f) * 5.0f;
                lizi[Num].zi= 0;

                lizi[Num].r = R;
                lizi[Num].g = G;
                lizi[Num].b = B;
            }
        }
    }
}

void Widget::drawshan()
{
    glBindTexture(GL_TEXTURE_2D, picturshan);
    glColor4f(0.86,0.66,0.41,0.8);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1);glVertex2f(0.5,2.2);
    glTexCoord2d(0,1);glVertex2f(-0.5,2.2);
    glTexCoord2d(0,0);glVertex2f(-4,-10);
    glTexCoord2d(1,0);glVertex2f(4,-10);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);

}

void Widget::drawshan2()
{
    glBindTexture(GL_TEXTURE_2D, pictureshan2);
    glColor4f(0.86,0.66,0.41,0.8);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1);glVertex2f(-1,11);
    glTexCoord2d(0,1);glVertex2f(-2,11);
    glTexCoord2d(0,0);glVertex2f(-6,-10);
    glTexCoord2d(1,0);glVertex2f(4,-10);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);

}

void Widget::drawshan3()
{
    glBindTexture(GL_TEXTURE_2D, pictureshan2);
    glColor4f(0.86,0.66,0.41,0.8);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1);glVertex2f(-4,11);
    glTexCoord2d(0,1);glVertex2f(-5,11);
    glTexCoord2d(0,0);glVertex2f(-14,-10);
    glTexCoord2d(1,0);glVertex2f(-9,-10);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
}

void Widget::drawmuto()
{
    glBindTexture(GL_TEXTURE_2D, picturemuto);
    glColor4f(0.8706,0.7216,0.5294,1.0f);
    glBegin(GL_QUADS);
    glTexCoord2d(1,1);glVertex2f(-1.7,5.5);
    glTexCoord2d(0,1);glVertex2f(-4.6,5.5);
    glTexCoord2d(0,0);glVertex2f(-2.5,-10);
    glTexCoord2d(1,0);glVertex2f(-2,-10);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_1)
    {
        lookY += 1;
        if(lookY>=66)
            lookY=66;
    }

    if(event->key()==Qt::Key_2)
    {
        lookY += 1;
        if(lookY>=66)
            lookY=66;
        Dy+=1;
        if(Dy>=36)
            Dy=36;
    }

    if(event->key()==Qt::Key_3)
    {
        lookY -= 1;
        if(lookY<0.0)
            lookY=0.5;
    }

    if(event->key()==Qt::Key_4)
    {
        lookX += 1;
        Dx  +=1;
    }

    if(event->key()==Qt::Key_5)
    {
        if(water)
        {
            lookangle = 40;
            Dx=-5;
            lookX=-5;
        }
        if(volcano)
        {
            lookangle = 14;
            Dx=0;
            lookX=0;
        }
        if(snow)
        {
            lookangle = 40;
            Dx=-5;
            lookX=-5;
        }
        if(through)
        {
            lookangle = 60;
            Dx=-5;
            lookX=-5;
        }
        if(fire)
        {
            lookangle = 40;
            Dx=-5;
            lookX=-5;
        }
        if(waterfall)
        {
            lookangle = 40;
            Dx = -5;
            lookX = -5;
        }
        lookY = 30;
        lookZ = 20;

        Dy=0;
        Dz=0;
    }

    if(event->key()==Qt::Key_6)
    {
        lookX -= 1;
        Dx  -=1;
    }

    if(event->key()==Qt::Key_7)
    {
        if(size>0.02)
        {
            size-=0.02;
        }
    }

    if(event->key()==Qt::Key_8)
    {

        lookY -= 1;
        if(lookY<=30.0)
            lookY=30;
        Dy-=1;
        if(Dy<0.0)
            Dy=0;
    }

    if(event->key()==Qt::Key_9)
    {
        if(size<1.98)
        {
            size+=0.02;
        }
    }

    if(event->key()==Qt::Key_Minus)
    {
        lookangle += 1;
    }

    if(event->key()==Qt::Key_Plus)
    {
        lookangle -= 1;
    }


    if(event->key()==Qt::Key_A)
    {
            slowdown +=0.3;
            if(slowdown>=5)
                slowdown = 5;
    }

    if(event->key()==Qt::Key_D)
    {
            slowdown -=0.3;
            if(slowdown<0.3)
                slowdown = 0.3;
    }

    if(event->key()==Qt::Key_W)
    {
        if(fire==false)
        for(Num=0;Num<MAXLIZI;Num++)
        {
            lizi[Num].yg -=0.3;
            if(lizi[Num].yg<-5.0)
                lizi[Num].yg = -5.0;
        }
        else
        {
            for(Num=0;Num<MAXLIZI;Num++)
            {
                lizi[Num].yg -=0.3;
                if(lizi[Num].yg<-105.0)
                    lizi[Num].yg = -105.0;
            }
        }
    }

    if(event->key()==Qt::Key_S)
    {
        if(fire==false)
         for(Num=0;Num<MAXLIZI;Num++)
            {
                lizi[Num].yg +=0.3;
                if(lizi[Num].yg>5.0)
                  lizi[Num].yg = 5.0;
            }
        else
        {
            for(Num=0;Num<MAXLIZI;Num++)
               {
                   lizi[Num].yg +=0.3;
                   if(lizi[Num].yg>105.0)
                     lizi[Num].yg = 105.0;
               }
        }
    }

    if(event->key()==Qt::Key_P)
    {
            play = false;
    }

    if(event->key()==Qt::Key_C)
    {
            play = true;
    }

    if(event->key()==Qt::Key_Right)
    {
        xspeed +=5;
        if(xspeed>=60)
            xspeed = 60;
    }

    if(event->key()==Qt::Key_Left)
    {
        xspeed -=5;
        if(xspeed<=-60)
            xspeed = -60;
    }

    if(event->key()==Qt::Key_Down)
    {
        yspeed -= 5;

    }

    if(event->key()==Qt::Key_Up)
    {
       yspeed += 5;

    }
    update();
}

void Widget::on_horizontalSlider_sliderMoved(int position)
{
    size=((float)position)/100;
    cout<<position;
}

void Widget::mouseMoveEvent(QMouseEvent *event)//auto hide the layoutwidget
{
    if(event->x()<=131&&event->y()<=485)
    {
        ui->scrollArea->show();
        update();
    }
    else
    {
        ui->scrollArea->hide();
        update();
    }
}

void Widget::on_Spray_clicked()
{
    slowdown = 2.0f;
    Gx = 0;
    Gy = -0.8;
    Gz = 0.8;
    xspeed = 1.0;
    yspeed = 46;
    temptime = 0.001;
    lookangle = 40;
    R=0;
    G=0;
    B=0.9;
    volcano = false;
    water = true;
    snow = false;
    through = false;
    waterfall = false;
    fire = false;
    Mx = 0;
    My = -20;
    size=0.15;
    for(Num=0;Num<MAXLIZI;Num++)
    {
          lizi[Num].life = 1.0f;
          lizi[Num].dying = float(rand()%100)/1000.0f+temptime;
          lizi[Num].r = R;
          lizi[Num].g = G;
          lizi[Num].b = B;

          lizi[Num].x = Mx;
          lizi[Num].y = My;
          lizi[Num].z = 0;

          lizi[Num].xi = 0.5f;
          lizi[Num].yi = 0.8f;
          lizi[Num].zi = 0.1f;

          lizi[Num].xg=Gx;
          lizi[Num].yg=Gy;
          lizi[Num].zg=Gz;
    }
}

void Widget::on_Snow_clicked()
{
    snow = true;
    water = false;
    volcano = false;
    through=false;
    waterfall = false;
    fire = false;
    temptime=0.001;
    R = 0.5;
    G = 0.5;
    B = 1.0;
    xspeed = 1.0;
    yspeed = 1.0;
    Gx = 0;
    Gy = -0.8;
    Gz = 0.8;
    lookangle = 40;
    slowdown = 5.0f;
    size=0.3;
    Dx=-5;
    lookX=-5;
    for(Num=0;Num<MAXLIZI;Num++)
    {
          lizi[Num].life = 1.0f;
          lizi[Num].dying = float(rand()%100)/1000.0f+temptime;
          lizi[Num].r = R;
          lizi[Num].g = G;
          lizi[Num].b = B;

          lizi[Num].x = rand()%40-25;
          lizi[Num].y = 0;
          lizi[Num].z = 0;

          lizi[Num].xi = float((rand() % 40) - 10.0f) * 10.0f;
          lizi[Num].yi = float((rand() % 30) - 26.0f) * 5.0f;
          lizi[Num].zi = float((rand() % 40) - 36.0f) * 10.0f;

          lizi[Num].xg=Gx;
          lizi[Num].yg=Gy;
          lizi[Num].zg=Gz;
    }
}

void Widget::on_Volcano_clicked()
{
    temptime = 0.02;
    Mx = 0;
    My = -40;
    R = 1.0;
    G = 0.1;
    B = 0.0;
    slowdown = 2.0f;
    Gx = 0;
    Gy = 3;
    Gz = 0.8;
    xspeed = 1.0;
    yspeed = 65.0;
    volcano = true;
    water = false;
    snow = false;
    through=false;
    waterfall = false;
    fire = false;
    lookangle = 14;
    size=0.5;
    Dx=0;
    lookX=0;
    for(Num=0;Num<MAXLIZI;Num++)
    {
          lizi[Num].life = 1.0f;
          lizi[Num].dying = float(rand()%100)/1000.0f+temptime;
          lizi[Num].r = R;
          lizi[Num].g = G;
          lizi[Num].b = B;

          lizi[Num].x = Mx;
          lizi[Num].y = My;
          lizi[Num].z = 0;

          lizi[Num].xi = -0.5f;
          lizi[Num].yi = 0.4f;
          lizi[Num].zi = 0.1f;

          lizi[Num].xg=Gx;
          lizi[Num].yg=Gy;
          lizi[Num].zg=Gz;

    }

}

void Widget::on_Universe_clicked()
{
    snow = false;
    volcano=false;
    water=false;
    through=true;
    waterfall = false;
    fire = false;
    Mx = 0;
    My = -30;
    R=0.6235;
    G=0.4745;
    B=0.9333;
    Gx=0;
    Gy=1.0;
    Gz=1.0;
    xspeed = 0.0;
    yspeed = 0.0;
    slowdown=3.0f;
    lookangle=60;
    temptime = 0.001;
    size=0.2;
    Dx=-5;
    lookX=-5;
    for(Num=0;Num<MAXLIZI;Num++)
    {
          lizi[Num].life = 1.0f;
          lizi[Num].dying = float(rand()%100)/1000.0f+temptime;
          lizi[Num].r = R;
          lizi[Num].g = G;
          lizi[Num].b = B;

          lizi[Num].x = Mx;
          lizi[Num].y = My;
          lizi[Num].z = 0;

          lizi[Num].xi = 0.5;
          lizi[Num].yi = 0.8f;
          lizi[Num].zi = 0.8f;

          lizi[Num].xg=Gx;
          lizi[Num].yg=Gy;
          lizi[Num].zg=Gz;

    }
}

void Widget::on_Waterfall_clicked()
{
    Dx=-5;
    lookX=-5;
    snow = false;
    water = false;
    volcano = false;
    through=false;
    waterfall=true;
    temptime=0.001;
    R = 0.401282;
    G = 0.557046;
    B = 0.998688;
    xspeed = 2.0;
    yspeed = 0.0;
    Gx = -3;
    Gy = -10.0;
    Gz = 0.0;
    lookangle = 40;
    slowdown = 5.0f;
    size=0.25;
    for(Num=0;Num<MAXLIZI;Num++)
    {
          lizi[Num].life = 1.0f;
          lizi[Num].dying = float(rand()%100)/1000.0f+temptime;
          lizi[Num].r = R;
          lizi[Num].g = G;
          lizi[Num].b = B;

          lizi[Num].x =rand()%10-5;
          lizi[Num].y = -20+(float(rand()%1000))/500.0f;
          lizi[Num].z = 0.0f;

          lizi[Num].xi = float((rand() % 20) - 10.0f) * 5.0f;
          lizi[Num].yi = float((rand() % 30) - 26.0f) * 5.0f;
          lizi[Num].zi = float((rand() % 40) - 36.0f) * 10.0f;

          lizi[Num].xg=Gx;
          lizi[Num].yg=Gy;
          lizi[Num].zg=Gz;
    }
    update();
}

void Widget::on_Fire_clicked()
{
    snow = false;
    water = false;
    volcano = false;
    through=false;
    waterfall = false;
    fire = true;
    temptime=0.02;
    R = 1.0;
    G = 0.1906;
    B = 0.0;
    xspeed = rand()%2-1;
    yspeed = 100.0;
    Gx = 0;
    Gy = 6.0;
    Gz = 0.0;
    lookangle = 40;
    slowdown = 5.0f;
    size=0.8;
    Dx=-5;
    lookX=-5;
    for(Num=0;Num<MAXLIZI;Num++)
    {
          lizi[Num].life = 1.0f;
          lizi[Num].dying = float(rand()%100)/1000.0f+temptime;
          lizi[Num].r = R;
          lizi[Num].g = G;
          lizi[Num].b = B;

          lizi[Num].x = rand()%6-3;
          lizi[Num].y = -30+rand()%2-1;
          lizi[Num].z = rand()%2-1;

          lizi[Num].xi = float((rand() % 40) - 10.0f) * 10.0f;
          lizi[Num].yi = float((rand() % 30) - 26.0f) * 5.0f;
          lizi[Num].zi = 0/*float((rand() % 40) - 36.0f) * 10.0f*/;

          lizi[Num].xg=Gx;
          lizi[Num].yg=Gy;
          lizi[Num].zg=Gz;

    }

}

void Widget::on_Add_time_clicked()
{
    temptime -= 0.005;
    if(temptime<=0.0f)
        temptime = 0.0f;
}

void Widget::on_Reduce_time_clicked()
{
     temptime += 0.005;
}

void Widget::on_Init_position_clicked()
{
    if(water)
    {
        lookangle = 40;
        Dx=-5;
        lookX=-5;
    }
    if(volcano)
    {
        lookangle = 14;
        Dx=0;
        lookX=0;
    }
    if(snow)
    {
        lookangle = 40;
        Dx=-5;
        lookX=-5;
    }
    if(through)
    {
        lookangle = 60;
        Dx=-5;
        lookX=-5;
    }
    if(fire)
    {
        lookangle = 40;
        Dx=-5;
        lookX=-5;
    }
    if(waterfall)
    {
        lookangle = 40;
        Dx = -5;
        lookX = -5;
    }
    lookY = 30;
    lookZ = 20;

    Dy=0;
    Dz=0;
}

void Widget::on_Color_clicked()
{
    QColor newcolor = QColorDialog::getColor();
    if(newcolor.isValid())
    {
        R = newcolor.redF();
        G = newcolor.greenF();
        B = newcolor.blueF();
        update();
    }
}

void Widget::on_Help_clicked()
{
    QTextEdit *showText = new QTextEdit();
    QFile file(":/Help.txt");
    if (file.open(QIODevice ::ReadOnly | QIODevice ::Text))
    {
        QTextStream textStream(&file);
        while (!textStream.atEnd())
        {
            showText->append(textStream.readLine());
            showText->resize(450,600);
            showText->show();
        }
    }
    else
    {
        QMessageBox ::information(NULL, NULL, "Error!!! No Such Txt File");
    }
}

void Widget::on_pushButton_clicked()
{
    play=false;
    update();
}

void Widget::on_pushButton_2_clicked()
{
    play=true;
    update();
}

void Widget::on_pushButton_3_clicked()
{
    lookangle-=1;
    update();
}

void Widget::on_pushButton_4_clicked()
{
    lookangle+=1;
    update();
}
