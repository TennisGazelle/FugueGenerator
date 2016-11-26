#ifndef __MUSIC_NOTATION_
#define __MUSIC_NOTATION_

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

#include "rapidxml/rapidxml.hpp"

using namespace std;

enum Pitch {
	C=0, C_SHARP,
	D, D_SHARP,
	E,
	F, F_SHARP,
	G, G_SHARP,
	A, A_SHARP,
	B,
	REST
};

const char * PitchEnumStrings[] = {
	"C", "C_SHARP",
	"D", "D_SHARP",
	"E",
	"F", "F_SHARP",
	"G", "G_SHARP",
	"A", "A_SHARP",
	"B",
	"REST"
};

const char * getPitchString(int enumVal) {
	return PitchEnumStrings[enumVal];
}

string getXMLPitchString(int enumVal) {
	switch (enumVal) {
		case 1:
		case 3:
		case 6:
		case 8:
		case 10:
			return string("<step>") + string(PitchEnumStrings[enumVal-1]) + string("</step>\n<alter>1</alter>");
		default:
			return string("<step>") + string(PitchEnumStrings[enumVal]) + string("</step>");
	}
}

enum NoteType{
	WHOLE = 0,
	HALF,
	QUARTER,
	EIGHTH,
	SIXTEENTH
};

const char * NoteTypeEnumStrings[] = {
	"WHOLE",
	"HALF",
	"QUARTER",
	"EIGHTH",
	"16TH"	
};

string getNoteTypeString(int enumVal) {
	return NoteTypeEnumStrings[enumVal];
}

string getNoteTypeStringForBeatFactor(float beat_factor) {
	if (beat_factor == 4)
		return getNoteTypeString(0);
	else if (beat_factor == 2)
		return getNoteTypeString(1);
	else if (beat_factor == 1)
		return getNoteTypeString(2);
	else if (beat_factor == 0.5f)
		return getNoteTypeString(3);
	else return getNoteTypeString(4);
}

const Pitch C_FLAT = B;
const Pitch D_FLAT = C_SHARP;
const Pitch E_FLAT = D_SHARP;
const Pitch F_FLAT = E;
const Pitch G_FLAT = F_SHARP;
const Pitch A_FLAT = G_SHARP;
const Pitch B_FLAT = A_SHARP;

enum Octave : unsigned int {
	octave0 = 0, 
	octave1, 
	octave2, 
	octave3, 
	octave4, 
	octave5, 
	octave6, 
	octave7
};

struct Note {
	Pitch pitch;
	Octave octave;
	float beat_factor;
};

struct Measure {
	std::vector<Note> notes;

	float getLength() const {
		float count = 0.0f;
		for (unsigned int i = 0 ; i < notes.size(); i++) {
			count += notes[i].beat_factor;
		}
		return count;
	}
};

class Passage {
public:
	Passage(int pTop = 4, int pBottom = 4) {
		setTimeSignature(pTop, pBottom);
	}

	~Passage() {

	}

	void add_note(const Note& pNote) {
		// initialize it if it's the first note in the passage
		if (measures.size() == 0) {
			Measure measure;
			measure.notes.push_back(pNote);
			measures.push_back(measure);

		// append to the end of the last measure if it fits
		} else if (measures[measures.size()-1].getLength() < pNote.beat_factor) {
			measures[measures.size()-1].notes.push_back(pNote);
		
		// put the 'half' that fits back there, then add the rest in a new measure
		} else {

		}
	}

	void add_notes(Note* pNotes, unsigned int size) {
		Measure measure;
		float count = 0.0;
		for (unsigned int i = 0; i < size; i++) {
			count += pNotes[i].beat_factor;
			pNotes[i].octave = (Octave)(pNotes[i].octave+3);
			measure.notes.push_back(pNotes[i]);

			if ((int)count == time_signature.first) {
				count = 0;
				measures.push_back(measure);
				measure.notes.clear();
			}
		}
	}

	void add_measure(const Measure& pMeasure) {

	}

	void setTimeSignature(int pTop, int pBottom) {
		time_signature = std::pair<int, int>(pTop, pBottom);
	}

	void print() const {
		cout << "Time Signature: " << time_signature.first << " " << time_signature.second << endl;
		for (unsigned int i = 0; i < measures.size(); i++) {
			cout << "Measure " << i << ": " << endl;
			for (unsigned int j = 0; j < measures[i].notes.size(); j++) {
				cout << getPitchString(measures[i].notes[j].pitch) << " " << measures[i].notes[j].beat_factor << endl;
			}
			cout << endl;
		}
	}

	void outputToFile(const string& outFileName) const {
		ofstream fout;
		fout.open(outFileName.c_str());
		
		fout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl <<
		"<!DOCTYPE score-partwise PUBLIC" << endl << 
		"   \"-//Recordare//DTD MusicXML 3.0 Partwise//EN\"" << endl <<
		"   \"http://www.musicxml.org/dtds/partwise.dtd\">" << endl;

    	fout << "<score-partwise version=\"3.0\">" << endl <<
  					"<part-list>" << endl <<
	    				"<score-part id=\"P1\">" << endl <<
						    "<part-name>" << "MUSIC" << "</part-name>" << endl <<
						"</score-part>" << endl <<
					"</part-list>" << endl;
		fout << "<part id=\"P1\">" << endl;

		for (unsigned int i = 0; i < measures.size(); i++) {
			fout << "<measure number=\"" << i+1 << "\">" << endl;
			if (i == 0) {
				fout << "<attributes>" << endl <<
			        "<divisions>1</divisions>" << endl <<
			        "<key>" << endl <<
			          "<fifths>0</fifths>" << endl <<
			        "</key>" << endl <<
			        "<time>" << endl <<
			          "<beats>3</beats>" << endl <<
			          "<beat-type>4</beat-type>" << endl <<
			        "</time>" << endl <<
			        "<clef>" << endl <<
			          "<sign>G</sign>" << endl <<
			          "<line>2</line>" << endl <<
			        "</clef>" << endl <<
			      "</attributes>" << endl;
			}

			for (unsigned int j = 0; j < measures[i].notes.size(); j++) {
				if (measures[i].notes[j].pitch != REST) {
				    fout << "<note>" << endl <<
				        "<pitch>" << endl <<
				        	getXMLPitchString(measures[i].notes[j].pitch) << endl <<
				        	"<octave>" << measures[i].notes[j].octave << "</octave>" << endl <<
				        "</pitch>" << endl <<
				        "<duration>" << measures[i].notes[j].beat_factor * 2 << "</duration>" << endl <<
				        "<type>" << getNoteTypeStringForBeatFactor(measures[i].notes[j].beat_factor) << "</type>" << endl <<
				    "</note>" << endl;
				} else {
					fout << "<note>" << endl <<
				        "<rest/>" << endl << 
				        "<duration>" << measures[i].notes[j].beat_factor * 2 << "</duration>" << endl <<
				        "<type>" << getNoteTypeStringForBeatFactor(measures[i].notes[j].beat_factor) << "</type>" << endl <<
				    "</note>" << endl;
				}
			}
			fout << "</measure>" << endl;

		}

		fout << "</part>" << endl;
		fout << "</score-partwise>" << endl;


		fout.close();
	}

	void outputToFileUsingTiny(const string& outFileName) const {
		
	}

private:
	pair<int, int> time_signature;
	Pitch key;
	vector<Measure> measures;
};



int main(int argc, char const *argv[]) {
	// write the container
	Note tune[] = {
		{ REST, (Octave)0, 2.0 },
		{ G, (Octave)1, .5f },
		{ C, (Octave)2, .5f },

		{ E, (Octave)2, 1.5f },		
		{ E, (Octave)2, .5f },
		{ D, (Octave)2, .5f },
		{ C, (Octave)2, .5f },

		{ E, (Octave)2, 1.5f },
		{ E, (Octave)2, .5f },
		{ D, (Octave)2, .5f },
		{ E, (Octave)2, .5f },

		{ F, (Octave)2, 1.5f },
		{ F, (Octave)2, .5f },
		{ E, (Octave)2, .5f },
		{ F, (Octave)2, .5f },

		{ G, (Octave)2, 2.0f },
		{ C, (Octave)2, .5f },
		{ C, (Octave)2, .5f },

		{ A, (Octave)2, 1.5f },
		{ A, (Octave)2, .5f },
		{ G, (Octave)2, .5f },
		{ F, (Octave)2, .5f },

		{ G, (Octave)2, 1.5f },
		{ G, (Octave)2, .5f },
		{ F, (Octave)2, .5f },
		{ E, (Octave)2, .5f },

		{ E, (Octave)2, 1.0f },
		{ F, (Octave)2, 1.0f },
		{ B, (Octave)1, 1.0f },

		{ C, (Octave)2, 3.0f }
	};

	Passage testPassage(3,4);

	testPassage.add_notes(tune, 30);

	testPassage.print();
	testPassage.outputToFile("../bin/testoutput.xml");


	return 0
;}
#endif