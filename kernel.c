//in line 1 we are type casting the hexadecimal number in the pointer variable to recognize that as a memory address that points to a 1-byte of adress and store it
unsigned char *vga = (unsigned char *)0xA0000;
void draw_block(int start_x, int start_y, unsigned char color);
void delay();
char get_key();
char delay_and_read();
void main(){
    int snake_x=50,snake_y=50;
    int speed_x=10,speed_y=0;
    int apple_x=100, apple_y=100;
    int tail_x[100];// An array to remember the last 100 X positions
    int tail_y[100];// An array to remember the last 100 Y positions
    int tail_length=0;// Starts at 0, goes up when you eat an apple
    while(1)
    {
         char key = delay_and_read();
         //drawing snake and apple simultaneously when the game starts
         draw_block(snake_x,snake_y,15);
         for(int i=0;i<tail_length;i++)
         {
             draw_block(tail_x[i],tail_y[i],2);
        }
         draw_block(apple_x,apple_y,4);
         //giving a delay to give human eye vanish effect
         delay();
         //after delay changing the snake's last block colour to the black for making the snake block move
         draw_block(snake_x,snake_y,0);
         for(int i=0;i<tail_length;i++)
         {
             draw_block(tail_x[i],tail_y[i],0);
        }
         for(int l=tail_length;l>0;l--)
         {
             tail_x[l]=tail_x[l-1];
             tail_y[l]=tail_y[l-1];
         }
         tail_x[0]=snake_x;
         tail_y[0]=snake_y;
         snake_x +=speed_x;
         snake_y +=speed_y;
         for(int i=0;i<tail_length;i++)
         {
             if(snake_x == tail_x[i])
             {
                 if(snake_y == tail_y[i])
                 {
                     tail_length=0;
                     snake_x=50;
                     snake_y=50;
                }
            }
        }
         //using if and else if blocks to find the key stroked and make the snake move in that direction
         if(key == 0x11)
         {
             speed_y = -10;
             speed_x = 0;
        }
        else if(key == 0x1F)
        {
            speed_y = 10;
            speed_x = 0;
        }
        else if (key == 0x1E)
        {
            speed_x = -10;
            speed_y = 0;
        }
        else if (key == 0x20)
        {
            speed_x = 10;
            speed_y = 0;
        }
        //using if and else if blocks to keep the snake running inside the screen not affecting the other address that aren't related to the display and graphics if it did it would cause a severe problem
         if(snake_x >= 320)
         {
             snake_x = 0;
        }
        else if(snake_x<0)
        {
            snake_x=310;
        }
        else if(snake_y>=200)
        {
            snake_y=0;
        }
        else if(snake_y<0)
        {
            snake_y=190;
        }
        //to check if the snake had collided with the apple and turning the apple pixel into black
        if(snake_x == apple_x && snake_y == apple_y)
        {
            draw_block(apple_x,apple_y,0);
            apple_x = (apple_x + 70)%320;
            apple_y = (apple_y+50)%200;
            tail_length+=1;
        }
    }

}
//functions to draw the blocks for snake and apple using for loop
void draw_block(int start_x, int start_y, unsigned char color)
{
    for(int y=start_y ;y<start_y+10;y++){
        for(int x = start_x;x<start_x+10;x++){
            int index = y*320 + x;
            //the next line is to put the number 15 inside that memory address
            vga[index]=color;
        }
    }
}

void delay()
{
    for(volatile int i=0;i<50000000;i++);
}

char get_key() {
    char key = 0;
    // Assembly instruction 'inb' reads 1 byte from hardware port 0x60
    __asm__ volatile ("inb $0x60, %0" : "=a"(key));
    return key;
}

/*char delay_and_read()
{
    char pressed = 0;
    for(volatile int i=0; i<50000000; i++) {
        char current = get_key();
        // If the CPU sees W, A, S, or D at any point, remember it!
        if(current == 0x11 || current == 0x1E || current == 0x1F || current == 0x20) {
            pressed = current;
        }
    }
    return pressed; // Return the captured key back to the game loop
} old logic for delay and read */


char delay_and_read() {
    char pressed = 0;

    // Outer loop: Check the keyboard 50 times per frame
    for(int i = 0; i < 50; i++) {

        char current = get_key();
        if(current == 0x11 || current == 0x1E || current == 0x1F || current == 0x20) {
            pressed = current;
        }

        // Inner loop: Do 1 million cycles of pure, fast CPU math
        for(volatile int j = 0; j < 1000000; j++);
    }

    return pressed;
}
