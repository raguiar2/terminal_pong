#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#define EXIT_SUCCESS 0
#define PADDLE "|\n  |\n  |\n  |"
#define BALL "o"
// Stores our balls position
typedef struct _Ball {
    int x;
    int y;
} Ball;
// Stores the direction that the ball is moving
typedef struct _Direction {
    int x;
    int y;
} Direction;
// store paddle position
typedef struct _Paddle {
    int x;
    int y;
} Paddle;

// Takes in a user input and moves the paddle that direction
Paddle moveUserPaddle(Paddle userPaddle){
  if (getch() == '\033') { // if the first value is esc
    getch(); // skip the [
    switch(getch()) { // the real value
        case 'A':
            // code for arrow up
	  userPaddle.y--;
	  break;
        case 'B':
            // code for arrow down
	  userPaddle.y++;
            break;
    }
  }
  mvprintw(userPaddle.y,userPaddle.x,PADDLE);
  return userPaddle;
}

Paddle moveComputerPaddle(Paddle computerPaddle, Ball b, int width, char* comp_paddle){
  //construct computer paddle string
  //comp_paddle[0] = '|';
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < width; j++){
      comp_paddle[i*width+j] = ' ';
    }
    comp_paddle[i*width+width-1] = '|';
    comp_paddle[i*width+width] = '\n';
  }
  computerPaddle.y = b.y-2;
  mvprintw(computerPaddle.y,computerPaddle.x,comp_paddle);
  return computerPaddle;
}

// checks to see if the ball boucnes off the given paddle. 
void checkBounceOffPaddle(Paddle paddle, Ball* b, Direction* d){
  bool inPaddle = ((*b).y < (paddle.y + 4) && (*b).y >= (paddle.y));
  if(inPaddle && (*b).x == paddle.x){
        (*d).x *= -1;
        (*b).x += (*d).x;
    }
}

void playGame(Ball b, Direction d, Paddle userPaddle, Paddle computerPaddle, int width, int height) {
   char* comp_paddle = malloc(4*width+6);
   while (true) {
    userPaddle = moveUserPaddle(userPaddle);
    computerPaddle = moveComputerPaddle(computerPaddle,b, width, comp_paddle);
    // print ball
    mvprintw(b.y,b.x,BALL);

    // move ball for next frame
    b.y += d.y;
    b.x += d.x;

    // Check for ball being outside of our window boundaries
    // bouncecs off back wall for now
    if (b.x == width - 1 || b.x == 0){
        // reset ball and make it go in a random direction
        b.x = width/2;
        b.y = height/2;
        d.x *= -1;
        b.x += d.x;
    }
    if (b.y == height - 1 || b.y == 0){
        // change direction
        d.y *= -1;
        b.y += d.y;
    }
    //bounce off paddle
    checkBounceOffPaddle(userPaddle,&b,&d);
    checkBounceOffPaddle(computerPaddle,&b,&d);
    refresh();      // Refresh the output
    usleep(50000);  // Sleep to show output (Single frame)
    clear();        // Clear output
  }
   free(comp_paddle);
}

int main() {
  initscr();              // Start ncurses
  nodelay(stdscr, TRUE);  // Don't wait for \n for getch to parse input
  cbreak();               // Switch off input buffering
  curs_set(FALSE);        // Don't place a cursor on the screen
  int height = 0;         // Window height
  int width = 0;          // Window width

// Get terminal window dimensions (rows and characters)
getmaxyx(stdscr, height, width);
  
// initialize our ball struct and direction struct
Ball b = {
    width/2,height/2
};
Direction d = {
    1,1
};
 
// initalze the two paddles
Paddle userPaddle = {
  2, height/2
};

Paddle computerPaddle = {
  width-2, width/2 
};
 while(true){
   // set position of everything
   // wait for user input
   // play game
   playGame(b,d,userPaddle,computerPaddle, width, height);
 }

}
