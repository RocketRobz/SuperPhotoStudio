#ifndef SCREEN_VARS_H
#define SCREEN_VARS_H

// List all common screen variables here.
extern u8 sysRegion;
extern bool showCursor;
extern int cursorAlpha;
extern int studioBg;
extern bool cinemaWide;
extern int iFps;

// List all common screen functions here.
extern void sndBack(void);
extern void sndSelect(void);
extern void sndHighlight(void);
extern bool touchingBackButton(void);

#endif // SCREEN_VARS_H