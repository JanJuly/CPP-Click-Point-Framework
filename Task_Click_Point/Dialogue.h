#pragma once
#include <vector>
#include <string>
#include "rapidxml.hpp"
#define _CRT_SECURE_NO_WARNINGS

using namespace rapidxml;
using namespace std;

// class for storing data for a dialogue line
class DialogueLine {
public:
	// the character the lines belongs to
	string speaker;
	// the line text
	string text;
};

// class for storing dialogue data
class Dialogue
{
public:
	// the name of the dialogue, for indexation
	string name;
	// the lines of the dialogue
	vector<DialogueLine> lines;
	int currentLine = 0;


};

// function to populate a list of Dialogues from an XML file
vector<Dialogue> ReadXmlDialogue() {
	FILE *pFile = fopen("Dialogue.xml", "rt");
	if (pFile != NULL) {
		fseek(pFile, 0, SEEK_END);
		long docLength = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		char *pBuffer = new char[docLength];

		memset(pBuffer, 0, docLength);
		fread(pBuffer, docLength, 1, pFile);
		fclose(pFile);

		xml_document<> doc;
		doc.parse<0>(pBuffer);

		//create temporary list to populate and return
		vector<Dialogue> temp;

		// get the main xml node
		xml_node<> *dataNode = doc.first_node();
		// define DialogueLine nodes
		xml_node<> *dialogueLine = dataNode->first_node("DialogueLine");
		//for each dialogueLine in the xml
		do {
			// get the name of dialogue the line belongs to
			string dialogueName = dialogueLine->first_node("dialogueName")->value();
			bool dialogueExists = false;
			// check if that dialogue already exists
			for (int i = 0; i < temp.size(); i++) {
				if (temp[i].name == dialogueName)
					dialogueExists = true;
			}
			// create line
			DialogueLine l;
			l.speaker = dialogueLine->first_node("speaker")->value();
			l.text = dialogueLine->first_node("line")->value();
			// if dialogue already exists, add line to it
			if (dialogueExists) {
				temp[temp.size() - 1].lines.push_back(l);
			}
			// else crate a new dialogue and add the line
			else {
				//create a dialogue instance and assign the name
				Dialogue d;
				d.name = dialogueName;
				d.lines.push_back(l);
				temp.push_back(d);
			}
			dialogueLine = dialogueLine->next_sibling();
		} while (dialogueLine);
		//return populated list
		return temp;
	}
}