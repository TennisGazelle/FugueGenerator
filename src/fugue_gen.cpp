#ifndef __FUGUE_GEN_
#define __FUGUE_GEN_

#include <vector>
#include <iostream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

XMLDocument document;

enum Step {
	C=0,
	D,
	E,
	F,
	G,
	A,
	B
};

inline const char* toString(Step pStep) {
	switch (pStep) {
		case C:
			return "C";
		case D:
			return "D";
		case E:
			return "E";
		case F:
			return "F";
		case G:
			return "G";
		case A:
			return "A";
		case B:
			return "B";
		default:
			return "UnKnownStepValue";
	}
}

enum PitchType {
	WHOLE,
	HALF,
	QUARTER,
	EIGHTH,
	SIXTEENTH // needs to be written as '16th'
};

inline const char* toString(PitchType pPT) {
	switch (pPT) {
		case WHOLE:
			return "whole";
		case HALF:
			return "half";
		case QUARTER:
			return "quarter";
		case EIGHTH:
			return "eighth";
		case SIXTEENTH:
			return "16th";
		default:
			return "UnKnownPitchTypeValue";
	}
}

enum PitchAccidental {
	DOUBLE_SHARP,
	SHARP,
	NATURAL,
	FLAT,
	DOUBLE_FLAT,
	NONE
};

inline const char* toString(PitchAccidental pPA) {
	switch (pPA) {
		case DOUBLE_SHARP:
			return "DOUBLE_SHARP";
		case SHARP:
			return "SHARP";
		case NATURAL:
			return "NATURAL";
		case FLAT:
			return "FLAT";
		case DOUBLE_FLAT:
			return "DOUBLE_FLAT";
		default:
			return "UnKnownPitchAccidentalType";
	}
}

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
	virtual XMLElement* makeXMLElement() const = 0;
};

class Note : public XML_Able {
public:

	virtual XMLElement* makeSubXMLElement() const = 0;
	XMLElement* makeXMLElement() const {
		XMLElement* thisElement = document.NewElement("note");
		thisElement->InsertEndChild(makeSubXMLElement());

		XMLElement* durationElement = document.NewElement("duration");
		durationElement->SetText(duration);
		thisElement->InsertEndChild(durationElement);

		XMLElement* voiceElement = document.NewElement("voice");
		voiceElement->SetText(voice);
		thisElement->InsertEndChild(voiceElement);

		XMLElement* pitchTypeElement = document.NewElement("type");
		pitchTypeElement->SetText(toString(type));
		thisElement->InsertEndChild(pitchTypeElement);

		XMLElement* staffElement = document.NewElement("staff");
		staffElement->SetText(staff);
		thisElement->InsertEndChild(staffElement);

		// TODO: complete this for the possible beams as well

		return thisElement;
	}

	unsigned int duration;
	unsigned int voice;
	PitchType type;
	unsigned int staff;
	vector<Beam> beams;
};

class Pitch : public Note {
public:

	XMLElement* makeSubXMLElement() const {
		XMLElement* thisElement = document.NewElement("pitch");

		XMLElement* stepElement = document.NewElement("step");
		stepElement->SetText(toString(step));
		thisElement->InsertEndChild(stepElement);

		XMLElement* octaveElement = document.NewElement("octave");
		octaveElement->SetText(octave);
		thisElement->InsertEndChild(octaveElement);

		XMLElement* accidentalElement = accidental != NONE ? document.NewElement("accidental") : NULL;
		if (accidentalElement != NULL) {
			accidentalElement->SetText(toString(accidental));
			thisElement->InsertEndChild(accidentalElement);
		}

		XMLElement* alterElement = alter != 0 ? document.NewElement("alter") : NULL;
		if (alterElement != NULL) {
			alterElement->SetText(alter);
			thisElement->InsertEndChild(alterElement);
		}

		return thisElement;
	}

	Step step;
	unsigned int octave;
	int alter;
	PitchAccidental accidental;
};

class Rest : public Note {
public:

	XMLElement* makeSubXMLElement() const {
		// TODO: Look at pitch, and complete it for the RESTS
	}

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

class Measure : public XML_Able {
public:

	XMLElement* makeXMLElement() const {
		return NULL;
	}

	MeasureAttributes measureAttributes;
	vector<Note> notes;
};

int main(int argc, char const *argv[]) {
//	XMLDocument doc;
//	XMLNode* node = doc.NewElement("RootNode");
//	XMLElement* element = doc.NewElement("ElementNode");
//	XMLText* text = doc.NewText("TextNode");
//
//	element->LinkEndChild(text);
//	element->SetAttribute("id", "0");
//	node->LinkEndChild(element);
//
//	doc.InsertEndChild(node);

	Pitch* somePitch = new Pitch;
	Note* someNote = somePitch;

	somePitch->step = C;
	somePitch->octave = 4;
	somePitch->alter = -1;
	somePitch->accidental = NONE;
	somePitch->duration = 1;
	somePitch->type = SIXTEENTH;
	somePitch->staff = 1;

	document.InsertEndChild(someNote->makeXMLElement());

	document.SaveFile("TestingFile.xml");
//
//	XMLElement someElement;
//	someElement.SetValue("RootNode", true);

	return 0;
}

#endif