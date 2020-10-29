#include <gdnative/string.h>

#include <fstream>
#include <reshaper.hpp>

#include "farsicharsdict.cpp"
#include "rvector.cpp"

using Farsi::AlphabetDict;
using std::vector;
using namespace godot;


void FarsiReshaper::_register_methods()
{
	/* no need to register _init function and constructor and destructor */
	register_method("getReshapedString", &FarsiReshaper::getReshapedString);
	register_method("setContentMargin", &FarsiReshaper::setContentMargin);
	register_method("getContentMargin", &FarsiReshaper::getContentMargin);
	register_method("enableAutoWrap", &FarsiReshaper::enableAutoWrap);
	register_method("disableAutoWrap", &FarsiReshaper::disableAutoWrap);

	register_method("setMaxLineWidth", &FarsiReshaper::setMaxLineWidth);
	register_method("setFont", &FarsiReshaper::setFont);

	return;
} // _register_methods()


FarsiReshaper::FarsiReshaper()
	: m_dict(AlphabetDict::getAlphabetDictionary()),
	  m_alph(m_dict.getAlphabets()),
	  m_spec(m_dict.getSpecials()),
	  m_other(m_dict.getOthersRange()),
	  m_nums(m_dict.getNumbers()),
	  m_contentMargin(10)
{
	return;
} // constructor.


void FarsiReshaper::_init()
{
	m_autoWrap = false;
	m_rcLoader = ResourceLoader::get_singleton();
	m_maxLineWidth = 0;
	m_currentLineWidth = 0;
	return;
} // _init()


void FarsiReshaper::disableAutoWrap()
{
	m_autoWrap = false;
	return;
} // disableAutoWrap


bool FarsiReshaper::enableAutoWrap(Font* __font, long __maxLineWidth)
{
	if (__maxLineWidth == 0) {
		m_autoWrap = false;
		return false;
	}
	m_maxLineWidth = __maxLineWidth;

	if (__font == nullptr) {
		m_autoWrap = false;
		return false;
	}
	m_font = Ref<Font>(__font);
	m_autoWrap = true;
	return true;
} // enableAutoWrap


void FarsiReshaper::setMaxLineWidth(int __maxLineWidth)
{
	m_maxLineWidth = __maxLineWidth;
	return;
} // setMaxWidth()


void FarsiReshaper::setFont(Font* __font)
{
	m_font = Ref<Font>(__font);
	m_maxLineWidth += m_font->get_string_size(" ").x;

	return;
}


void FarsiReshaper::setContentMargin(int __margin)
{
	m_contentMargin = __margin;
	return;
} // setContentMargin


int FarsiReshaper::getContentMargin() const
{
	return m_contentMargin;
} // getContentMargin


void FarsiReshaper::pushAlphBytes(vector<uint8_t>& token,
								  uint16_t currCp,
								  uint8_t form)
{
	token.push_back(0xef);
	auto vec = m_alph->at(currCp);
	for (uint8_t j = 0; j < 2; j++)
		token.push_back(vec[form + j]);
	return;
} // pushAlphBytes()


FarsiReshaper::CodepointType FarsiReshaper::getCodepointType(uint16_t cp)
{
	if (m_alph->count(cp)) {
		return cpALPHABET;
	} else if (m_nums->count(cp)) {
		return cpNUMBER;
	} else if (m_spec->count(cp)) {
		return cpSPECIAL;
	} else if (cp < m_other->second) {
		return cpOTHER;
	} else {
		return cpINVALID;
	}
} // getCodepointType()


bool FarsiReshaper::isDelimiter(uint16_t codepoint) const
{
	return (codepoint == cSPACE || codepoint == cNEWLINE || codepoint == cTAB);
}


String FarsiReshaper::getReshapedString(const godot::String __data)
{
	if (m_autoWrap == true && (m_maxLineWidth == 0 || m_font.is_null())) {
		return "Error: Either maxLineWidth is 0, or Font is null.";
	}

	m_result.clear();
	m_frStr = "";
	m_currentLineWidth = 0;
	/*
	 * We are goin to split our string to some charSets seperated by delimiters.
	 * The delimiter itself should be included into the charSet
	 */
	/* The following line of code frees us from checking if we are at the
	 * begining or end of our string and doing a different action based on that
	 */
	m_frStr = String(" ") + __data + String(" ");
	/* The reverse-vector class that is used for storing a line of string.
	 */
	Rvector<uint8_t> line;

	int indx = 1;				  // Low index over the origin string (m_frStr)
	int lastDelimIndx = indx - 1; // hold position of last delimiter
	Range charSetRange;
	while (indx < m_frStr.length()) {
		if (isDelimiter(m_frStr[indx]) == true) {
			charSetRange.set(lastDelimIndx, indx);
			parseCharSet(charSetRange, line);
			lastDelimIndx = indx;
		}
		indx++;
	}

	line.at(line.capacity() - 1) = cNEWLINE;
	for (uint8_t byte : line) {
		m_result.push_back(byte);
	}
	m_result[m_result.size() - 1] = 0;
	String str((char*)m_result.data());
	return str;
} // getReshapedBytes()


void FarsiReshaper::parseCharSet(Range& range, Rvector<uint8_t>& line)
{
	/* Tokens represent a part of text that is parsed and going to be added to
	 * result string.
	 */
	vector<uint8_t> newToken;

	/* Now we parse this founded token */
	int indx = range.high - 1; // Index over the founded token
	int lastTokenIndx = indx;
	CodepointType lastCpType = getCodepointType(m_frStr[indx]);
	Range tokenRange;
	while (true) {
		if (getCodepointType(m_frStr[indx]) == lastCpType && indx > range.low) {
			/* In this case the current character is of type of the last
			 * character and it should be considered at the same token as the
			 * last character. So we just decrease the indx and continue the
			 * loop
			 */
			indx--;
			continue; /* It's not really neccessary. */
		} else {
			tokenRange.set(indx, lastTokenIndx);
			switch (lastCpType) {
				case cpALPHABET:
					renderAlphabets(tokenRange, newToken);
					break;
				case cpNUMBER:
					renderNumbers(tokenRange, newToken);
					break;
				case cpSPECIAL:
					renderSpecials(tokenRange, newToken);
					break;
				case cpOTHER:
					renderOthers(tokenRange, newToken);
					break;
				default:
					break;
			} // switch
			if (indx <= range.low) break;
			lastTokenIndx = indx;
			lastCpType = getCodepointType(m_frStr[indx]);
		} // else
	}	  // while

	/* We know that m_frStr[range.low] is the delimiter (space, newline or
	 * tab). So if this delimiter is newline some actions need to be done.
	 */
	auto delimiter = m_frStr[range.low];
	if (delimiter == cNEWLINE) {
		m_currentLineWidth = 0;

		line.at(line.capacity() - 1) = cNEWLINE;
		for (uint8_t byte : line) {
			m_result.push_back(byte);
		}
		line.clear();
	}
	newToken.push_back(delimiter);

	/* If user asked for auto-wraping and provided us with a control node */
	if (m_autoWrap && !m_font.is_null()) {
		newToken.push_back(0);
		String newS((char*)newToken.data());
		long newSize = m_font->get_string_size(newS).x;
		newToken.pop_back();

		m_currentLineWidth += newSize;
		if (m_currentLineWidth >= m_maxLineWidth - m_contentMargin) {
			line.at(line.capacity() - 1) = cNEWLINE;
			m_currentLineWidth = newSize;
			for (uint8_t byte : line) {
				m_result.push_back(byte);
			}
			line.clear();
		}
	}

	line.prepend(newToken);

	return;
} // parsetoken()


void FarsiReshaper::renderAlphabets(Range& range, vector<uint8_t>& token)
{
	uint16_t currCp = -1; // holds current character codepoints in m_frStr.
	uint16_t nextCp = -1; // holds next character codepoints in m_frStr.
	uint16_t prevCp = -1; // holds previous character codepoints in m_frStr.

	int nextVecSize; // holds next character vector.size in m_frStr.
	int prevVecSize; // holds previous character vector.size in m_frStr.
	int currVecSize; // holds current character vector.size in m_frStr.
	for (int i = range.high; i > range.low; --i) {
		nextCp = m_frStr[i + 1];
		currCp = m_frStr[i];
		prevCp = m_frStr[i - 1];

		currVecSize = m_alph->at(currCp).size();

		try {
			nextVecSize = m_alph->at(nextCp).size();
		} catch (std::out_of_range& e) {
			nextVecSize = INVALID_VEC_SIZE;
		}

		try {
			prevVecSize = m_alph->at(prevCp).size();
		} catch (std::out_of_range& e) {
			prevVecSize = INVALID_VEC_SIZE;
		}
		/*
		COMMENT:
			Bear in mind that characters with 2 context form (vector.size is
		4) can only be ISOLATED and END context form.
		*/
		if (prevVecSize != CHAR_4_VEC_SIZE) {
			/* Current character can only be BEGINING or ISOLATED*/
			if (currVecSize == CHAR_2_VEC_SIZE) {
				/* Current character is ISOLATED. see COMMENT*/
				pushAlphBytes(token, currCp, ISOLATED);
				continue;
			} /* else */
			if (nextVecSize == INVALID_VEC_SIZE) {
				/* Current character is ISOLATED */
				pushAlphBytes(token, currCp, ISOLATED);
			} else {
				/* Current character is BEGINING */
				pushAlphBytes(token, currCp, BEGINNING);
			}

		} else {
			/* In this case the currenct character can be MIDDLE or END*/
			if (currVecSize == CHAR_2_VEC_SIZE) {
				/* Current character is END. see COMMENT*/
				pushAlphBytes(token, currCp, END);
				continue;
			} else if (nextVecSize == INVALID_VEC_SIZE) {
				/* Current character is END */
				pushAlphBytes(token, currCp, END);
			} else {
				/* Current character is MIDDLE */
				pushAlphBytes(token, currCp, MIDDLE);
			}
		}
	} // end of for

	return;
} // renderAlphabets()


void FarsiReshaper::renderNumbers(Range& range, vector<uint8_t>& token)
{
	// static const uint8_t cNUMBER_FIRST_BYTE = 0xdb;
	for (int i = range.low + 1; i <= range.high; ++i) {
		token.push_back(m_dict.cNUMBER_FIRST_BYTE);
		token.push_back(m_nums->at(m_frStr[i]));
	}

	return;
} // renderNumbers()


void FarsiReshaper::renderOthers(Range& range, vector<uint8_t>& token)
{
	for (int i = range.high; i > range.low; --i)
		token.push_back(m_frStr[i]);
	return;
} // renderOthers()


void FarsiReshaper::renderSpecials(Range& range, vector<uint8_t>& token)
{
	for (int i = range.high; i > range.low; --i)
		for (uint8_t byte : m_spec->at(m_frStr[i])) {
			token.push_back(byte);
		}
	return;
} // renderSpecials()


FarsiReshaper::~FarsiReshaper() { return; } // ~Reshaper()
