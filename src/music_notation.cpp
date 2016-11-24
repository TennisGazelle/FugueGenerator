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

	void add_notes(const Note* pNotes, unsigned int size) {
		Measure measure;
		float count = 0.0;
		for (unsigned int i = 0; i < size; i++) {
			count += pNotes[i].beat_factor;
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
		//fout.open(outFileName.c_str());
		
		cout << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << endl <<
		"<!DOCTYPE score-partwise PUBLIC" << endl << 
		"   \"-//Recordare//DTD MusicXML 3.0 Partwise//EN\"" << endl <<
		"   \"http://www.musicxml.org/dtds/partwise.dtd\">" << endl;

    	cout << "<score-partwise version=\"3.0\">" << endl <<
  					"<part-list>" << endl <<
	    				"<score-part id=\"P1\">" << endl <<
						    "<part-name>" << "MUSIC" << "</part-name>" << endl <<
						"</score-part>" << endl <<
					"</part-list>" << endl;
		cout << "<part id=\"P1\">";

		for (unsigned int i = 0; i < measures.size(); i++) {
			cout << "<measure number=\"" << i+1 << "\">" << endl;
			if (i == 0) {
				cout << "<attributes>" << endl <<
			        "<divisions>1</divisions>" << endl <<
			        "<key>" << endl <<
			          "<fifths>0</fifths>" << endl <<
			        "</key>" << endl <<
			        "<time>" << endl <<
			          "<beats>4</beats>" << endl <<
			          "<beat-type>4</beat-type>" << endl <<
			        "</time>" << endl <<
			        "<clef>" << endl <<
			          "<sign>G</sign>" << endl <<
			          "<line>2</line>" << endl <<
			        "</clef>" << endl <<
			      "</attributes>" << endl;
			}
		    cout << "<note>" << endl <<
		        "<pitch>" << endl <<
		        	"<step>C</step>" << endl <<
		        	"<octave>4</octave>" << endl <<
		        "</pitch>" << endl <<
		        "<duration>4</duration>" << endl <<
		        "<type>whole</type>" << endl <<
		    "</note>" << endl <<
		    "</measure>" << endl;
		}

		cout << "</part>" << endl;
		cout << "</score-partwise>" << endl;


		fout.close();
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
	testPassage.outputToFile("");


	return 0
;}
#endif