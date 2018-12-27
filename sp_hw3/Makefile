
all: bidding_system bidding_system_EDF customer

# bidding_system
bidding_system: bidding_system.o
	gcc -o  bidding_system bidding_system.o
bidding_system.o: bidding_system.c
	gcc -c bidding_system.c


# bidding_system_EDF
bidding_system_EDF: bidding_system_EDF.o
	gcc -o  bidding_system_EDF bidding_system_EDF.o
bidding_system_EDF.o: bidding_system_EDF.c
	gcc -c bidding_system_EDF.c


# customer
customer: customer.o
	gcc -o  customer customer.o
customer.o: customer.c
	gcc -c customer.c






