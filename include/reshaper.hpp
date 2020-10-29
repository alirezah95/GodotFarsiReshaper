#ifndef RESHAPER_HPP
#define RESHAPER_HPP

#include <Control.hpp>
#include <Font.hpp>
#include <Node.hpp>
#include <ResourceLoader.hpp>
#include <core/Godot.hpp>
#include <core/Ref.hpp>
#include <core/String.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>


using std::map;
using std::pair;
using std::shared_ptr;
using std::string;
using std::vector;

template <class T> class Rvector;

namespace godot
{
class FarsiReshaper;
}

namespace Farsi
{
class AlphabetDict;
}


struct Range
{
	int low;
	int high;
	void set(int __low, int __high)
	{
		low = __low;
		high = __high;
	}
};


class godot::FarsiReshaper : public godot::Node
{
#define ISOLATED 0 * 2
#define END 1 * 2
#define BEGINNING 2 * 2
#define MIDDLE 3 * 2
#define CHAR_4_VEC_SIZE 8
#define CHAR_2_VEC_SIZE 4
#define INVALID_VEC_SIZE -1
	GODOT_CLASS(FarsiReshaper, Node)
public:
	static void _register_methods();

	FarsiReshaper();
	~FarsiReshaper();

	void _init(); // must exits even if no code is put in it

	void disableAutoWrap();
	bool enableAutoWrap(Font* __font, long __maxWidth);
	void setMaxLineWidth(int __maxWidth);
	void setFont(Font* __font);
	void setContentMargin(int __margin);
	int getContentMargin() const;

	String getReshapedString(const godot::String __data);

private:
	void inline pushAlphBytes(vector<uint8_t>& word,
							  uint16_t currCp,
							  uint8_t form);

	void renderAlphabets(Range& range, vector<uint8_t>& token);
	void renderNumbers(Range& range, vector<uint8_t>& token);
	void renderOthers(Range& range, vector<uint8_t>& token);
	void renderSpecials(Range& range, vector<uint8_t>& token);

	bool isDelimiter(uint16_t codepoint) const;
	void parseCharSet(Range& range, Rvector<uint8_t>& line);

private:
	enum CodepointType {
		cpALPHABET = 1,
		cpNUMBER,
		cpSPECIAL,
		cpOTHER,
		cpINVALID
	};

	CodepointType getCodepointType(uint16_t cp);

private:
	// int resultSize;
	const uint8_t cNEWLINE = 0x0a;
	const uint8_t cSPACE = 0x20;
	const uint8_t cTAB = 0x09;

	Farsi::AlphabetDict& m_dict;
	shared_ptr<map<uint16_t, vector<uint8_t>>> m_alph;
	shared_ptr<map<uint16_t, vector<uint8_t>>> m_spec;
	shared_ptr<pair<uint8_t, uint8_t>> m_other;
	shared_ptr<map<uint16_t, uint8_t>> m_nums;
	ResourceLoader* m_rcLoader;
	Ref<Font> m_font;
	String m_frStr;
	vector<uint8_t> m_result;

	bool m_autoWrap;
	long m_maxLineWidth;
	long m_currentLineWidth;
	int m_contentMargin;
};

#endif // RESHAPER_HPP
