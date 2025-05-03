#include "Paragraph.h"


const std::set<size_t>& Paragraph::GetCharacters() const {
	return mCharacters; 
}

size_t Paragraph::GetCharsNum() const { 
	return mCharsNum;
}

Interval  Paragraph::GetPosition() const { 
	return mPosition;
}

//const std::string& Paragraph::GetText() const { 
//	return mText;
//}

void Paragraph::SetCharacters(size_t character) {
	mCharacters.insert(character); 
	mCharsNum++; 
}
