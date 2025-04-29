#include "text_ranker.h"
#include "IntervalTree.h"
#include <iostream>


// קמפול:
//  C:\Users\user\Documents\year2\project\TextRank\TextRank>C:\Users\user\AppData\Local\Programs\Python\Python312\python.exe setup.py build_ext --inplace

static std::vector<Paragraph> split_with_positions(const std::string& str, const std::string& delimiter) {
    std::vector<Paragraph> tokens;
    size_t start = 0, end = 0;
    Interval intr;
    while ((end = str.find(delimiter, start)) != std::string::npos) {
        intr.low = start;
        intr.high = end;
        tokens.push_back(Paragraph(str.substr(start, end - start), intr));
        start = end + delimiter.length();
    }
    if (start < str.length()) {
        intr.low = start;
        intr.high = str.length();
        tokens.push_back(Paragraph(str.substr(start), intr));
    }
    return tokens;
}

static std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = str.find(delimiter, start)) != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }
    if (start < str.length()) {
        tokens.push_back(str.substr(start));
    }
    return tokens;
}



static bool PairComp(std::pair<int, double> a, std::pair<int, double> b) 
{
    //return a.second > b.second;
    return a.second < b.second;
}

bool TextRanker::ExtractKeyParagraphs(const std::string& input, std::vector<std::vector<std::pair<int, int>>> characters, std::vector<Paragraph>& outputs, int topK)
{
    outputs.clear();
    if (input.empty() || topK < 1) {
        return false;
    }

    for (size_t i = 0; i < characters.size(); i++)
    {
        std::vector<Interval> character;
        for (size_t j = 0; j < characters[i].size(); j++)
        {
            character.push_back({ characters[i][j].first, characters[i][j].second });
        }
        this->mCharacters.push_back(character);
		//character.clear();
    }

    // TextRank
    bool ret = true;
    ret &= ExtractParagraphs(input, this->mParagraphs);
    ret &= BuildGraph(this->mParagraphs, this->mCharacters);
    ret &= CalcParagraphScores();

    if (!ret) {
        return false;
    }

    // Return the sentences with the highest score
    int kDim = mParagraphs.size();
    std::vector< std::pair<int, double> > visitPairs;  // (id, score)
    for(int i=0; i<kDim; ++i) {
        visitPairs.push_back(std::pair<int, double>(i, mScores[i]));
    }

	//quickSort(visitPairs, 0, visitPairs.size() - 1, PairComp); // Sort the pairs based on the score
    std::sort(visitPairs.begin(), visitPairs.end(), PairComp);

    for(int i=0; i<topK && i<kDim; ++i) {
        int id = visitPairs[i].first;
        outputs.push_back(mParagraphs[id]);
    }
    return true;
}

bool TextRanker::ExtractParagraphs(const std::string& input, std::vector<Paragraph>& outputs)
{
    outputs.clear();
    if (input.empty()) { 
        //outputs.push_back({ "", 0, 0 });
        return false; 
    }

    static const int maxTextLen = 100000;  // Maximum number of characters (need to consider word separators, UTF encoding, etc.)
    std::string tempInput;
    if ((int)input.size() > maxTextLen) {
        tempInput = input.substr(0, maxTextLen);  // Articles that are too long will be truncated
    } else {
        tempInput = input;
    }
    //std::transform(tempInput.begin(), tempInput.end(), tempInput.begin(), ::tolower);

    // Replace all sentence ending punctuation with `.`
    //static const std::string punctuations[] = {"?", "!", ".", ";", "？", "！", "。", "；", "……", "…", "\n"};
    //for (int i=0; i<(int)(sizeof(punctuations)/sizeof(punctuations[0])); ++i){
    //    std::string punc = punctuations[i];
    //    StringReplaceAll(tempInput, punc, ".");
    //}

    // Paragraph segmentation
    static const int minParagraphLen = 40;   // Minimum number of characters in a sentence (need to consider word separators, UTF encoding, etc.)
    std::vector<Paragraph> tempOutput = split_with_positions(tempInput, "###PARA###");
    std::vector<Paragraph> tempOutput2;
    for (int i=0; i<(int)tempOutput.size(); i++) {
        if ((int)(tempOutput[i].GetPosition().high-tempOutput[i].GetPosition().low) < minParagraphLen) {
            tempOutput2.push_back(tempOutput[i]);   // The number of characters in a single sentence is too small, so it is discarded.
        }
        else {
            outputs.push_back(tempOutput[i]);
        }
    }

     //Deduplication
    //RemoveDuplicates(tempOutput2, outputs);

    // If there are too many sentences, they will be truncated
    static const int maxParagraphsNum = 30;
    if ((int)outputs.size() > maxParagraphsNum) {
        outputs.resize(maxParagraphsNum);
    }

    return true;
}

//bool TextRanker::RemoveDuplicates(const std::vector<Paragraph>& inputs, std::vector<Paragraph>& outputs)
//{
//    outputs.clear();
//
//    // לבדוק עד כמה נכון לעשות סט של אובייקטים בשביל למנוע כפילויות
//    std::unordered_set<Paragraph> s(inputs.begin(), inputs.end());
//    outputs = std::vector<Paragraph>(s.begin(), s.end());  
//
//    return true;
//}

bool TextRanker::BuildGraph(std::vector<Paragraph>& paragraphs,const std::vector<std::vector<Interval>>& characters)
{
    if (paragraphs.empty()) { return false; }

    int kDim = paragraphs.size();

    // Calculate the adjacency matrix
    mAdjacencyMatrix.clear();
    mAdjacencyMatrix.resize(kDim, std::vector<double>(kDim, 0.0));

    InitCharsList(paragraphs, characters); // The words contained in each sentence are made into a `set` in advance to speed up the calculation of GetSimilarity.

    for(int i = 0; i < kDim - 1; i++)
    {
        for(int j = i + 1; j < kDim; j++)
        {
            double similarity = GetSimilarity(i, j);
            // the similarity matrix is symmetrical, so transposes are filled in with the same similarity
            mAdjacencyMatrix[i][j] = similarity;
            mAdjacencyMatrix[j][i] = similarity;
        }
    }

    // Count the weight and number of outbound links of each node
    mOutWeightSum.clear();
    mOutWeightSum.resize(kDim, 0.0);

    for (int i=0; i<kDim; ++i) {
        for (int j=0; j<kDim; ++j) {
            if (i==j) { continue; }
            mOutWeightSum[i] += mAdjacencyMatrix[i][j];
        }
    }

    return true;
}

bool TextRanker::InitCharsList(std::vector<Paragraph>& paragraphs, const std::vector<std::vector<Interval>> characters)
{
    int kDim = paragraphs.size();
    if (paragraphs.empty()) {
        return false;
    }

    //std::vector<Interval> ints;

	// init the interval tree
    std::unique_ptr<Node> root = nullptr;
    for (size_t i = 0; i < (int)paragraphs.size(); i++)
    {
        //ints.push_back(paragraphs[i].GetPosition());
        root = Node::insertTree(std::move(root), std::make_unique<Node>(i, paragraphs[i].GetPosition()));
    }
    root->inorder();
	// check the intervals
    for (size_t i = 0; i < characters.size(); i++)
    {
        for (size_t j = 0; j < characters[i].size(); j++)
        {
            Node* res = root->overlapSearch(characters[i][j]);
            if (res == nullptr)
                // צריך לבדוק מה לעשות במקרה של דמות שחוצה את הפסקאות
                std::cout << "\nNo overlaps ["<<characters[i][j].low<<" , "<<characters[i][j].high<<"]\n";
                // לסדר שאם הוא נמצא בשתיהם אז הקשת שלהם תקבל ניקוד גבוה.
            else
                paragraphs[res->GetParagraphIndex()].SetCharacters(i);

        }
    }
    return true;
}

double TextRanker::GetSimilarity(int a, int b)
{
    // if a or b does not contains characters
    if (mParagraphs[a].GetCharsNum() == 0 || mParagraphs[b].GetCharsNum() == 0) {
        return 0.0;
    }

    //int commonChars = 0;
    /*for (int val : mParagraphs[a].GetCharacters()) {
        if (mParagraphs[b].GetCharacters().count(val) > 0) {
            commonChars++;
        }
    }*/

    std::vector<size_t> commonChars;
	std::set_intersection(
		mParagraphs[a].GetCharacters().begin(),
		mParagraphs[a].GetCharacters().end(),
		mParagraphs[b].GetCharacters().begin(),
		mParagraphs[b].GetCharacters().end(),
		std::back_inserter(commonChars)
	);
    
    double denominator = std::log(static_cast<double>(mParagraphs[a].GetCharsNum())) + std::log(static_cast<double>(mParagraphs[b].GetCharsNum()));
    if (std::fabs(denominator) < 1e-6) {
        return 0.0;
    }
    return 1.0 * commonChars.size() / denominator;
}

bool TextRanker::CalcParagraphScores()
{
    if (mAdjacencyMatrix.empty() || mAdjacencyMatrix[0].empty() || mOutWeightSum.empty()) {
        return false;
    }

    int kDim = mParagraphs.size();

    // Initially, the score of all nodes is 1.0
    mScores.clear();
    mScores.resize(kDim, 1.0);

    // iterate
    int iterNum=0;
    for (; iterNum<mMaxIter; iterNum++) {
        double maxDelta = 0.0;
        std::vector<double> newScores(kDim, 0.0); // current iteration score

        for (int i=0; i<kDim; i++) {
            double sum_weight = 0.0;
            for (int j=0; j<kDim; j++) {
                if (i == j || mOutWeightSum[j] < 1e-6)
                    continue;
                double weight = mAdjacencyMatrix[j][i];
                sum_weight += weight/mOutWeightSum[j] * mScores[j];
            }
            double newScore = 1.0-m_d + m_d*sum_weight;
            newScores[i] = newScore;

            double delta = fabs(newScore - mScores[i]);
            maxDelta = std::max(maxDelta, delta);
        }

        mScores = newScores;
        if (maxDelta < mTol) {
            break;
        }
    }

    // std::cout << "iterNum: " << iterNum << "\n";
    return true;
}

//void TextRanker::StringReplaceAll(std::string& str, const std::string& from, const std::string& to) {
//    if (from.empty()) { return; }
//
//    bool isToContainFrom = false;   // In case 'to' contains 'from', like replacing 'x' with 'yx'
//    if (to.find(from, 0) != std::string::npos) {
//        isToContainFrom = true;
//    }
//
//    size_t start_pos = 0;
//    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
//        str.replace(start_pos, from.length(), to);
//        if (isToContainFrom) {
//            start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
//        }
//    }
//}


