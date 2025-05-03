#pragma once

//#ifndef TEXT_RANKER_H
//#define TEXT_RANKER_H

#include <string>
#include <vector>
#include <set>
#include "Paragraph.h"
#include "IntervalTree.h"
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <iterator>


class TextRanker {
public:
    explicit TextRanker()
        : m_d(0.85), mMaxIter(100), mTol(1.0e-5) { }
    explicit TextRanker(double d, int maxIter, double tol)
        : m_d(d), mMaxIter(maxIter), mTol(tol) { }

     ~TextRanker() { }

     std::vector<std::string> ExtractKeyParagraphs(const std::string& input, std::vector<std::vector<std::pair<int, int>>> characters, int topK);

private:
    bool ExtractParagraphs(const std::string& input, std::vector<Paragraph>& output);
    bool RemoveDuplicates(const std::vector<Paragraph>& input, std::vector<Paragraph>& output);
    bool BuildGraph(std::vector<Paragraph>& paragraphs, const std::vector<std::vector<Interval>>& characters);
    double GetSimilarity(int a, int b);
    bool CalcParagraphScores();
    bool InitCharsList(std::vector<Paragraph>& paragraphs, const std::vector<std::vector<Interval>> characters);
    //void StringReplaceAll(std::string& str, const std::string& from, const std::string& to);

//private:
	std::string mInput;  // The input text
	std::vector<std::vector<Interval>> mCharacters;  // The location of characters in the input text

    double m_d;  // The parameter d in the iteration formula
	int mMaxIter;   // Maximum number of iterations
    double mTol;   // Iteration accuracy
    std::vector<Paragraph> mParagraphs;  // Paragraphs after segmentation
    std::vector< std::vector<double>> mAdjacencyMatrix;  // Adjacency Matrix
    std::vector<double> mOutWeightSum;  // The weight of each node’s outbound link 
    std::vector<double> mScores;  // The score of each node
    //std::vector< std::set<std::string> > mWordSets;    // The set of words each sentence contains
};

//#endif
