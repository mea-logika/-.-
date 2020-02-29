#include "TXLib.h"
#include <cmath>
struct TPos
    {
    double x, y;
    };
struct TFon
    {
     HDC Image_;
     TPos Pos_;
     bool visible_;
     TPos Size_;
     bool Scroll_;

     void Init ( const char* fname);
     void Move ( int dx, int dy);
     void Show ();
     };
struct TAnim
    {
     HDC Image_;
     TPos Pos_, Leg_;
     bool visible_;
     TPos Frame_;

     int MaxRow_, MaxCol_;
     int Row_, Col_;

     void Init ( const char* fname, int MaxC, int MaxR);
     void Move ( int dx, int dy, TFon Fon);
     void Show ();
     };


TAnim AGirl;
TFon Fon, Map;
COLORREF ColEarth = RGB(0,255,0), ColSky = RGB (0, 0, 255);
const int step = 5;

void Game ();
void LegOnEarth (TPos* Leg, HDC Map, COLORREF Col);
//----------------------------------------------------------------------------------
int main()
    {
    Fon.Init ("фон.bmp");
    Map.Init ("map.bmp");

    txCreateWindow (Fon.Size_.x / 2, Fon.Size_.y);
    AGirl.Init("1.bmp", 6, 2 );

    Game();


    txDeleteDC(AGirl.Image_);
    txDeleteDC(Fon.Image_);
    }
void Game ()
    {
    while ( !txGetAsyncKeyState (VK_ESCAPE) )
        {
        Fon.Show();
        AGirl.Show();

        double dx = (txMouseX()-AGirl.Pos_.x-AGirl.Frame_.x/2) / step;
        double dy = (txMouseY()-AGirl.Pos_.y-AGirl.Frame_.y/2) / step;

        TPos Old = AGirl.Pos_;
        AGirl.Move( dx, dy, Fon);

        TPos Leg = { AGirl.Pos_.x+AGirl.Leg_.x, AGirl.Pos_.y+AGirl.Leg_.y};

        if ( Leg.y > Fon.Size_.y) AGirl.Pos_.y = Fon.Size_.y - AGirl.Frame_.y;
        if ( txGetPixel(Leg.x, Leg.y, Map.Image_ ) == ColSky )
            {
            LegOnEarth ( &Leg, Map.Image_, ColEarth);
            AGirl.Pos_.y = Leg.y - AGirl.Leg_.y;
            }
        if ( txGetPixel(Leg.x, Leg.y, Map.Image_ ) != ColEarth) AGirl.Pos_ = Old;

        txSleep(100);
        }
    }

void TAnim::Init ( const char* fname, int MaxC, int MaxR)
    {
    Image_ = txLoadImage (fname);
    visible_ = true;
    Frame_.x = txGetExtentX(Image_) / MaxC;
    Frame_.y = txGetExtentY(Image_) / MaxR;
    MaxRow_ = MaxR;
    MaxCol_ = MaxC;
    Pos_.x = Fon.Size_.x / 4;
    Pos_.y = Fon.Size_.y - Frame_.y * 1.5;
    Leg_.x = 80;
    Leg_.y = 146;
    Row_ = 0; Col_ = 0;
    visible_ = true;
    }
void TAnim::Show()
    {
    double KadrX = Frame_.x * Col_;
    double KadrY = Frame_.y * Row_;
    txTransparentBlt(txDC(), Pos_.x, Pos_.y, Frame_.x, Frame_.y, Image_, KadrX, KadrY, RGB(255, 0, 255));
    }
void TAnim::Move ( int dx, int dy, TFon Fon)
    {
    if (!Fon.Scroll_)
        {
        Pos_.x += dx;
        Pos_.y += dy;
        }

    if (dx<0) Row_ = 0;
    else Row_ = 1;
    Col_ = (Col_ + 1) % MaxCol_;
    }
void TFon::Init ( const char* fname)
    {
    Image_ = txLoadImage (fname);
    visible_ = true;
    Size_.x = txGetExtentX(Image_);
    Size_.y = txGetExtentY(Image_);
    Pos_.x = 0;
    Pos_.y = 0;
    Scroll_ = false;
    }
void TFon::Show ()
    {

    txBitBlt(txDC(), Pos_.x, Pos_.y, Size_.x, Size_.y, Image_);
    }
void TFon::Move ( int dx, int dy)
    {
    if  ( Scroll_ )
        {
        // тут будет движение фона со скроллингом
        }
    else
        {
        Pos_.x += dx;
        Pos_.y += dy;
        }

    }
void LegOnEarth ( TPos* Leg, HDC Map, COLORREF Col)
    {

    while ( txGetPixel ( (*Leg).x, (*Leg).y, Map) != Col) (*Leg).y++;

    }



/*
int random (int down, int up)
    {
    int r;
    r = rand() % (up-down+1)+down;
    return r;
    }
  */


