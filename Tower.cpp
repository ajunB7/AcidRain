/*
Commands to compile and run:

    g++ -o Tower Tower.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./Tower

Note: the -L option and -lstdc++ may not be needed on some machines.

*/


#include <cstdlib>
#include <iostream>
#include <list>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <deque>
#include <cmath>
#include <string>
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

const int Border = 5;
const int BufferSize = 10;
const int FPS = 40;
int gameSpeed = 1;
int maxRain = 1;
int level = 1;
int maxBackGroundRain = 100;
float bucketLine = 500;
int score = 0;
bool startGame = true;
bool finishGame = false;
bool collision = false;
bool cheatMode = false;
long textTime = 0;
XFontStruct * fontName;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC gc[6];
	Colormap colorMap;
	
	Pixmap	pixmap;		// double buffer
	int		width;		// size of pixmap
	int		height;

};


class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};



class Cloud : public Displayable {
	public:
		virtual void paint(XInfo &xInfo){
			// diameter = flatness vertial and then horizontal
			// XPoint p[3];
			// p[0].x = x+diameter; p[0].y = y+diameter/2.5;
			// p[1].x = x; p[1].y = y+diameter/2.5;
			// p[2].x = (p[0].x+p[1].x)/2; p[2].y = y-diameter;

			XColor color, color2;

   			// Shadow
			XAllocNamedColor(xInfo.display,xInfo.colorMap, "DarkGrey", &color, &color2);
   			XSetForeground(xInfo.display,xInfo.gc[2],color.pixel);
			// Outer Circles Shadows
			int xShadow = x+sqrt(diameter);
			int yShadow = y+sqrt(diameter);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow, yShadow, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter/1.5, yShadow+diameter/1.5, diameter/2, diameter/2, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter, yShadow+diameter/2, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter*1.75, yShadow+diameter/1.5, diameter/2, diameter/2, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter*2, yShadow+diameter/2, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter*2.5, yShadow+diameter/2.75, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter*3, yShadow+diameter/4, diameter/1.5, diameter/1.5, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter*2.75, yShadow+diameter/10, diameter/1.5, diameter/1.5, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter*2.25, yShadow-diameter/2.75, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter*2, yShadow-diameter/2.75, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter*1.5, yShadow-diameter/2.60, diameter/1.5, diameter/1.5, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter, yShadow-diameter/2.75, diameter/1.5, diameter/1.5, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], xShadow+diameter/1.5, yShadow-diameter/2.75, diameter, diameter
				, 0, 360*64);

			XAllocNamedColor(xInfo.display,xInfo.colorMap, "Grey", &color, &color2);
   			XSetForeground(xInfo.display,xInfo.gc[2],color.pixel);
			// Normal
   			// Main Inner Circle
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x, y, diameter*3, diameter, 0, 360*64);
			
			
			// Outer Circles
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x, y, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter/1.5, y+diameter/1.5, diameter/2, diameter/2, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter, y+diameter/2, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter*1.75, y+diameter/1.5, diameter/2, diameter/2, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter*2, y+diameter/2, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter*2.5, y+diameter/2.75, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter*3, y+diameter/4, diameter/1.5, diameter/1.5, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter*2.75, y+diameter/10, diameter/1.5, diameter/1.5, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter*2.25, y-diameter/2.75, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter*2, y-diameter/2.75, diameter, diameter, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter*1.5, y-diameter/2.60, diameter/1.5, diameter/1.5, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter, y-diameter/2.75, diameter/1.5, diameter/1.5, 0, 360*64);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x+diameter/1.5, y-diameter/2.75, diameter, diameter
				, 0, 360*64);
		}

		void move(XInfo &xInfo, long now){
			if (x-speed < 0){
				moveRight = true;
			}else if (x + diameter*3 + speed > xInfo.width){
				moveRight = false;
			} else if (now - lastSwitch > 2000000){
				int randomInt = rand() % 4 + 1;
				if (randomInt == 1){
					if (moveRight){
						moveRight = false;
					} else{
						moveRight = true;
					}
				}
				lastSwitch = now;
			}

			if (moveRight){
				x+= speed;
			}else {
				x -= speed;
			}
		}

		void setSpeed(int newSpeed){
			speed = newSpeed;
		}

		int getX(){
			return x+diameter*3/2;
		}

		int getY(){
			return y+diameter/2;
		}

		void resizeCloud(double windowWidth){
			diameter = windowWidth/30+30;
			if (x > windowWidth){
				x = windowWidth;
			}
		}


		Cloud(int x, double diameter, int speed, long lastSwitch): x(x), diameter(diameter), speed(speed), lastSwitch(lastSwitch) {
			y = 0;
			moveRight = true;
		}


	private:
		int x,y, speed;
		double diameter;
		bool moveRight;
		long lastSwitch;
};

class Crop : public Displayable{
	public:
		virtual void paint(XInfo &xInfo){
			// x = width of 1 block
			x = xInfo.width/4.00 * cropNum;
			xStart = xInfo.width/4.00 * (cropNum -1);
			XColor color, color2;


   			
			// XDrawLine(xInfo.display, xInfo.pixmap, xInfo.gc[3], 0, bucketLine, xInfo.width, bucketLine);

   			for (double i = 1.00; i <= cropLeft; i++)
   			{	

   				// diff = height of 1 block
   				double diff = (xInfo.height- bucketLine)/4.00;
   				double yLine = xInfo.height - diff*i;

   				XAllocNamedColor(xInfo.display,xInfo.colorMap, "Black", &color, &color2);

   				XSetForeground(xInfo.display,xInfo.gc[3],color.pixel);
   				XDrawRectangle(xInfo.display, xInfo.pixmap, xInfo.gc[3], xStart, yLine, xInfo.width/4.00, diff);


   				XAllocNamedColor(xInfo.display,xInfo.colorMap, "Green", &color, &color2);
   				XSetForeground(xInfo.display,xInfo.gc[3],color.pixel);
   				if (i == cropLeft){
   					XFillRectangle(xInfo.display, xInfo.pixmap, xInfo.gc[3], xStart, yLine, xInfo.width/4.00, 5);
   					maxCrop = yLine;
   				}
   			}
		}
		int getMax(){
			return maxCrop;
		}

		void destroyCrop(){
			cropLeft--;
		}

		bool allCropsDestroyed(){
			return cropLeft == 0;
		}

		int getXStart(){
			return xStart;
		}

		int getX(){
			return x;
		}
		Crop(int cropNum): cropNum(cropNum){
			x = 0;
			y = bucketLine;
			maxCrop = bucketLine;
			cropLeft = 4.00;
		}


	private:
		int x,y, cropNum, maxCrop, cropLeft, xStart;
};


class RainDrop : public Displayable{
	public:
		virtual void paint(XInfo &xInfo){
			// diameter = flatness vertial and then horizontal
			XPoint p[3];
			p[0].x = x+diameter; p[0].y = y+diameter/2.5;
			p[1].x = x; p[1].y = y+diameter/2.5;
			p[2].x = (p[0].x+p[1].x)/2; p[2].y = y-diameter;

			XColor color, color2;

			XAllocNamedColor(xInfo.display,xInfo.colorMap, "Orange", &color, &color2);
   			XSetForeground(xInfo.display,xInfo.gc[2],color.pixel);

			XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[2], p, 3, Complex, CoordModeOrigin);
			XFillArc(xInfo.display, xInfo.pixmap, xInfo.gc[2], x, y, diameter, diameter, 0, 360*64);
		}

		void drop (XInfo &xinfo){
				y+= speed;
		}

		void setCheatSpeed(int newSpeed){
			oldSpeed = speed;
			speed = newSpeed;
			slowMo = true;
		}
		void resetCheatSpeed(){
			speed = oldSpeed;
			slowMo = false;
		}

		bool usingCheatMode(){
			return slowMo;
		}

		int getX(){
			return x;
		}
		int getY(){
			return y;
		}
		int getDiameter(){
			return diameter;
		}


		RainDrop(int x, int y, int diameter, int speed): x(x), y(y), diameter(diameter), speed(speed) {
			slowMo = false;
		}


	private:
		int x,y,diameter, speed, oldSpeed;
		bool slowMo;
};


class BackGroundRainDrop : public Displayable{
	public:
		virtual void paint(XInfo &xInfo){
			int diameter = 5;
			XPoint p[3];
			p[0].x = x+diameter; p[0].y = y+diameter/2.5;
			p[1].x = x; p[1].y = y+diameter/2.5;
			p[2].x = (p[0].x+p[1].x)/2; p[2].y = y-diameter;

			XColor color, color2;

			XAllocNamedColor(xInfo.display,xInfo.colorMap, "SkyBlue", &color, &color2);
			if (cheatMode){
				XAllocNamedColor(xInfo.display,xInfo.colorMap, "Red", &color, &color2);
			}
   			XSetForeground(xInfo.display,xInfo.gc[4],color.pixel);

			XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[4], p, 3, Complex, CoordModeOrigin);
		}

		bool drop (XInfo &xinfo){
			if ((y + speed) > bucketLine) {
				y = bucketLine;
				return true;
			} else{
				y+= speed;
				x+= sqrt(speed);
				return false;
			}
		}

		void setSpeed(int newSpeed){
			speed = newSpeed;
		}

		BackGroundRainDrop(int x, int speed): x(x), speed(speed) {
			y = 0;
		}


	private:
		int x,y, speed;
};


class Lightning : public Displayable{
	public:
		virtual void paint(XInfo &xInfo){
			XColor color, color2;
			XAllocNamedColor(xInfo.display,xInfo.colorMap, "white", &color, &color2);
   			XSetForeground(xInfo.display,xInfo.gc[4],color.pixel);
   			XFillRectangle(xInfo.display, xInfo.pixmap, xInfo.gc[4], 0, 0, xInfo.width, bucketLine);

   			int diameter = 5;
			XPoint p[6];
			p[0].x = x; p[0].y = y;
			p[1].x = x-50; p[1].y = y+50;
			p[2].x = x+50; p[2].y = y+50;
			p[3].x = x-100; p[3].y = y+200;
			p[4].x = x+200; p[4].y = y+30;
			p[5].x = x+100; p[5].y = y;


			XAllocNamedColor(xInfo.display,xInfo.colorMap, "Yellow", &color, &color2);
   			XSetForeground(xInfo.display,xInfo.gc[4],color.pixel);

			XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[4], p, 6, Complex, CoordModeOrigin);

		}

		bool strike(long timeNow){
			if(status){

					// how long it will flash for
				if ((timeNow - lastLightning) >= 150000){
					lastLightning = timeNow;
					status = false;
					return status;
				}
				return true;

			} 
			// How often to flash
			if ((timeNow - lastLightning) >= 7000000){
				lastLightning = timeNow;
				status = true;
				return status;
			}else {
				status = false;
				return status;
			}
		}

		void setXY(int xPoint, int yPoint){
			x = xPoint;
			y = yPoint;
		}

		Lightning(long lastLightning): lastLightning(lastLightning){
		}


	private:
		long lastLightning;
		bool status;
		int x,y;
};

class Bucket : public Displayable {
	public: 
		virtual void paint(XInfo &xInfo){
			if (!dumping){
				y = bucketLine;

				XPoint p[4];
				p[0].x = x-diameter; p[0].y = y;
				p[1].x = x+diameter; p[1].y = y;
				p[2].x = x+50; p[2].y = y-70;
				p[3].x = x-50; p[3].y = y-70;

				XColor color, color2;
				XAllocNamedColor(xInfo.display,xInfo.colorMap, "Brown", &color, &color2);
	   			XSetForeground(xInfo.display,xInfo.gc[1],color2.pixel);

	   			int arc = 75;
				XDrawArc(xInfo.display, xInfo.pixmap, xInfo.gc[1], x-37, y-105, arc, arc, 0, 180*64);
				XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[1], p, 4, Complex, CoordModeOrigin);
				// XDrawRectangle(xInfo.display, xInfo.pixmap, xInfo.gc[1], x, y, 100, 75);

				if (filled > 0){
					int max = 68;
						float percentFilled = filled/max;
						int xPercent = percentFilled*20;
						
						XPoint p[4];
						p[0].x = x-28; p[0].y = y-5;
						p[1].x = x+28; p[1].y = y-5;
						p[2].x = p[1].x+xPercent; p[2].y = y-filled;
						p[3].x = p[0].x-xPercent; p[3].y = y-filled;

						XColor color, color2;
						XAllocNamedColor(xInfo.display,xInfo.colorMap, "Yellow", &color, &color2);
			   			XSetForeground(xInfo.display,xInfo.gc[1],color2.pixel);

						XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[1], p, 4, Complex, CoordModeOrigin);
						// XDrawRectangle(xInfo.display, xInfo.window, xInfo.gc, x, y, 100, 75);
					}

				if ((x+ moveDir < xInfo.width) && x + moveDir > 0){
					x+= moveDir;
				}
			}else{
				XPoint p[4];
				if (!dumped){
					if (xInfo.width - xBeforeDump < xInfo.width/2){
						x+= 30;
					} else{
						x-= 30;
					}
					y-= 10;


					p[0].x = x-diameter; p[0].y = y;
					p[1].x = x+diameter; p[1].y = y;
					p[2].x = x+50; p[2].y = y-70;
					p[3].x = x-50; p[3].y = y-70;

					XColor color, color2;
					XAllocNamedColor(xInfo.display,xInfo.colorMap, "Brown", &color, &color2);
		   			XSetForeground(xInfo.display,xInfo.gc[1],color2.pixel);

		   			int arc = 75;
					XDrawArc(xInfo.display, xInfo.pixmap, xInfo.gc[1], x-37, y-105, arc, arc, 0, 180*64);
					XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[1], p, 4, Complex, CoordModeOrigin);

					if (filled > 0){
						int max = 68;
							float percentFilled = filled/max;
							int xPercent = percentFilled*20;
							
							XPoint p[4];
							p[0].x = x-28; p[0].y = y-5;
							p[1].x = x+28; p[1].y = y-5;
							p[2].x = p[1].x+xPercent; p[2].y = y-filled;
							p[3].x = p[0].x-xPercent; p[3].y = y-filled;

							XColor color, color2;
							XAllocNamedColor(xInfo.display,xInfo.colorMap, "Yellow", &color, &color2);
				   			XSetForeground(xInfo.display,xInfo.gc[1],color2.pixel);

							XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[1], p, 4, Complex, CoordModeOrigin);
							// XDrawRectangle(xInfo.display, xInfo.window, xInfo.gc, x, y, 100, 75);
						}
					if (x >= xInfo.width || x <= 0 || y <= 0){
						dumped = true;
						filled = 0;
					}
				} else{
					y+=10;
					if (xInfo.width - xBeforeDump < xInfo.width/2){
						x-= 30;
						if (x <= xBeforeDump && y >= yBeforeDump){
							dumping = false;
							dumped = false;
						}
					} else{
						x+= 30;
						if (x >= xBeforeDump && y >= yBeforeDump){
							dumping = false;
							dumped = false;
						}
					}
					p[0].x = x-diameter; p[0].y = y;
					p[1].x = x+diameter; p[1].y = y;
					p[2].x = x+50; p[2].y = y-70;
					p[3].x = x-50; p[3].y = y-70;

					XColor color, color2;
					XAllocNamedColor(xInfo.display,xInfo.colorMap, "Brown", &color, &color2);
		   			XSetForeground(xInfo.display,xInfo.gc[1],color2.pixel);

		   			int diameter = 75;
					XDrawArc(xInfo.display, xInfo.pixmap, xInfo.gc[1], x-37, y-105, diameter, diameter, 0, 180*64);
					XFillPolygon(xInfo.display, xInfo.pixmap, xInfo.gc[1], p, 4, Complex, CoordModeOrigin);

				}


				// XDrawRectangle(xInfo.display, xInfo.pixmap, xInfo.gc[1], x, y, 100, 75);

			}

		}

		void addWater(float amountPercent){
				int max = 68;
				float amountDecimal = amountPercent/100.00;
				int amount =  amountDecimal * max;
				float percentFilled = filled/max;
				if ((filled+amount) <= max){
					filled += amount;
				} else if ((filled + amount) > max){
					filled = max;
				}

		}

		void move(bool right){
			if (right){
				moveDir = 10;
			}else {
				moveDir = -10;
			}
		}

		void dump(){
			xBeforeDump = x;
			yBeforeDump = y;
			dumping = true;
		}
		void stop(){
			moveDir = 0;
		}
		int getX(){
			return x;
		}

		int getY(){
			return y;
		}

		int getDiameter(){
			return 50;
		}

		int getHeight(){
			return 70;
		}

		bool full(){
			return filled >= 68;
		}

		void setX(int newX){
			x = newX;
		}

		Bucket(int x, int y): x(x), y(y){
			filled = 0;
			diameter = 30;
			dumping = false;
			dumped = false;
		}
	private:
		int y;
		int x;
		float filled;
		int moveDir;
		int diameter;
		int xBeforeDump, yBeforeDump, rotate;
		bool dumping, dumped;
};

class Word : public Displayable{
	public:
		virtual void paint(XInfo &xInfo){
			
			XColor color, color2;

			XAllocNamedColor(xInfo.display,xInfo.colorMap, "White", &color, &color2);
		   	XSetForeground(xInfo.display,xInfo.gc[5],color2.pixel);
			XDrawString(xInfo.display, xInfo.pixmap, xInfo.gc[5], x,y, words.c_str(), words.length());

		}
			

		Word(int x, int y, string words): x(x), y(y), words(words) {
		}


	private:
		int x,y;
		string words;
};

/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
    cerr << str << endl;
    exit(0);
}
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void toggleCheatMode(){
	if (!cheatMode){
		cheatMode = true;
		cout << "CheatMode ON!" << endl;
	}else{
		cheatMode = false;
		cout << "CheatMode OFF!" << endl;
	}
}

list<Displayable *> paintList;
deque<RainDrop *> rainList;
deque<BackGroundRainDrop *> backGroundRainList;
Bucket bucket(bucketLine, bucketLine);
Cloud cloud(300, 60, gameSpeed, now());
Lightning lightning(now());
Crop crop1(1);
Crop crop2(2);
Crop crop3(3);
Crop crop4(4);
/*
 * Create a window
 */
void initX(int argc, char* argv[], XInfo& xInfo) {


    /*
    * Display opening uses the DISPLAY	environment variable.
    * It can go wrong if DISPLAY isn't set, or you don't have permission.
    */
    xInfo.display = XOpenDisplay( "" );
    if ( !xInfo.display )	{
        error( "Can't open display." );
    }

    /*
    * Find out some things about the display you're using.
    */
    xInfo.screen = DefaultScreen( xInfo.display ); // macro to get default screen index

    unsigned long white, black;
    white = XWhitePixel( xInfo.display, xInfo.screen ); 
    black = XBlackPixel( xInfo.display, xInfo.screen );
    
    XSizeHints hSize;
	hSize.x = 100;
	hSize.y = 100;
	hSize.width = 1000;
	hSize.height = 600;
	hSize.flags = PPosition | PSize;

    xInfo.window = XCreateSimpleWindow(
                       xInfo.display,				// display where window appears
                       DefaultRootWindow( xInfo.display ), // window's parent in window tree
					   hSize.x, hSize.y,
					   hSize.width, hSize.height,	 // size of the window
                       5,						     // width of window's border
                       black,						// window border colour
                       black );					    // window background colour

    // extra window properties like a window title
    XSetStandardProperties(
        xInfo.display,		// display containing the window
        xInfo.window,		// window whose properties are set
        "Acid|Rain",	// window's title
        "AR",				// icon's title
        None,				// pixmap for the icon
        argv, argc,			// applications command line args
        &hSize );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */

	for (int i = 0; i < 6; ++i)
	{
			xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
			XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
			XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
			XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
			XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);
	}


    // Valid Inputs
   	XSelectInput(xInfo.display, xInfo.window, 
		ButtonPressMask | KeyPressMask
		| StructureNotifyMask
		| KeyReleaseMask // for resize events
		);

	int depth = DefaultDepth(xInfo.display, DefaultScreen(xInfo.display));
	xInfo.pixmap = XCreatePixmap(xInfo.display, xInfo.window, hSize.width, hSize.height, depth);
	xInfo.width = hSize.width;
	xInfo.height = hSize.height;

	/*
	 * Don't paint the background -- reduce flickering
	 */
	XSetWindowBackgroundPixmap(xInfo.display, xInfo.window, None);

	xInfo.colorMap =  DefaultColormap(xInfo.display,xInfo.screen);

	fontName = XLoadQueryFont(xInfo.display, "9x15bold");
	// fontName = XLoadQueryFont(xInfo.display, "-*-helvetica-bold-r-*-*-17-*");
	if (!fontName){
		cout << "FONT Load FAILED! - Loading Default" << endl;
		exit(-1);
	}

	XSetFont (xInfo.display, xInfo.gc[5], fontName->fid);
    /*
     * Put the window on the screen.
     */
    XMapRaised( xInfo.display, xInfo.window );

    XFlush(xInfo.display);
    sleep(2);	// let server get set up before sending drawing commands
}

void handleKeyPress(XInfo &xinfo, XEvent &event, bool pressed) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)

	if ( i == 1) {
		if (text[0] == 'q' && pressed) {
			error("Terminating normally.");
		} else if (key == XK_Return && startGame && pressed) {
			startGame = false;
		} else if (text[0] == 'd' && pressed){
			bucket.move(true);
		} else if (text[0] == 'a' && pressed) {
			bucket.move(false);
	// Handle Key Release
		} else if ((text[0] == 'd' || text[0] == 'a') && !pressed){
			bucket.stop();
		} else if (text[0] == 'c' && pressed){
			toggleCheatMode();
		}
	}
}

void handleResize(XInfo &xinfo, XEvent &event) {
	XConfigureEvent xce = event.xconfigure;
	fprintf(stderr, "Handling resize  w=%d  h=%d\n", xce.width, xce.height);


	// Force Max Width of 700w X 500h
	if (xce.width < 700){
		XResizeWindow(xinfo.display, xinfo.window, 700, xce.height);
		xce.width = 700;
	}
	if (xce.height < 500){
		XResizeWindow(xinfo.display, xinfo.window, xce.width, 500);
		xce.height = 500;
	}

	XFreePixmap(xinfo.display, xinfo.pixmap);
	int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
	xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, xce.width, xce.height, depth);
	xinfo.width = xce.width;
	xinfo.height = xce.height;

	cloud.resizeCloud((double)xce.width);
	bucketLine = (float)xce.height*5.00/6.00;
	if(bucket.getX() > xinfo.width){
		bucket.setX(xinfo.width);
	}


	// Make Max 700w by 500h

}


/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo, bool startGame, bool finishGame) {

	list<Displayable *>::const_iterator begin = paintList.begin();
	list<Displayable *>::const_iterator end = paintList.end();

	// draw into the buffer
	// note that a window and a pixmap are “drawables”
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height);


	if (!startGame && !finishGame){

		for (int i = 0; i < backGroundRainList.size(); i++)
		{
			BackGroundRainDrop *d = backGroundRainList[i];
			d->paint(xinfo); // the displayables know about the pixmap
		}

		while( begin != end ) {
			Displayable *d = *begin;
			d->paint(xinfo); // the displayables know about the pixmap
			
			begin++;
		}

		for (int i = 0; i < rainList.size(); i++)
		{
			RainDrop *d = rainList[i];
			d->paint(xinfo); // the displayables know about the pixmap
		}

		if (lightning.strike(now())){
			lightning.setXY(cloud.getX(), cloud.getY());
			lightning.paint(xinfo);
		}

		string levelLine = "Level: " + to_string(level);
		Word levelWord(15,25, levelLine);
		levelWord.paint(xinfo);
		string scoreLine = "Score: " + to_string(score);
		Word word1(15,45, scoreLine);
		word1.paint(xinfo);
		
		if ((score % 5 == 0) && collision){
	 			level++;
	 			textTime = now();
	 			if (maxRain < 2 && !cheatMode){
	 				maxRain++;
	 			}
	 			if (!cheatMode){
	 				gameSpeed++;
	 			}
	 	}

	 	if(now() - textTime <= 1000000){
	 			 string levelString = "Level " + to_string(level);
				int stringSizeLevel = XTextWidth(fontName, levelString.c_str(), levelString.size());
				Word wordLevel((xinfo.width - stringSizeLevel)/2,xinfo.height/2+15, levelString);
				wordLevel.paint(xinfo);
	 	}

		if (bucket.full())
		{
			string bucketString2 = "Bucket Full! - Click anywhere to empty Bucket";
			int stringSize = XTextWidth(fontName, bucketString2.c_str(), bucketString2.size());
			Word word2((xinfo.width- stringSize)/2,xinfo.height/2-5, bucketString2);
			word2.paint(xinfo);
		}
	} else if (startGame) {

					XColor color, color2;
			XAllocNamedColor(xinfo.display,xinfo.colorMap, "Red", &color, &color2);
			XSetForeground(xinfo.display,xinfo.gc[1],color2.pixel);
			XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[1], 0, xinfo.height/2.5-25, xinfo.width, xinfo.height/2.5-25);


			string string1 = "Welcome To AcidRain";
			int stringSize = XTextWidth(fontName, string1.c_str(), string1.size());
			Word word1((xinfo.width-stringSize)/2,xinfo.height/2.5, string1);
			word1.paint(xinfo);
			string string2 = "The Objective of this game is to protect your crops.";
			stringSize = XTextWidth(fontName, string2.c_str(), string2.size());
			Word word2((xinfo.width-stringSize)/2,xinfo.height/2.5+20, string2);
			word2.paint(xinfo);
			string string3 = "Once any of your crops is destroyed, you Lose.";
			stringSize = XTextWidth(fontName, string3.c_str(), string3.size());
			Word word3((xinfo.width-stringSize)/2,xinfo.height/2.5+40, string3);
			word3.paint(xinfo);
			string string4 = "Move Bucket and protect crops using \'A\' for Left and \'D\' for Right";
			stringSize = XTextWidth(fontName, string4.c_str(), string4.size());
			Word word4((xinfo.width-stringSize)/2,xinfo.height/2.5+60, string4);
			word4.paint(xinfo);
			string string5 = "When the bucket is full, Click anywhere on the screen to empty the bucket";
			stringSize = XTextWidth(fontName, string5.c_str(), string5.size());
			Word word5((xinfo.width-stringSize)/2,xinfo.height/2.5+80, string5);
			word5.paint(xinfo);
			string string6 = "Press Enter to Start Game or Q to quit.";
			stringSize = XTextWidth(fontName, string6.c_str(), string6.size());
			Word word6((xinfo.width-stringSize)/2,xinfo.height/2.5+100, string6);
			word6.paint(xinfo);
			string string7 = "Press C to enter CheatMode";
			stringSize = XTextWidth(fontName, string7.c_str(), string7.size());
			Word word7((xinfo.width-stringSize)/2,xinfo.height/2.5+120, string7);
			word7.paint(xinfo);

			XDrawLine(xinfo.display, xinfo.pixmap, xinfo.gc[1], 0, xinfo.height/2.5+120+15, xinfo.width, xinfo.height/2.5+120+15);

			string string8 = "Ajunpreet Bambrah";
			stringSize = XTextWidth(fontName, string8.c_str(), string8.size());
			Word word8((xinfo.width-stringSize)/2,xinfo.height/1.50, string8);
			word8.paint(xinfo);

			string string9 = "abambrah";
			stringSize = XTextWidth(fontName, string9.c_str(), string9.size());
			Word word9((xinfo.width-stringSize)/2,xinfo.height/1.50+20, string9);
			word9.paint(xinfo);
	} else if (finishGame) {
			string string1 = "Game Over!";
			int stringSize = XTextWidth(fontName, string1.c_str(), string1.size());
			Word word1((xinfo.width-stringSize)/2,xinfo.height/2.5, string1);
			word1.paint(xinfo);
			string string2 = "Your Score was " + to_string(score);
			stringSize = XTextWidth(fontName, string2.c_str(), string2.size());
			Word word2((xinfo.width-stringSize)/2,xinfo.height/2.5+20, string2);
			word2.paint(xinfo);
			string string3 = "You Reached Level " + to_string(level);
			stringSize = XTextWidth(fontName, string3.c_str(), string3.size());
			Word word3((xinfo.width-stringSize)/2,xinfo.height/2.5+40, string3);
			word3.paint(xinfo);
			string string4 = "Press Q to quit";
			stringSize = XTextWidth(fontName, string4.c_str(), string4.size());
			Word word4((xinfo.width-stringSize)/2,xinfo.height/2.5+60, string4);
			word4.paint(xinfo);
	}
	// copy buffer to window
	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
		0, 0); // position to put top left corner of pixmap in window

	XFlush( xinfo.display );

}

void createRain(XInfo &xinfo){
	if (rainList.size() >= maxRain){
		return;
	}
		int x = cloud.getX();
		int diameter = rand() % 10 + 15;
		int speed = rand() % gameSpeed + 5;
		RainDrop * tmpRain = new RainDrop(x,cloud.getY(),diameter, speed);
		rainList.push_front(tmpRain);
}

void createBackGroundRain(XInfo &xinfo){
	if (backGroundRainList.size() >= maxBackGroundRain){
		return;
	}

		int x = rand() % xinfo.width;
		int speed = rand() % gameSpeed + 5;
		BackGroundRainDrop * tmpRain = new BackGroundRainDrop(x, speed);
		backGroundRainList.push_front(tmpRain);
}


void makeItRain(XInfo &xinfo) {
	cloud.move(xinfo, now());
	// Create rain as needed
	createRain(xinfo);
	createBackGroundRain(xinfo);
	for (int i = 0; i < rainList.size(); i++)
	{
		RainDrop *d = rainList[i];

		if (cheatMode && !d->usingCheatMode()){
			d->setCheatSpeed(1);
		}else if (!cheatMode && d->usingCheatMode()){
			d->resetCheatSpeed();
		}

		d->drop(xinfo); //Move RainDrop down

		bool cTest1,cTest2, cTest3,cTest4;
		// Check if the rain drop is within the x of the bucket
		cTest1 = d->getX()+d->getDiameter() < (bucket.getX() + bucket.getDiameter());
		cTest2 = d->getX()+d->getDiameter() > (bucket.getX() - bucket.getDiameter());
		//Check if the rain drop is touching the top of the bucket
		cTest3 = (abs(d->getY() - bucket.getY() + d->getDiameter() + 15) * 2 < (d->getDiameter() + bucket.getHeight()));
		cTest4 = (d->getY()-d->getDiameter()) < (bucket.getY() - bucket.getHeight());

		collision = cTest1 && cTest2
	 	 				 && cTest3
	 	 				 && cTest4;
	 	if (collision && !bucket.full()){
	 		bucket.addWater(d->getDiameter());
	 		score++;
			rainList.erase(rainList.begin() + i);
			continue;
	 	}else{
	 		collision = false;
	 	}
	 	// Top Y Pos
	 	int c1Max = crop1.getMax();
	 	int c2Max = crop2.getMax();
	 	int c3Max = crop3.getMax();
	 	int c4Max = crop4.getMax();
	 	// xStarting Pos
	 	int c1XStart = crop1.getXStart();
	 	int c2XStart = crop2.getXStart();
	 	int c3XStart = crop3.getXStart();
	 	int c4XStart = crop4.getXStart();
	 	// xWidth Pos
	 	int c1X = crop1.getX();
	 	int c2X = crop2.getX();
	 	int c3X = crop3.getX();
	 	int c4X = crop4.getX();

	 	int rainX = d->getX();
	 	int rainY = d->getY();

	 	if (rainX > c1XStart && rainX < c1X && rainY >= c1Max){
			rainList.erase(rainList.begin() + i);
			crop1.destroyCrop();
			if (crop1.allCropsDestroyed()){
				finishGame = true;
			}
	 	} else if (rainX > c2XStart && rainX < c2X && rainY >= c2Max){
	 		rainList.erase(rainList.begin() + i);
			crop2.destroyCrop();
			if (crop2.allCropsDestroyed()){
				finishGame = true;
			}
	 	} else if (rainX > c3XStart && rainX < c3X && rainY >= c3Max){
	 		rainList.erase(rainList.begin() + i);
			crop3.destroyCrop();
			if (crop3.allCropsDestroyed()){
				finishGame = true;
			}
	 	} else if (rainX > c4XStart && rainX < c4X && rainY >= c4Max){
			rainList.erase(rainList.begin() + i);
			crop4.destroyCrop();
			if (crop4.allCropsDestroyed()){
				finishGame = true;
			}
	 	}
	 // 	cout << test1 << endl;
		// cout << test2 << endl;
		// cout << test3 << endl;
		// cout << test4 << endl;
		// cout << test5 << endl;
		// cout << "-----X---" << endl;
		// cout << d->getX() << endl;
		// cout << (bucket.getX() + bucket.getHeight()) << " - " << (bucket.getX() - bucket.getDiameter()) << endl;
		// cout << "-----Y---" << endl;
		// cout << d->getY() << endl;
		// cout << (bucket.getY() + bucket.getHeight()) << " - " << (bucket.getY() - bucket.getHeight()) << endl;
	}

	for (int i = 0; i < backGroundRainList.size(); i++)
	{
		BackGroundRainDrop *d = backGroundRainList[i];
		bool yMaxReached = d->drop(xinfo); //Move RainDrop down
		// If rain reaches the crop, delete it
		if (yMaxReached){
			backGroundRainList.erase(backGroundRainList.begin() + i);
		}
	}

}

void eventLoop(XInfo &xinfo) {

	// RainDrop rDrop(100,450,40);
	// paintList.push_front(&rDrop);
	paintList.push_front(&bucket);
	paintList.push_front(&cloud);
	paintList.push_front(&crop1);
	paintList.push_front(&crop2);
	paintList.push_front(&crop3);
	paintList.push_front(&crop4);

	XEvent event;
	unsigned long lastRepaint = 0;
	long refreshFPS = 1000000/FPS;
	while( true ) {
		/*
		 * This is NOT a performant event loop!  
		 * It needs help!
		 */
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			switch( event.type ) {
				case KeyPress:
					handleKeyPress(xinfo, event, true);
					break;
				case KeyRelease:
					handleKeyPress(xinfo, event, false);
					break;
				case ConfigureNotify:
					handleResize(xinfo, event);
					break;
				case ButtonPress:
					if (!startGame && !finishGame){
						bucket.dump();
					}
					break;
			}
		}else if (XPending(xinfo.display) == 0) {
			usleep(refreshFPS);
		}
		
		long finish = now();
		if (finish - lastRepaint < refreshFPS){
			continue;
		} else{
			lastRepaint = now();
		}

		if (!startGame && !finishGame){
			makeItRain(xinfo); //Make Rain Drop (move y)
		}
		repaint(xinfo, startGame, finishGame);
	}
}


/*
 *   Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char* argv[] ) {
    XInfo xInfo;

    initX(argc, argv, xInfo);
	// Bucket bucket(500);
	// bucket.paint(xInfo);

	// XColor color, color2;
	// XAllocNamedColor(xInfo.display,xInfo.colorMap, "Red", &color, &color2);
	// XSetForeground(xInfo.display,xInfo.gc[1],color2.pixel);
	// Word word(100,100, "Hello");
	// word.paint(xInfo);

	// // DrawLine has x1,y1, x2, y2 .. so start and end points
	// // XDrawLine(xInfo.display, xInfo.window, xInfo.gc[2], 0, bucketLine-100, xInfo.width, bucketLine-200);

	// // RainDrop rDrop(100,450,40);
 // //    rDrop.paint(xInfo);
 // //    Cloud cloud(300);
	// // cloud.paint(xInfo);
	// XFlush(xInfo.display);

	eventLoop(xInfo);
    // wait for user input to quit (a concole event for now)
    cout << "press ENTER to exit";
    cin.get();

    XCloseDisplay(xInfo.display);
}

