a = 10;
while (a) {
    
    out "a: ";
    outl a;

    if (a < 3) {
        outl "a < 3";
    }

    if (a <= 1) {
        outl "a <= 1";
    }

    if (a > 7) {
        outl "a > 7";
    }

    if (a >= 9) {
        outl "a >= 9";
    }

    if (a == 5) {
        outl "a == 5";
    }

    if (a != 4) {
        outl "a != 4";
    }

    a = a - 1;
}