/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LATINIME_CORRECTION_H
#define LATINIME_CORRECTION_H

#include <stdint.h>
#include "correction_state.h"

#include "defines.h"

namespace latinime {

class ProximityInfo;

class Correction {

public:
    typedef enum {
        TRAVERSE_ALL_ON_TERMINAL,
        TRAVERSE_ALL_NOT_ON_TERMINAL,
        UNRELATED,
        ON_TERMINAL,
        NOT_ON_TERMINAL
    } CorrectionType;

    Correction(const int typedLetterMultiplier, const int fullWordMultiplier);
    void initCorrection(
            const ProximityInfo *pi, const int inputLength, const int maxWordLength);
    void initCorrectionState(const int rootPos, const int childCount, const bool traverseAll);

    // TODO: remove
    void setCorrectionParams(const int skipPos, const int excessivePos, const int transposedPos,
            const int spaceProximityPos, const int missingSpacePos);
    void checkState();
    bool initProcessState(const int index);

    void getProcessState(int *matchedCount, int *inputIndex, int *outputIndex,
            bool *traverseAllNodes, int *diffs);
    int getOutputIndex();
    int getInputIndex();
    bool needsToTraverseAll();

    virtual ~Correction();
    int getSpaceProximityPos() const {
        return mSpaceProximityPos;
    }
    int getMissingSpacePos() const {
        return mMissingSpacePos;
    }

    int getSkipPos() const {
        return mSkipPos;
    }

    int getExcessivePos() const {
        return mExcessivePos;
    }

    int getTransposedPos() const {
        return mTransposedPos;
    }

    bool needsToPrune() const;

    int getFreqForSplitTwoWords(const int firstFreq, const int secondFreq);
    int getFinalFreq(const int freq, unsigned short **word, int* wordLength);

    CorrectionType processCharAndCalcState(const int32_t c, const bool isTerminal);

    int getDiffs() const {
        return mDiffs;
    }

    /////////////////////////
    // Tree helper methods
    int goDownTree(const int parentIndex, const int childCount, const int firstChildPos);

    inline int getTreeSiblingPos(const int index) const {
        return mCorrectionStates[index].mSiblingPos;
    }

    inline void setTreeSiblingPos(const int index, const int pos) {
        mCorrectionStates[index].mSiblingPos = pos;
    }

    inline int getTreeParentIndex(const int index) const {
        return mCorrectionStates[index].mParentIndex;
    }
private:
    void charMatched();
    void incrementInputIndex();
    void incrementOutputIndex();
    void startTraverseAll();

    // TODO: remove

    void incrementDiffs() {
        ++mDiffs;
    }

    const int TYPED_LETTER_MULTIPLIER;
    const int FULL_WORD_MULTIPLIER;

    const ProximityInfo *mProximityInfo;

    int mMaxEditDistance;
    int mMaxDepth;
    int mInputLength;
    int mSkipPos;
    int mSkippedOutputIndex;
    int mExcessivePos;
    int mTransposedPos;
    int mSpaceProximityPos;
    int mMissingSpacePos;

    int mMatchedCharCount;
    int mInputIndex;
    int mOutputIndex;
    int mTerminalInputIndex;
    int mTerminalOutputIndex;
    int mDiffs;
    bool mTraverseAllNodes;
    unsigned short mWord[MAX_WORD_LENGTH_INTERNAL];

    CorrectionState mCorrectionStates[MAX_WORD_LENGTH_INTERNAL];

    inline bool isQuote(const unsigned short c);
    inline CorrectionType processSkipChar(const int32_t c, const bool isTerminal);

    class RankingAlgorithm {
    public:
        static int calculateFinalFreq(const int inputIndex, const int depth,
                const int matchCount, const int freq, const bool sameLength,
                const Correction* correction);
        static int calcFreqForSplitTwoWords(const int firstFreq, const int secondFreq,
                const Correction* correction);
    };
};
} // namespace latinime
#endif // LATINIME_CORRECTION_H
