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
	// default constructor
	Note() {}

	Note(unsigned int pDuration, unsigned int pVoice, PitchType pPitchType, unsigned int pStaff = 1)
					: duration(pDuration), voice(pVoice), type(pPitchType), staff(pStaff) {}

	Note(unsigned int pDuration, unsigned int pVoice, unsigned int pStaff = 1)
					: duration(pDuration), voice(pVoice), staff(pStaff) {
		setTypeFromDuration(pDuration);
	}

	void setTypeFromDuration(unsigned int pDuration) {
		// under the assumption that
		// duration of 1 -- 16th note
		// duration of 2 -- 8th note
		// so forth...
		switch (pDuration) {
			default:
			case 1:
				type = SIXTEENTH;
				break;
			case 2:
				type = EIGHTH;
				break;
			case 4:
				type = QUARTER;
				break;
			case 8:
				type = HALF;
				break;
			case 16:
				type = WHOLE;
				break;
		}
	}

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
	unsigned int voice = 1;
	PitchType type;
	unsigned int staff = 1;
	vector<Beam> beams;
};

class Pitch : public Note {
public:
	Pitch() {}

	Pitch(unsigned int pDuration, Step pStep, unsigned int pOctave, int pAlter = 0, PitchAccidental pPitchAccidental = NONE)
					: step(pStep), octave(pOctave), alter(pAlter), accidental(pPitchAccidental) {
		duration = pDuration;
		setTypeFromDuration(pDuration);
	}

	XMLElement* makeSubXMLElement() const {
		XMLElement* thisElement = document.NewElement("pitch");

		XMLElement* stepElement = document.NewElement("step");
		stepElement->SetText(toString(step));
		thisElement->InsertEndChild(stepElement);

		XMLElement* alterElement = alter != 0 ? document.NewElement("alter") : NULL;
		if (alterElement != NULL) {
			alterElement->SetText(alter);
			thisElement->InsertEndChild(alterElement);
		}

		XMLElement* octaveElement = document.NewElement("octave");
		octaveElement->SetText(octave);
		thisElement->InsertEndChild(octaveElement);

		XMLElement* accidentalElement = accidental != NONE ? document.NewElement("accidental") : NULL;
		if (accidentalElement != NULL) {
			accidentalElement->SetText(toString(accidental));
			thisElement->InsertEndChild(accidentalElement);
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
	Rest() {}
	Rest(unsigned int pDuration, Step pDisplayStep = B, unsigned int pDisplayOctave = 4)
					: displayStep(pDisplayStep), displayOctave(pDisplayOctave){
		duration = pDuration;
		setTypeFromDuration(pDuration);
	}

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
		staveElement->SetText((int)clefs.size()); // cast to int, since *some* compilers don't like it...

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
	void addNotes(Note* pNotes, unsigned int size) {
		for (unsigned int i = 0; i < size; i++) {
			notes.push_back(&pNotes[i]);
		}
	}

	void addNotes(vector<Note*> pNotes) {
		notes.insert(notes.end(), pNotes.begin(), pNotes.end());
	}

	unsigned int measureNumber;
	MeasureAttributes measureAttributes;
	vector<Note*> notes;
};

struct ScorePart {
		string _scorePartId;
		string partName;
		string _scoreInstrumentId;
		string instrumentName;
		string _midiInstrumentId;
		unsigned int midiChannel;
		unsigned int midiProgram;
		unsigned int volume;
		unsigned int pan;
};

class Part : public XML_Able {
public:
	XMLElement* makeXMLElement() const {
		// go 'up' a level and define the file wide nodes
		XMLElement* masterElement = document.NewElement("score-partwise");
		masterElement->SetAttribute("version", "3.0");

		// make all the nodes first
		XMLElement* partListElement = document.NewElement("part-list");
			XMLElement* scorePartElement = document.NewElement("score-part");
				XMLElement* partNameElement = document.NewElement("part-name");
				XMLElement* scoreInstrumentElement = document.NewElement("score-instrument");
					XMLElement* instrumentNameElement = document.NewElement("instrument-name");
				XMLElement* midiInstrumentElement = document.NewElement("midi-instrument");
					XMLElement* midiChannelElement = document.NewElement("midi-channel");
					XMLElement* midiProgramElement = document.NewElement("midi-program");
					XMLElement* volumeElement = document.NewElement("volume");
					XMLElement* panElement = document.NewElement("pan");


		// assign over all the values
		scorePartElement->SetAttribute("id", scorePart._scorePartId.c_str());
		partNameElement->SetText(scorePart.partName.c_str());
		scoreInstrumentElement->SetAttribute("id", scorePart._scoreInstrumentId.c_str());
		instrumentNameElement->SetText(scorePart.instrumentName.c_str());
		midiInstrumentElement->SetAttribute("id", scorePart._midiInstrumentId.c_str());
		midiChannelElement->SetText(scorePart.midiChannel);
		midiProgramElement->SetText(scorePart.midiProgram);
		volumeElement->SetText(scorePart.volume);
		panElement->SetText(scorePart.pan);

		// all the linking up (bottom up)
		midiInstrumentElement->InsertEndChild(midiChannelElement);
		midiInstrumentElement->InsertEndChild(midiProgramElement);
		midiInstrumentElement->InsertEndChild(volumeElement);
		midiInstrumentElement->InsertEndChild(panElement);

		scoreInstrumentElement->InsertEndChild(instrumentNameElement);

		scorePartElement->InsertEndChild(partNameElement);
		scorePartElement->InsertEndChild(scoreInstrumentElement);
		scorePartElement->InsertEndChild((midiInstrumentElement));

		partListElement->InsertEndChild(scorePartElement);

		XMLElement* thisElement = document.NewElement("part");
		thisElement->SetAttribute("id", scorePart._scorePartId.c_str());
		for (Measure pMeasure : measures) {
			thisElement->InsertEndChild(pMeasure.makeXMLElement());
		}

		masterElement->InsertEndChild(partListElement);
		masterElement->InsertEndChild(thisElement);

		return masterElement;
	}
	ScorePart scorePart;
	vector<Measure> measures;

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

	vector<Note*> passage;
	passage.push_back(new Rest(2));
	passage.push_back(new Pitch(1,C,5));
	passage.push_back(new Pitch(1,B,4));
	passage.push_back(new Pitch(2,C,5));
	passage.push_back(new Pitch(2,G,4));
	passage.push_back(new Pitch(2,A,4,-1)); // Non accidental A-Flat
	passage.push_back(new Pitch(1,C,5));
	passage.push_back(new Pitch(1,B,4));
	passage.push_back(new Pitch(2,C,5));
	passage.push_back(new Pitch(2,D,5));

	Measure someMeasure;
	someMeasure.measureNumber = 1;
	someMeasure.addNotes(passage);
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

	Part somePart;
	somePart.measures.push_back(someMeasure);
	somePart.scorePart._scorePartId = "P1";
	somePart.scorePart.partName = "Piano";
	somePart.scorePart._scoreInstrumentId = "P1-I1";
	somePart.scorePart.instrumentName = "Piano";
	somePart.scorePart._midiInstrumentId = "P1-I1";
	somePart.scorePart.midiChannel = 1;
	somePart.scorePart.midiProgram = 1;
	somePart.scorePart.volume = 79;
	somePart.scorePart.pan = 0;

	XMLDeclaration* startingDeclaration = document.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"");
	XMLNode* xmlText = document.NewUnknown("DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 3.0 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\"");

	document.InsertEndChild(startingDeclaration);
	document.InsertEndChild(xmlText);
	document.InsertEndChild(somePart.makeXMLElement());

	document.SaveFile("TestingFile.xml");

	return 0;
}

#endif