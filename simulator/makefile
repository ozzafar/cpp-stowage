COMP = g++-9.3.0

OBJS = main.o Simulation.o AlgorithmResults.o NumTasks.o NumThreads.o SimpleTasksProducer.o ThreadPoolExecuter.o AlgorithmRegistration.o $\
$\
../common/objects/Container.o ../common/objects/ContainersPosition.o ../common/objects/CraneManagement.o ../common/objects/Route.o $\
../common/objects/Ship.o ../common/objects/ShipPlan.o  ../common/objects/Travel.o ../common/objects/Named.o $\
$\
../common/utils/Errors.o ../common/utils/ErrorsIterator.o ../common/utils/IO.o ../common/utils/Registrar.o ../common/utils/ISO_6346.o $\
$\
 ../interfaces/WeightBalanceCalculator.o  $\


EXEC = simulator
CPP_COMP_FLAG = -std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -ldl -export-dynamic -lstdc++fs -pthread #(-rdynamic)

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@
$(OBJS):
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp -o $@

clean:
	rm -f $(OBJS) $(EXEC)

# ./simulator -travel_path ../input/travels -algorithm_path ../algorithm -output ../input -num_threads 5