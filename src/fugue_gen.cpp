#ifndef __FUGUE_GEN_
#define __FUGUE_GEN_

#include <vector>

using namespace std;

enum Step {
	C=0,
	D,
	E,
	F,
	G,
	A,
	B,
};

enum PitchType {
	WHOLE,
	HALF,
	QUARTER,
	EIGHTH,
	SIXTEENTH, // needs to be written as '16th'
};

enum PitchAccidental {
	DOUBLE_SHARP,
	SHARP,
	NATURAL,
	FLAT,
	DOUBLE_FLAT,
	NONE
};

enum KeyMode {
	MAJOR,
	MINOR
};

enum Sign {
	G_TREBLE, // treble clef
	C_ALTO, // alto clef
	F_BASS  // bass clef
};

struct Beam {
	enum State { BEGIN, END } state;
	unsigned int id;
};

class XML_Able {
public:
	// some function that returns a xml node type
	// for tinyxml that can be "appended" to one 
	// another for quick xml writing


};

class Note {
public:
	unsigned int duration;
	unsigned int voice;
	PitchType type;
	unsigned int staff;
	vector<Beam> beams;
};

class Pitch : public Note {
public:
	Step step;
	unsigned int octave;
	PitchAccidental accidental;
};

class Rest : public Note {
public:
	Step displayStep;
	unsigned int displayOctave;
};

struct Key {
	int fifths;
	KeyMode keyMode;
};

struct Time {
	unsigned int beats;
	unsigned int beat_type;

};

struct Clef {
	Sign sign;
	unsigned int number;
};

struct MeasureAttributes {
	unsigned int divisions = 4;
	Key key;
	Time time;
	unsigned int staves = 3;
	vector<Clef> clefs;
};

class Measure {
	MeasureAttributes measureAttributes;
	vector<Note> notes;
};

int main(int argc, char const *argv[]) {
	
	return 0;
}

#endif