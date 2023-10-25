alias ccpp='g++ -Wall -Wextra -Wconversion -Wshadow -fsanitize=undefined,address -D_GLIBCXX_DEBUG -std="c++20" -g'

cppdiff() {
    ccpp $1
    ./a.out < "$2.in" | diff "$2.out" -
}
cpphash() {
    cpp -dD -P -fpreprocessed $1 2> /dev/null | tr -d '[:space:]' | md5sum | cut -c -6
}
