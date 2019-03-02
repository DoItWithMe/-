#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include<QtOpenGL/QtOpenGL>
#include<QtOpenGL/glut>
#include<QOpenGLFunctions>
#include<QOpenGLWidget>
#include<windows.h>
#define MAXLIZI 20000

namespace Ui {
class Widget;
}

class LIZI
{
public:
    float life;//active time for a spot
    float dying;//kill the spot
    float r,g,b;//color setting
    float x,y,z;//position
    float xi,yi,zi;//shift
    float xg,yg,zg;//gravity
};

class Widget : public QOpenGLWidget,protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void initializeGL();
    void paintGL();//paint it
    void resizeGL(int w,int h);

    void loadGLTextures(GLuint *temp,const char *filename);//texture
    void keyPressEvent(QKeyEvent *event);//keyboard event control
    void mouseMoveEvent(QMouseEvent *event);


    GLdouble lookX,lookY,lookZ;//camera position
    GLdouble Dx,Dy,Dz;//the center position
    GLdouble lookangle;//distance
    GLuint picturewater,picturevolcano,picturesnow,picturshan,pictureshan2,picturefire,picturemuto;//texture resource

    int Num;//array arrange
    float Mx,My;// default spot position
    float slowdown,Gx,Gy,Gz,temptime;// gravity, slow spots
    float xspeed,yspeed;//x speed, y speed
    float R,G,B;// general color
    bool play,volcano,water,snow,through,waterfall,fire;//choose which one to paint
    GLdouble size;//spot's size
    LIZI lizi[MAXLIZI];//the spots are stored here

    void liziwater();//draw spurt
    void lizivolcano();//draw volcano
    void lizisnow();//draw snow
    void lizigothrough();//see the universe
    void liziwaterfall();//draw waterfall
    void lizifire();//draw the fire
    void drawmuto();
    void drawshan();//draw mountain type 1
    void drawshan2();//draw mountain type 2
    void drawshan3();

private slots: 
    void on_Spray_clicked();

    void on_Snow_clicked();

    void on_Volcano_clicked();

    void on_Universe_clicked();

    void on_Waterfall_clicked();

    void on_Fire_clicked();

    void on_Add_time_clicked();

    void on_Reduce_time_clicked();

    void on_Init_position_clicked();

    void on_Color_clicked();

    void on_Help_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
