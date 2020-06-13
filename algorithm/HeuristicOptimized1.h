//
// Created by Oz Zafar on 12/06/2020.
//

#ifndef CPP_STOWAGE_HEURISTICOPTIMIZED1_H
#define CPP_STOWAGE_HEURISTICOPTIMIZED1_H

#include "NaiveAlgorithm.h"

class HeuristicOptimized1 : public NaiveAlgorithm {
    int getUnloadInstructions(const string &output_path) override;
    int getLoadInstructions(const string &input_path, const string &output_path) override ;

    /* try to move container, if there is no available then unload.
     * we do it because move operation costs less then unload (and
     * probably additional load to return back the container). this
     * method also writes the operation to the instructions file.*/
    void moveOrTemporaryUnloadContainer(const string &output_path, string &containerId, int i,
                                        int j, ContainersPosition &containersPosition, vector<string> &containersToReturn);

    void findFirstAvailablePosition(int i, int j, int& targetFloor, int &targetX, int &targetY);

    void finishIteration(int &index, int &saveX, int &saveY) const;

    void findNextBestPosition(Container& container, int &nextPositionX,int &nextPositionY);
};


#endif //CPP_STOWAGE_HEURISTICOPTIMIZED1_H
