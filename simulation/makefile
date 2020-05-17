COMP = g++-8.3.0

OBJS = main.o Simulation.o AlgorithmResults.o $\
$\
../common/objects/Container.o ../common/objects/ContainersPosition.o ../common/objects/CraneManagement.o ../common/objects/Route.o $\
../common/objects/Ship.o ../common/objects/ShipPlan.o  ../common/objects/Travel.o $\
$\
../common/utils/Errors.o ../common/utils/ErrorsIterator.o ../common/utils/IO.o ../common/utils/Registrar.o $\
$\
../interfaces/AlgorithmRegistration.o ../interfaces/WeightBalanceCalculator.o


EXEC = simulator
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -ldl -export-dynamic -lstdc++fs #(-rdynamic)

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@

clean:
	rm -f $(OBJS) $(EXEC)

# ./simulator -travel_path ../travels -algorithm_path ../algorithms -output ../output