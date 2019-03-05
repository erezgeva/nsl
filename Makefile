ALL:=test1 test2
CPPFLAGS:=-Wall -Wextra -Winline -g
#CFLAGS:=-ansi
# -ansi -std=c89
# -std=c99
#CXXFLAGS:=-ansi
# -ansi -std=c++98
# -std=c++11
LDLIBS:=-lm
all: clean $(ALL)
clean:
	$(RM) $(ALL)
deb:
	dpkg-buildpackage -b --no-sign
