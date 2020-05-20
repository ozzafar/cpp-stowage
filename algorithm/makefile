COMP = g++-9.3.0

SHARED_OBJS = _206039984_a.so _206039984_b.so _206039984_c.so

OBJS = BasicAlgorithm.o NaiveAlgorithm.o NaiveAlgorithmVertical.o NaiveAlgorithmHorizontal.o  $\
$\
../common/objects/Container.o ../common/objects/ContainersPosition.o ../common/objects/CraneManagement.o ../common/objects/Route.o $\
 ../common/objects/Ship.o  ../common/objects/ShipPlan.o  ../common/objects/Travel.o $\
$\
../common/utils/Errors.o ../common/utils/ErrorsIterator.o  ../common/utils/IO.o ../common/utils/ISO_6346.o $\
$\
../interfaces/WeightBalanceCalculator.o $\


CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
CPP_LINK_FLAG = -shared

all: $(SHARED_OBJS)

_206039984_a.so: $(OBJS) _206039984_a.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_206039984_b.so: $(OBJS) _206039984_b.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_206039984_c.so: $(OBJS) _206039984_c.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

$(OBJS) _206039984_a.o _206039984_b.o _206039984_c.o:
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@

clean:
	rm -f $(OBJS) $(SHARED_OBJS)