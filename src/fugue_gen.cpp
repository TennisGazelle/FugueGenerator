#ifndef __FUGUE_GEN_
#define __FUGUE_GEN_

#include <vector>
#include <iostream>
#include <stdlib.h>
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

inline Step fromStringToStep(const char * pStepAsString) {
	string pSAS = string(pStepAsString);
	if (pSAS == "C" || pSAS == "c")
		return C;
	else if (pSAS == "D" || pSAS == "d")
		return D;
	else if (pSAS == "E" || pSAS == "e")
		return E;
	else if (pSAS == "F" || pSAS == "f")
		return F;
	else if (pSAS == "G" || pSAS == "g")
		return G;
	else if (pSAS == "A" || pSAS == "a")
		return A;
	else if (pSAS == "B" || pSAS == "b")
		return B;
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

inline PitchType fromStringToPitchType(const char * pPitchTypeAsString) {
	string pPTAS = string(pPitchTypeAsString);
	if (pPTAS == "whole")
		return WHOLE;
	else if (pPTAS == "half")
		return HALF;
	else if (pPTAS == "quarter")
		return QUARTER;
	else if (pPTAS == "eighth")
		return EIGHTH;
	else if (pPTAS == "16th")
		return SIXTEENTH;
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
			return "sharp";
		case NATURAL:
			return "natural";
		case FLAT:
			return "flat";
		case DOUBLE_FLAT:
			return "DOUBLE_FLAT";
		default:
			return "UnKnownPitchAccidentalType";
	}
}

inline PitchAccidental fromStringToPitchAccidental(const char * pPitchAccidental) {
	string pPAString = string(pPitchAccidental);
	if (pPAString == "natural")
		return NATURAL;
	else if (pPAString == "sharp")
		return SHARP;
	else if (pPAString == "flat")
		return FLAT;
	else
		return NATURAL;
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

	virtual void print() const {}
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
		if (isChord){
			XMLElement* chordElement = document.NewElement("chord");
			thisElement->InsertEndChild(chordElement);
		}
		thisElement->SetAttribute("default-x", _default_x);
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

		XMLElement* accidentalElement = accidental != NONE ? document.NewElement("accidental") : NULL;
		if (accidentalElement != NULL) {
			accidentalElement->SetText(toString(accidental));
			thisElement->InsertEndChild(accidentalElement);
		}

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

	virtual void print() const = 0;

	unsigned int _default_x = 0;
	bool isChord = false;
	unsigned int duration;
	unsigned int voice = 1;
	PitchType type;
	unsigned int staff = 1;
	PitchAccidental accidental = NONE;
	vector<Beam> beams;
};

class Pitch : public Note {
public:
	Pitch() {}

	Pitch(unsigned int pDuration, Step pStep, unsigned int pOctave, int pAlter = 0, PitchAccidental pPitchAccidental = NONE)
					: step(pStep), octave(pOctave), alter(pAlter) {
		duration = pDuration;
		accidental = pPitchAccidental;
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

		return thisElement;
	}

	void print() const {
		cout << toString(step) << octave << "(" << alter << ")" << " - " << duration << endl;
	}

	Step step;
	unsigned int octave;
	int alter = 0;
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

	void print() const {
		cout << "REST - " << duration << endl;
	}

	Step displayStep;
	unsigned int displayOctave;
};

class RestMeasure : public Note {
public:
	RestMeasure() {}
	RestMeasure(unsigned int pDuration, unsigned int pVoice, unsigned int pStaff) {
		duration = pDuration;
		voice = pVoice;
		staff = pStaff;
	}

	XMLElement* makeSubXMLElement() const {
		XMLElement* thisElement = document.NewElement("rest");
		thisElement->SetAttribute("measure", "yes");
		return thisElement;
	}

	void print() const {
		cout << "REST (Measure; voice: " << voice << ")" << endl;
	}
};

class Barline : public XML_Able {
public:
	Barline() {
		location = "right";
		bar_style = "light-heavy";
	}

	XMLElement* makeXMLElement() const {
		XMLElement* thisElement = document.NewElement("barline");
		thisElement->SetAttribute("location", location.c_str());

		XMLElement* barStyleElement = document.NewElement("bar-style");
		barStyleElement->SetText(bar_style.c_str());

		thisElement->InsertEndChild(barStyleElement);

		return thisElement;
	}

	string location;
	string bar_style;

};

struct Key {
	int fifths;
	KeyMode keyMode;
};

enum TimeSymbol {
		COMMON,
		CUT,
		SINGLE_NUMBER, // "single-number"
		NOTE,
		DOTTED_NOTE, // "dotted-note"
		NORMAL
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

class MeasureAttributes : public XML_Able {
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
			if (i == 0 && measureNumber == 1) {
				thisElement->InsertEndChild(measureAttributes.makeXMLElement());
			}
			thisElement->InsertEndChild(notes[i]->makeXMLElement());
		}

		if (barline != NULL) {
			thisElement->InsertEndChild(barline->makeXMLElement());
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

	void print() const {
		cout << "======================================" << endl;
		cout << "Measure number: " << measureNumber << endl;
		cout << "Notes: " << endl;
		for (unsigned int i = 0; i < notes.size(); i++) {
			notes[i]->print();
		}

	}

	unsigned int _width = 0;
	unsigned int measureNumber;
	MeasureAttributes measureAttributes;
	vector<Note*> notes;
	Barline* barline = NULL;
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
		for (unsigned int i = 0; i < measures.size(); i++) {
			thisElement->InsertEndChild(measures[i].makeXMLElement());
		}

		masterElement->InsertEndChild(partListElement);
		masterElement->InsertEndChild(thisElement);

		return masterElement;
	}
	ScorePart scorePart;
	vector<Measure> measures;

};


Part loadDocumentIntoProjectFormat(const string& filename) {
	// helper variable
	bool chordFlag = false;

	// load the file
	XMLDocument pDoc;
	pDoc.LoadFile(filename.c_str());

	// load the measure information
	Part specificPart;
	XMLElement* scorePartList = pDoc.FirstChildElement("score-partwise")->FirstChildElement("part-list")->FirstChildElement("score-part");

	specificPart.scorePart._scorePartId = string(scorePartList->Attribute("id"));
	specificPart.scorePart.partName = string(scorePartList->FirstChildElement("part-name")->GetText());
	specificPart.scorePart._scoreInstrumentId = string(scorePartList->FirstChildElement("score-instrument")->Attribute("id"));
	specificPart.scorePart.instrumentName = string(scorePartList->FirstChildElement("score-instrument")->FirstChildElement("instrument-name")->GetText());
	specificPart.scorePart._midiInstrumentId = specificPart.scorePart._scoreInstrumentId;
	specificPart.scorePart.midiChannel = 1;
	specificPart.scorePart.midiProgram = 1;
	specificPart.scorePart.volume = 79;
	specificPart.scorePart.pan = 0;

	XMLElement* fMeasureList = pDoc.FirstChildElement("score-partwise")->FirstChildElement("part");

	XMLElement* measureElement = fMeasureList->FirstChild()->ToElement();
	while (measureElement != fMeasureList->LastChild()) {

		Measure currentMeasure;
		currentMeasure.measureNumber = stoul(measureElement->Attribute("number"));

		// go through the notes
		XMLNode* noteNode;
		if (currentMeasure.measureNumber == 1) {
			noteNode = measureElement->FirstChild()->NextSibling()->NextSibling()->NextSibling();
		} else {
			noteNode = measureElement->FirstChild();
		}
		while (noteNode != measureElement->LastChild()) {
			if (string(noteNode->FirstChild()->ToElement()->Value()) == "rest") {
				Rest* currentRest = new Rest;

				if (noteNode->ToElement()->Attribute("default-x") != NULL) {
					currentRest->_default_x = stoul(noteNode->ToElement()->Attribute("default-x"));
				}

				if (noteNode->FirstChildElement()->Attribute("measure") != NULL) {
					currentMeasure.addNotes(new RestMeasure(currentRest->duration, currentRest->voice, currentRest->staff), 1);
					delete currentRest;
				} else {
					// error checking for rest children, "display-step"
					if (noteNode->FirstChild()->FirstChildElement("display-step") != NULL) {
						currentRest->displayStep = fromStringToStep(noteNode->FirstChild()->FirstChildElement("display-step")->GetText());
					} else {
						currentRest->displayStep = C;
					}
					// error checking for other rest child "display-octave"
					if (noteNode->FirstChild()->FirstChildElement("display-octave") != NULL) {
						currentRest->displayOctave = stoul(noteNode->FirstChild()->FirstChildElement("display-octave")->GetText());
					} else {
						currentRest->displayOctave = 4;
					}

					// cast the earlier pointer to that
					currentRest->duration = stoul(noteNode->FirstChildElement("duration")->GetText());
					currentRest->voice = stoul(noteNode->FirstChildElement("voice")->GetText());
					currentRest->staff = stoul(noteNode->FirstChildElement("staff")->GetText());
					currentRest->type = fromStringToPitchType(noteNode->FirstChildElement("type")->GetText());

					//save this note into memory
					currentMeasure.addNotes(currentRest, 1);
					//go to the next node in the measure
					noteNode = noteNode->NextSibling();
				}
			} else if (string(noteNode->FirstChild()->ToElement()->Value()) == "pitch") {
				Pitch* currentPitch = new Pitch;

				if (noteNode->ToElement()->Attribute("default-x") != NULL) {
					currentPitch->_default_x = stoul(noteNode->ToElement()->Attribute("default-x"));
				}

				currentPitch->step = fromStringToStep(noteNode->FirstChild()->FirstChildElement("step")->GetText());
				currentPitch->octave = stoul(noteNode->FirstChild()->FirstChildElement("octave")->GetText());
				currentPitch->alter = 0;
				if (noteNode->FirstChild()->FirstChildElement("alter") != NULL) {
					currentPitch->alter = stoi(noteNode->FirstChild()->FirstChildElement("alter")->GetText());
				}
				if (noteNode->FirstChildElement("accidental") != NULL) {
					currentPitch->accidental = fromStringToPitchAccidental(noteNode->FirstChildElement("accidental")->GetText());
				}

				// cast the earlier pointer to that
				currentPitch->duration = stoul(noteNode->FirstChildElement("duration")->GetText());
				currentPitch->voice = stoul(noteNode->FirstChildElement("voice")->GetText());
				currentPitch->type = fromStringToPitchType(noteNode->FirstChildElement("type")->GetText());
				currentPitch->staff = stoul(noteNode->FirstChildElement("staff")->GetText());
				currentPitch->isChord = chordFlag;
				chordFlag = false;

				//save this note into memory
				currentMeasure.addNotes(currentPitch, 1);
				//go to the next node in the measure
				noteNode = noteNode->NextSibling();

			} else if (string(noteNode->FirstChild()->ToElement()->Value()) == "chord") {
				cout << "This was note was part of a chord " << endl;
				chordFlag = true;
				noteNode->DeleteChild(noteNode->FirstChild());
			}	else {
				cout << "I have no idea what I read inside the measure" << endl;
				cout << "It was this :" << string(noteNode->ToElement()->Value()) << endl;
				noteNode = noteNode->NextSibling();
			}
		}

		//save this measure in memory
		specificPart.measures.push_back(currentMeasure);

		// go onto the next measure
		measureElement = measureElement->NextSiblingElement();
	}

	// hardcode the time signature stuff for the first measure
	specificPart.measures[0].measureAttributes.divisions = 4;
	specificPart.measures[0].measureAttributes.key.fifths = -3;
	specificPart.measures[0].measureAttributes.key.keyMode = MAJOR;
	specificPart.measures[0].measureAttributes.time.beats = 4;
	specificPart.measures[0].measureAttributes.time.beat_type = 4;
	specificPart.measures[0].measureAttributes.time.symbol = "common";

	Clef someClef;
	someClef.number = 1;
	someClef.sign = G_TREBLE;
	specificPart.measures[0].measureAttributes.clefs.push_back(someClef);

	someClef.number = 2;
	someClef.sign = F_BASS;
	specificPart.measures[0].measureAttributes.clefs.push_back(someClef);
	return specificPart;
}

int main(int argc, char const *argv[]) {
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
	someMeasure.measureAttributes.key.fifths = -3;
	someMeasure.measureAttributes.key.keyMode = MAJOR;
	someMeasure.measureAttributes.time.beats = 4;
	someMeasure.measureAttributes.time.beat_type = 4;
	someMeasure.measureAttributes.time.symbol = "common";
	someMeasure.measureAttributes.staves = 1;
	someMeasure.barline = new Barline;
	Clef someClef;
	someClef.number = 1;
	someClef.sign = G_TREBLE;
	someMeasure.measureAttributes.clefs.push_back(someClef);

	someClef.number = 2;
	someClef.sign = F_BASS;
	someMeasure.measureAttributes.clefs.push_back(someClef);
	cout << "def of some measure" << endl;

	Part somePart = loadDocumentIntoProjectFormat("C:\\Users\\HP\\Dropbox\\UNR stuff\\CS - 491 - MMM\\FugueGenerator\\ref\\fugue_as_xml.xml");
	//somePart.measures.push_back(someMeasure);
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