//This code comes with no warranty.  Most of the Linux portion of this code 
//belongs to George foot gfoot@users.sourceforge.net.  The Windows portion
//of this code is public domain.

#ifdef WIN32
// *** WINDOWS API CONSOLE CODE ***

#include <conio.h>
#include <windows.h>

void conio_init (void) { system("cls"); }
void conio_exit (void) {}
int conio_kbhit (void) { return kbhit(); }
int conio_getch (void) { return getch(); }
void conio_gotoxy (int x, int y) {
  COORD pos = {x, y};
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
//conio_getsize returns x == y == 0 if it cannot get the size
void conio_getsize(int* x, int* y) {
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
  *x = info.dwSize.X;
  *y = info.dwSize.Y;
}
//conio_settitle sets a title for the console window, whereever possible,
//else does nothing.
void conio_settitle(const char* title) {
  SetConsoleTitle(title);
}

#else
// *** Try to use ncurses ***
//Following functions by George Foot

#include <ncurses.h>

static int __gf__cached__gf__ = 0, __gf__cached_value__gf__ = 0;

inline void conio_init (void)
{
	int x,y;
	initscr();
	start_color();
	cbreak();
	noecho();
	nonl();
//	intrflush (stdscr, FALSE);
	keypad (stdscr, TRUE);
	nodelay (stdscr, TRUE);
	scrollok (stdscr, TRUE);
}

inline void conio_exit (void)
{
	endwin();
}

inline int conio_kbhit (void)
{
  if (__gf__cached__gf__) return 1;
  __gf__cached_value__gf__ = getch();
  if (__gf__cached_value__gf__ != ERR) __gf__cached__gf__ = 1;
  return __gf__cached__gf__;
}

inline int conio_getch (void)
{
  if (__gf__cached__gf__) {
	__gf__cached__gf__ = 0;
	return __gf__cached_value__gf__;
  } else
	return getch();
}

inline void conio_gotoxy (int x, int y) { move (y-1, x-1); }

void conio_getsize(int* x, int* y) {
  *x = 0;
  *y = 0;
}
void conio_settitle(const char* title) {
}

#endif