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

inline const char* toString(KeyMode pKM) {
	switch (pKM) {
		case MAJOR:
			return "Major";
		case MINOR:
			return "Minor";
		default:
			return "UnKnownKeyModeType";
	}
}

enum Sign {
	G_TREBLE, // treble clef
	C_ALTO, // alto clef
	F_BASS  // bass clef
};

enum State { BEGIN, END };

inline const char* toString(State pState) {
	switch (pState) {
		case BEGIN:
			return "begin";
		case END:
			return "end";
		default:
			return "UnKnownBeamState";
	}
}

struct Beam {
	State state;
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
		for (Beam pBeam : beams) {
			XMLElement* beamElement = document.NewElement("beam");
			beamElement->SetAttribute("number", pBeam.id);
			beamElement->SetText(toString(pBeam.state));
			thisElement->InsertEndChild(beamElement);
		}

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
		XMLElement* thisElement = document.NewElement("rest");

		XMLElement* stepElement = document.NewElement("display-step");
		stepElement->SetText(toString(displayStep));
		thisElement->InsertEndChild(stepElement);

		XMLElement* octaveElement = document.NewElement("display-octave");
		octaveElement->SetText(displayOctave);
		thisElement->InsertEndChild(octaveElement);

		return thisElement;
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
	string symbol;
};

struct Clef {
	Sign sign;
	unsigned int number;
};

struct MeasureAttributes : public XML_Able {
public:

	XMLElement* makeXMLElement() const {
		XMLElement* thisElement = document.NewElement("attributes");

		XMLElement* divElement = document.NewElement("divisions");
		divElement->SetText(divisions);

		// take care of the key node first
		XMLElement* keyElement = document.NewElement("key");
		XMLElement* fifthsElement = document.NewElement("fifths");
		XMLElement* modeElement = document.NewElement("mode");
		fifthsElement->SetText(key.fifths);
		modeElement->SetText(toString(key.keyMode));
		keyElement->InsertEndChild(fifthsElement);
		keyElement->InsertEndChild(modeElement);

		// take care of time node
		XMLElement* timeElement = document.NewElement("time");
		XMLElement* beatElement = document.NewElement("beats");
		XMLElement* beatTypeElement = document.NewElement("beat-type");
		beatElement->SetText(time.beats);
		beatTypeElement->SetText(time.beat_type);
		timeElement->SetAttribute("symbol", time.symbol.c_str());
		timeElement->InsertEndChild(beatElement);
		timeElement->InsertEndChild(beatTypeElement);

		XMLElement* staveElement = document.NewElement("staves");
		staveElement->SetText(clefs.size());

		// take care of the clef node
		for (Clef pClef : clefs) {
			// TODO: take care of this for all clefs (look at beams TODO)
		}

		// finally add them into this element
		thisElement->InsertEndChild(divElement);
		thisElement->InsertEndChild(keyElement);
		thisElement->InsertEndChild(timeElement);
		thisElement->InsertEndChild(staveElement);

		return thisElement;
	}

	unsigned int divisions = 4;
	Key key;
	Time time;
	unsigned int staves = 3; // this should be the save as clefs.size()
	vector<Clef> clefs;
};

class Measure : public XML_Able {
public:

	XMLElement* makeXMLElement() const {
		XMLElement* thisElement = document.NewElement("measure");
		thisElement->SetAttribute("number", measureNumber);

		for (unsigned int i = 0; i < notes.size(); i++) {
			if (i == 0) {
				thisElement->InsertEndChild(measureAttributes.makeXMLElement());
			}
			thisElement->InsertEndChild(notes[i]->makeXMLElement());
		}
		return thisElement;
	}

	unsigned int measureNumber;
	MeasureAttributes measureAttributes;
	vector<Note*> notes;
};

int main(int argc, char const *argv[]) {
	Pitch* somePitch = new Pitch;
	Note* someNote = somePitch;

	somePitch->step = C;
	somePitch->octave = 4;
	somePitch->alter = -1;
	somePitch->accidental = NONE;
	somePitch->duration = 1;
	somePitch->type = SIXTEENTH;
	somePitch->staff = 1;

	Measure someMeasure;
	someMeasure.measureNumber = 1;
	someMeasure.notes.push_back(someNote);
	someMeasure.measureAttributes.divisions = 4;
	someMeasure.measureAttributes.key.fifths = 0;
	someMeasure.measureAttributes.key.keyMode = MAJOR;
	someMeasure.measureAttributes.time.beats = 4;
	someMeasure.measureAttributes.time.beat_type = 4;
	someMeasure.measureAttributes.time.symbol = "common";
	someMeasure.measureAttributes.staves = 1;
	Clef someClef;
	someClef.number = 1;
	someClef.sign = G_TREBLE;
	someMeasure.measureAttributes.clefs.push_back(someClef);
	cout << "def of some measure" << endl;

	document.InsertEndChild(someMeasure.makeXMLElement());

	document.SaveFile("TestingFile.xml");

	return 0;
}

#endif