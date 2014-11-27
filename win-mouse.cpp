/*

 ~~__ ____   v   v           \_|_/
   ||/    \    v           __/o o\__
   |/      \       (@@@@)    \_u_/
   /        \     (@@@@@@)   / | \
  /  MOUSE   \    (@@@@@@)
  ------------     (@@@@)
  | __     __|       ||
  ||#| __ |#||       ||
~~|----||----|~~~~~~~||~~~~~~
       ##           *   *
*    * ##   *  *      *
       ##       *
*  *    ## *           *
*        ##     *
         ##
*/

#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);

class MouseController
{

  void mouseClick(DWORD flags){
    INPUT  Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = flags;
    SendInput(1, &Input, sizeof(INPUT));
  }

public:

  double SCREENWIDTH = GetSystemMetrics(SM_CXSCREEN) - 1;
  double SCREENHEIGHT = GetSystemMetrics(SM_CYSCREEN) - 1;

  //////////////// LEFT ////////////////

  void mouseLeftDown() { mouseClick(MOUSEEVENTF_LEFTDOWN); }

  void mouseLeftUp() { mouseClick(MOUSEEVENTF_LEFTUP); }

  void mouseLeftClick()
  {
    mouseLeftDown();
    mouseLeftUp();
  }

  void mouseDblLeftClick()
  {
    mouseLeftClick();
    mouseLeftClick();
  }

  //////////////// RIGHT ////////////////

  void mouseRightDown() { mouseClick(MOUSEEVENTF_RIGHTDOWN); }

  void mouseRightUp() { mouseClick(MOUSEEVENTF_RIGHTDOWN); }

  void mouseRightClick()
  {
    mouseRightDown();
    mouseRightUp();
  }

  void mouseDblRightClick()
  {
    mouseRightClick();
    mouseRightClick();
  }

  //////////////// MOVE ////////////////

  void mouseMove(int x, int y)
  {
    double fx = x*(65535.0f / SCREENWIDTH);
    double fy = y*(65535.0f / SCREENHEIGHT);
    INPUT  Input = { 0 };
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    Input.mi.dx = fx;
    Input.mi.dy = fy;
    SendInput(1, &Input, sizeof(INPUT));
  }
};
