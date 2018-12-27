### compile all
all: bidding_system host player

### bidding_system
bidding_system: bidding_system.o
	gcc -o bidding_system bidding_system.o
bidding_system.o: bidding_system.c
	gcc -c bidding_system.c

host: host.o
	gcc -o host host.o
host.o: host.c
	gcc -c host.c

### player
player: player.o
	gcc -o player player.o
player.o: player.c
	gcc -c player.c

### clean
clean:
	rm -rf bidding_system bidding_system.o host host.o player player.o

