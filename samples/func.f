( 
)
{
;
};

() {
    ;
};

(a, b) {
    outl a;
};

// there is an unresolved reduce/reduce conflict in the parser
// between:
// expr: T_SYMBOL .
// symbol: T_SYMBOL .
// but priority is set by the definition - should fix this...
(a) {
    ;
};

// see above
(a) {
    a;
};

f0 = () {
    outl "0 args";
};

f1 = (a) {
    outl "1 arg";
};

f2 = (a, b) {
    outl "2 args";
};

// will fail...
// nested
() {
    () {

    };
};


