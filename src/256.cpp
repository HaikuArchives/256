#include <Alert.h>
#include <Application.h>
#include <Bitmap.h>
#include <String.h>
#include <View.h>
#include <Window.h>

// Application view class
class AppViewClass:public BView
{
 public:
  AppViewClass();
  virtual void AttachedToWindow(void);
  virtual void Draw(BRect r);
  virtual void MouseDown(BPoint p);
 private:
  BBitmap *bmp;
  BString *info[256];
};

AppViewClass::AppViewClass():BView(BRect(0,0,319,319),
                                   "View256",
                                   B_FOLLOW_LEFT,
                                   B_WILL_DRAW)
{
 char st[10]="  0, 0x00";
 // Making text strings for every number from 0 to 255.
 for(int i=0;i<256;i++)
 {
  info[i]=new BString(st);

  // Making text for hexadecimal numbers.
  if(st[8]=='9') st[8]='A';
  else if(st[8]=='F')
  {
   st[8]='0';
   if(st[7]=='9') st[7]='A';
   else st[7]++;
  }
  else st[8]++;
  
  // Making text for decimal numbers.
  if(st[2]=='9')
  {
   st[2]='0';
   if(st[1]==' ') st[1]='1';
   else if(st[1]=='9')
   {
    st[1]='0';
    if(st[0]==' ') st[0]='1'; else st[0]++;
   }
   else st[1]++;
  }
  else st[2]++;
  
 }
 return;
}

void AppViewClass::AttachedToWindow()
{
 // Making bitmap 320x320, every square 20x20.
 char *bmpdata=new char[102400];
 int c=0;
 for(int i=0;i<256;i+=16)
 {
  for(int j=0;j<20;j++)
  {
   int k=i;
   for(int l=0;l<16;l++)
   {
    for(int m=0;m<20;m++) bmpdata[c++]=k;
    k++;
   }  
  }
 }
 bmp=new BBitmap(BRect(0,0,319,319),B_CMAP8);
 bmp->SetBits(bmpdata, 102400, 0, B_CMAP8);
 Draw(BRect(0,0,10,10));
 delete bmpdata;
 return;
};

void AppViewClass::Draw(BRect r)
{
 SetViewBitmap(bmp);
 return;
}


void AppViewClass::MouseDown(BPoint p)
{
 uint32 b;
 BPoint p1;
 GetMouse(&p1, &b);
 if(b==B_PRIMARY_MOUSE_BUTTON)
 {
  // Mouse left button pressed. Show window with color.
  int c=(int)(p.x/20)+(int)(p.y/20)*16;
  (new BAlert("",info[c]->String(),"OK"))->Go();
 }
 else if(b==B_SECONDARY_MOUSE_BUTTON)
  // Mouse right button pressed. Show info about program.
  (new BAlert("","256 v. 1.0 by Al.V. Sarikov.\nKherson, Ukraine, 2002.\nE-mail: avix@ukrpost.net.\nHome page: http://avix.pp.ru.","OK"))->Go();
}


// Application window class
class AppWindowClass:public BWindow
{
 public:
  AppWindowClass();
  virtual bool QuitRequested(void);
 private:
  AppViewClass *AppView;
};

AppWindowClass::AppWindowClass():BWindow(BRect(51,51,370,370),
                                         "256",
                                         B_TITLED_WINDOW,
                                         B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
 AppView=new AppViewClass;
 AddChild(AppView);
}

bool AppWindowClass::QuitRequested(void)
{
 be_app->PostMessage(B_QUIT_REQUESTED);
 return true;
}

// Application class
class AppClass:public BApplication
{
 public:
  AppClass();
  virtual bool QuitRequested(void);
 private:
  AppWindowClass *AppWindow;
};

AppClass::AppClass():BApplication("application/x-vnd.256")
{
 AppWindow=new AppWindowClass();
 AppWindow->Show();
}

bool AppClass::QuitRequested(void)
{
 return true;
}

int main(void)
{
 AppClass App;
 App.Run();
 return 1;
}