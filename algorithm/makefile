COMP = g++-8.3.0

SHARED_OBJS = _206039984_a.so

OBJS = BasicAlgorithm.o NaiveAlgorithm.o WeightBalanceCalculator.o $\
$\
../common/objects/Container.o ../common/objects/ContainersPosition.o ../common/objects/CraneManagement.o ../common/objects/Route.o $\
 ../common/objects/Ship.o  ../common/objects/ShipPlan.o  ../common/objects/Travel.o $\
$\
../common/utils/Errors.o ../common/utils/IO.o $\



CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC
CPP_LINK_FLAG = -shared

all: $(SHARED_OBJS)

_206039984_a.so: $(OBJS) _206039984_a.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

$(OBJS) _206039984_a.o:
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@

clean:
	rm -f $(OBJS) $(SHARED_OBJS)