# XXX: http://codeforces.com/blog/entry/15547

#OPT=-O2
OPT=-g3
gcc -Wall -Wextra -pedantic $OPT -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover -fstack-protector ass.c -o ass -DCACA_COMUN_LOGs
