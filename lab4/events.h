#ifndef _EVENTS_H_
#define _EVENTS_H_

void keyboard(unsigned char key, int x, int y);
void skeyboard(int key, int x, int y);
void mouseMotion(int x, int y);
void mouseButton(int button, int state, int x, int y);

float zoomFactor;

int winWidth;
int winHeight;

#endif
