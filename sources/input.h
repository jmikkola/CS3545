/*
 * input.h
 *
 *  Created on: Mar 3, 2011
 *      Author: jeremy
 */

#ifndef INPUT_H_
#define INPUT_H_

void input_update ();
void input_keyDown (SDLKey k);
void input_keyUp (SDLKey k);
void input_mouseMove (int xPos, int yPos);

extern int keys_down[330];

#endif /* INPUT_H_ */
