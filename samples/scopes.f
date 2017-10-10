// a simple scopes example
() {

}

(cond) {
    stmt
}

([1..10]) {
    outl "hello"
}

(a = [1..10]) {
    outl a
}

(a = [1..10]) {
    outl a
    a = a + 1 // check range of a, throw error
}

add(a b) = {
    outl a + b
}

// module-level, named after file? or rename from file?
scopes(a b c) = { // a b c are parameters

}

