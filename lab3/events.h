#ifndef _EVENTS_H_
#define _EVENTS_H_

void keyboard(unsigned char key, int x, int y);
void skeyboard(int key, int x, int y);
void mButton(int button, int state, int x, int y);
void mMotion(int x, int y);

extern float zoomFactor;

#endif
