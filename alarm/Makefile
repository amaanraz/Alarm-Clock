
all:
	arm-linux-gnueabihf-gcc -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror threads/input.c threads/alarm.c threads/clock.c modules/misc.c modules/gpio.c modules/lcd.c main.c -lpthread -o main -lm
	cp main $(HOME)/cmpt433/public/myApps/project

clean:
	rm main
	