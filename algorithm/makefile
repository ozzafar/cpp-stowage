COMP = g++-9.3.0

SHARED_OBJS = _206039984_a.so _206039984_b.so _206039984_c.so _206039984_d.so _206039984_e.so _206039984_f.so _206039984_g.so

OBJS = BasicAlgorithm.o NaiveAlgorithm.o NaiveAlgorithmVertical.o NaiveAlgorithmHorizontal.o OptimizedAlgorithmHorizontal.o OptimizedAlgorithmVertical.o HeuristicOptimized1.o HeuristicOptimized2.o $\
$\
../common/objects/Container.o ../common/objects/ContainersPosition.o ../common/objects/CraneManagement.o ../common/objects/Route.o $\
 ../common/objects/Ship.o  ../common/objects/ShipPlan.o  ../common/objects/Travel.o ../common/objects/Named.o $\
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

_206039984_d.so: $(OBJS) _206039984_d.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_206039984_e.so: $(OBJS) _206039984_e.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_206039984_f.so: $(OBJS) _206039984_f.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

_206039984_g.so: $(OBJS) _206039984_g.o
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

$(OBJS) _206039984_a.o _206039984_b.o _206039984_c.o _206039984_d.o  _206039984_e.o _206039984_f.o _206039984_g.o :
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@

clean:
	rm -f $(OBJS) $(SHARED_OBJS)