//OpenGL Coverflow API by J.Roth

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <QtOpenGL>
#include <QGLWidget>
#include <QMutex>

#include "pictureflow.h" //TODO mover los tipos de flow de sitio

class ImageLoaderGL;
class QGLContext;
class WidgetLoader;
class ImageLoaderByteArrayGL;

typedef enum Performance
{
	low=0,
	medium,
	high,
	ultraHigh
};

//Cover Vector
typedef struct RVect{
	float x;
	float y;
	float z;
	float rot;
};

//the cover info struct
typedef struct CFImage{
	GLuint img;
	char name[256];

	float width;
	float height;

	int index;

	RVect current;
	RVect animEnd;
};

struct Preset{
	/*** Animation Settings ***/
	//sets the speed of the animation
	float animationStep;
	//sets the acceleration of the animation
	float animationSpeedUp;
	//sets the maximum speed of the animation
	float animationStepMax;
	//sets the distance of view 
	float animationFadeOutDist;
	//sets the rotation increasion
	float preRotation;
	//sets the light strenght on rotation 
	float viewRotateLightStrenght;
	//sets the speed of the rotation 
	float viewRotateAdd;
	//sets the speed of reversing the rotation
	float viewRotateSub;
	//sets the maximum view angle
	float viewAngle;

	/*** Position Configuration ***/
	//the X Position of the Coverflow 
	float cfX;
	//the Y Position of the Coverflow 
	float cfY;
	//the Z Position of the Coverflow 
	float cfZ;
	//the X Rotation of the Coverflow 
	float cfRX;
	//the Y Rotation of the Coverflow 
	float cfRY;
	//the Z Rotation of the Coverflow 
	float cfRZ;
	//sets the rotation of each cover
	float rotation;
	//sets the distance between the covers
	float xDistance;
	//sets the distance between the centered and the non centered covers
	float centerDistance;
	//sets the pushback amount 
	float zDistance;
	//sets the elevation amount
	float yDistance;

	float zoom;
};

extern struct Preset defaultYACReaderFlowConfig;
extern struct Preset presetYACReaderFlowClassicConfig;
extern struct Preset presetYACReaderFlowStripeConfig;
extern struct Preset presetYACReaderFlowOverlappedStripeConfig;
extern struct Preset pressetYACReaderFlowUpConfig;
extern struct Preset pressetYACReaderFlowDownConfig;

class YACReaderFlowGL : public QGLWidget
{
	Q_OBJECT
protected:
	int timerId;
	/*** System variables ***/
	CFImage dummy;
	int viewRotateActive;
	float stepBackup;

	/*functions*/
	void calcPos(CFImage *CF,int pos);
	void calcRV(RVect *RV,int pos);
	void animate(RVect *Current,RVect to);
	void drawCover(CFImage *CF);
	
	int updateCount;
	WidgetLoader * loader;
	int fontSize;

	GLuint defaultTexture;
	GLuint markTexture;
    void initializeGL();
    void paintGL();
	void timerEvent(QTimerEvent *);

	//number of Covers
	int numObjects;
	int lazyPopulateObjects;
	bool showMarks;
	QVector<bool> loaded;
	QVector<bool> marks;
	QList<QString> paths;
	CFImage * cfImages;

	Performance performance;

	/*** Animation Settings ***/
	Preset config;

	//sets/returns the curent selected cover 
	int currentSelected;

	//defines the position of the centered cover 
	RVect centerPos;

	/*** Style ***/
	//sets the amount of shading of the covers in the back (0-1)
	float shadingTop;
	float shadingBottom;

	//sets the reflection strenght (0-1)
	float reflectionUp;
	float reflectionBottom;

	/*** System info ***/
	float viewRotate;
	
public:


	/*Constructor*/
	YACReaderFlowGL(QWidget *parent = 0,struct Preset p = pressetYACReaderFlowDownConfig);
    ~YACReaderFlowGL();

	//size;
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

	/*functions*/

	//if called it moves the coverflow to the left
	void showPrevious();
	//if called it moves the coverflow to the right
	void showNext();
	//go to
	void setCurrentIndex(int pos);
	//must be called whenever the coverflow animation is stopped
	void cleanupAnimation();
	//Draws the coverflow
	void draw();
	//updates the coverflow
	void updatePositions();
	//inserts a new item to the coverflow 
	//if item is set to a value > -1 it updates a already set value
	//otherwise a new entry is set
	void insert(char *name, GLuint Tex, float x, float y,int item = -1);
	//removes a item
	void remove(int item);
	//replaces the texture of the item 'item' with Tex
	void replace(char *name, GLuint Tex, float x, float y,int item);
	//create n covers with the default nu
	void populate(int n);
	/*Info*/
	//retuns the CFImage Struct of the current selected item
	//to read title or textures
	CFImage getCurrentSelected();

	public slots:
	void setCF_RX(int value);
	//the Y Rotation of the Coverflow 
	void setCF_RY(int value);
	//the Z Rotation of the Coverflow 
	void setCF_RZ(int value);

	//perspective
	void setZoom(int zoom);

	void setRotation(int angle);
	//sets the distance between the covers
	void setX_Distance(int distance);
	//sets the distance between the centered and the non centered covers
	void setCenter_Distance(int distance);
	//sets the pushback amount 
	void setZ_Distance(int distance);

	void setCF_Y(int value);
	void setCF_Z(int value);

	void setY_Distance(int value);

	void setFadeOutDist(int value);

	void setLightStrenght(int value);

	void setMaxAngle(int value);

	void setPreset(const Preset & p);

	void setPerformance(Performance performance);

	

	virtual void updateImageData() = 0;

	void reset();

	//interface with yacreaderlibrary, compatibility
	void setShowMarks(bool value);
	void setMarks(QVector<bool> marks);
	void setMarkImage(QImage & image);
	void markSlide(int index);
	void unmarkSlide(int index);
	void setSlideSize(QSize size);
	void clear();
	void setCenterIndex(int index);
	void showSlide(int index);
	int centerIndex();
	void updateMarks();
	//void setFlowType(PictureFlow::FlowType flowType);
	void render();

	//void paintEvent(QPaintEvent *event);
	void mouseDoubleClickEvent(QMouseEvent* event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent * event);
	void keyPressEvent(QKeyEvent *event);
	void resizeGL(int width, int height);
	friend class ImageLoaderGL;
	friend class ImageLoaderByteArrayGL;

signals:
	void centerIndexChanged(int);
	void selected(unsigned int);
};

//class WidgetLoader : public QGLWidget
//{
//	Q_OBJECT
//public:
//	WidgetLoader(QWidget *parent, QGLWidget * shared);
//	YACReaderFlowGL * flow;
//public slots:
//	void loadTexture(int index);
//
//};

class YACReaderComicFlowGL : public YACReaderFlowGL
{
public:
	YACReaderComicFlowGL(QWidget *parent = 0,struct Preset p = defaultYACReaderFlowConfig);
	void setImagePaths(QStringList paths);
	void updateImageData();
	friend class ImageLoaderGL;
private:
	ImageLoaderGL * worker;

};

class YACReaderPageFlowGL : public YACReaderFlowGL
{
public:
	YACReaderPageFlowGL(QWidget *parent = 0,struct Preset p = defaultYACReaderFlowConfig);
	void updateImageData();
	void populate(int n);
	QVector<bool> imagesReady;
	QVector<QByteArray> rawImages;
	QVector<bool> imagesSetted;
	friend class ImageLoaderByteArrayGL;
private:
	ImageLoaderByteArrayGL * worker;
};

class ImageLoaderGL : public QThread
{
public:
	ImageLoaderGL(YACReaderFlowGL * flow);
	~ImageLoaderGL();
	// returns FALSE if worker is still busy and can't take the task
	bool busy() const;
	void generate(int index, const QString& fileName);
	void reset(){idx = -1;fileName="";};
	int index() const { return idx; };
	QImage result();
	YACReaderFlowGL * flow;
	GLuint resultTexture;
	QImage loadImage(const QString& fileName);

protected:
	void run();

private:
	QMutex mutex;
	QWaitCondition condition;
	

	bool restart;
	bool working;
	int idx;
	QString fileName;
	QSize size;
	QImage img;
};

class ImageLoaderByteArrayGL : public QThread
{
public:
	ImageLoaderByteArrayGL(YACReaderFlowGL * flow);
	~ImageLoaderByteArrayGL();
	// returns FALSE if worker is still busy and can't take the task
	bool busy() const;
	void generate(int index, const QByteArray& raw);
	void reset(){idx = -1; rawData.clear();};
	int index() const { return idx; };
	QImage result();
	YACReaderFlowGL * flow;
	GLuint resultTexture;
	QImage loadImage(const QByteArray& rawData);

protected:
	void run();

private:
	QMutex mutex;
	QWaitCondition condition;


	bool restart;
	bool working;
	int idx;
	QByteArray rawData;
	QSize size;
	QImage img;
};

//class TextureLoader : public QThread
//{
//public:
//	TextureLoader();
//	~TextureLoader();
//	// returns FALSE if worker is still busy and can't take the task
//
//	YACReaderFlow * flow;
//	ImageLoader * worker;
//protected:
//	void run();
//
//};