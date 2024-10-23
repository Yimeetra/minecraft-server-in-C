#include <stdio.h>

int main()
{
    enum states { before, inside, after } state;
    int c;
  
    state = before;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            putchar('\n');
            state = before;
            continue;
        }
        switch (state) {
        case before:
            if (c != ' ') {
                putchar(c);
                state = inside;
            }
            break;
        case inside:
            if (c == ' ')
                state = after;
            else
                putchar(c);
            break;
        case after:
            break;
        }
    }

    return 0;
}