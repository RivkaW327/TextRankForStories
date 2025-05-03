#pragma once
//#include <string>
#include <set>
#include "IntervalTree.h"
#include <string>


class Paragraph
{

public:
	// Constructor
	Paragraph() : mPosition({ 0, 0 }), mCharsNum(0) {}
	Paragraph(Interval position)
		: mPosition(position), mCharsNum(0) { }
	// Destructor
	~Paragraph() {}

	bool operator==(const Paragraph& other) const {
		return mPosition.high == other.mPosition.high && mPosition.low == other.mPosition.low && mCharsNum == other.mCharsNum && mCharacters == other.mCharacters;
	}

	// Getters
	//const std::string& GetText() const;
	Interval GetPosition() const;
	//size_t GetStartIndex() const { /*return mStartIndex; }
	//size_t GetEndIndex() const { return mEndIndex*/; }
	size_t GetCharsNum() const;
	const std::set<size_t>& GetCharacters() const;

	//Setters
	//void SetCharsNum(size_t charsNum) { mCharsNum = charsNum; }
	void SetCharacters(size_t character);


private:
	//std::string mText;  // The text of the paragraph
	Interval mPosition; // The position of the paragraph
	//size_t mStartIndex;  // The start index of the paragraph
	//size_t mEndIndex;  // The end index of the paragraph
	size_t mCharsNum;  // The number of chars in the paragraph
	std::set<size_t> mCharacters;  // The characters in the paragraph


};

//namespace std {
//	template <>
//	struct hash<Paragraph> {
//		size_t operator()(const Paragraph& p) const {
//			return std::hash<std::string>()(p.GetText());//^ std::hash<Interval>()(p.GetPosition());
//		}
//	};
//}
