using namespace std;
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <time.h>
#include <conio.h>

// Group1 ------------------------------------------------------------ Group1
// Variables controlling active pages/options
int currentOptionIndex = 0;
int currentPageIndex = 0;
bool listenKeys = true;
int currentOptionsLen = NULL;

// Group1.1
// General function prototypes:
bool navigate(int asc);
void reprintPage();
void rotateTurn();

// Group2 ------------------------------------------------------------ Group2
// Classes and Variables related to or for pages (displaying screen and options to press)
class visualElement {
	//Access specifier:
public:
	string elementType = "visualElement";
	string name;
	string pageName;
	virtual void printElement() {
		cout << "Print base element." << endl;
	}
};

class lineStr : public visualElement {
	//Access specifier:
public:
	string str;

	//Default Constructor:
	lineStr() {
	}
	//Parameterized Constructor:
	lineStr(string str) {
		this->str = str;
		this->elementType = "lineStr";
	}

	//Methods:
	void printElement() {
		cout << str << endl;
	}
};

class displayBar : public visualElement {
	//Access specifier:
public:
	vector<string>* displayItems;
	int displayIndex = 0;
	int space;

	//Default Constructor:
	displayBar() {
	}

	//Parameterized Constructor:
	displayBar(vector<string>* displayItems, int displayIndex, int space) {
		this->displayItems = displayItems;
		this->displayIndex = displayIndex;
		this->space = space;
		this->elementType = "displayBar";
	}

	//Methods:
	void printElement() {
		string prefix = " ";
		string suffix = " ";
		for (int i = 0; i < (*displayItems).size(); i++) {
			if (i == displayIndex) {
				prefix = "|";
				suffix = "|";
			}
			else {
				prefix = " ";
				suffix = " ";
			}
			string tempStr = prefix + (*displayItems)[i] + suffix;
			for (int i = 0; i < space; i++) {
				tempStr += " ";
			}

			cout << tempStr;
		}
		cout << endl;
	}

};

class option : public visualElement {
	//Access specifier:
public:
	string prefix = " ";
	bool isSelected = false;
	void (*function)();
	void (*detail)();
	//Default Constructor:
	option() {
	}

	//Parameterized Constructor:
	option(string name, string pageName, void (*function)(), void (*detail)()) {
		this->name = name;
		this->pageName = pageName;
		this->function = function;
		this->detail = detail;
		this->elementType = "option";
	}

	//Methods
	void printElement() {
		cout << prefix << name << "\n";
		if (isSelected) {
			detail();
			isSelected = false;
		}
	}

};
// Options function/detail prototypes:
void detailTemp();
void detailPlaylist();
void detailAddTruth();
void detailAddDare();
void detailSetName();
void functionTemp();
void functionBegin();
void functionNext();
void functionAddPlayer();
void functionFinished();
void functionNew();
void functionSetName();
void functionAddTruth();
void functionAddDare();
void functionTruth();
void functionDare();

//-Page options:
option newO = option("New...", "playlistP", &functionNew, &detailTemp);    //New option "new"
option setNameO = option("Set Name", "playlistCreatorP", &functionSetName, &detailSetName);
option addTruthO = option("Add Truth", "playlistCreatorP", &functionAddTruth, &detailAddTruth);
option addDareO = option("Add Dare", "playlistCreatorP", &functionAddDare, &detailAddDare);
option createPlaylistO = option("Create Playlist", "playlistCreatorP", &functionBegin, &detailTemp);
vector<option*> options_playlistCreator = { &setNameO, &addTruthO, &addDareO, &createPlaylistO };

option option_begin = option("Begin", "beginP", &functionBegin, &detailTemp);
vector<option*> options_mainPage = { &option_begin };
vector<option*> options_playlistPage = { &newO };
option nextO = option("Next", "settingsP", &functionNext, &detailTemp);
vector<option*> options_settingsPage = { &nextO };
option addPlayerO = option("Add Player", "playPlayerInputP", &functionAddPlayer, &detailTemp);
option finishedO = option("Finished", "playPlayerInputP", &functionFinished, &detailTemp);
vector<option*> options_playerInputPage = { &addPlayerO, &finishedO };
option truthO = option("Truth", "playTruthOrDareP", &functionTruth, &detailTemp);
option dareO = option("Dare", "playTruthOrDareP", &functionDare, &detailTemp);
vector<option*> options_playPage = { &truthO, &dareO };
vector<vector<option*>> globalOptions = { options_mainPage, options_playlistCreator, options_playlistPage, options_settingsPage, options_playerInputPage, options_playPage };

class page {
	//Access specifier:
public:
	string name;
	vector<visualElement*> elements;
	vector<displayBar*> bars;
	vector<option*> options;
	//Default Constructor:
	page() {
	}

	//Parameterized Constructor:
	page(string name, vector<visualElement*> elements, vector<displayBar*> bars) {
		this->name = name;
		this->elements = elements;
		this->bars = bars;
		this->options = options;
		//Compare elements list and global options list, options.pushBack named options from elements list.
		for (int i = 0; i < elements.size(); i++) {
			if (elements[i]->elementType == "option") {
				for (int x = 0; x < globalOptions.size(); x++) {
					for (int y = 0; y < globalOptions[x].size(); y++) {
						if (elements[i]->name == globalOptions[x][y]->name && elements[i]->pageName == globalOptions[x][y]->pageName) {
							options.push_back(globalOptions[x][y]);
						}
					}
				}
			}
		}
	}

	//Methods:

	//Assign prefix for options as an indicator for user selection.
	void assignPrefixes() {
		int optionIndex = 0;
		for (int i = 0; i < elements.size(); i++) {
			if (elements[i]->elementType == "option") {
				if (optionIndex == currentOptionIndex) {
					options[optionIndex]->prefix = ">";
				}
				else {
					options[optionIndex]->prefix = ":";
				}
				optionIndex++;
			}
		}
	}
	void printPage() {
		system("CLS");
		assignPrefixes();
		for (int i = 0; i < elements.size(); i++) {
			elements[i]->printElement();
		}
	}
};




//-Page prompts:
lineStr ____space("");
lineStr ____line("----------------------------------------------------------------");
lineStr str_mainPrompt("Welcome to Truth or Dare! Use the arrows and enter to navigate.");
lineStr str_playlistPrompt("Playlist Selector. Choose an existing playlist or create a new set.");
lineStr str_settingsPrompt("Settings. Hit enter on an option to toggle.");
lineStr str_playerInputPrompt("Play - Character Input. Add players to the game.");
lineStr str_playlistCreatorPrompt("Playlist Creator. Create a your own playlist.");
lineStr str_playPagePrompt("Would you like to answer a Truth, or face a Dare?");

//-Page displayBars:
vector<string> mainDisplayItems = { "Begin", "Play", "Playlist", "Settings" };
vector<string> playerDisplayItems = { "Players:" };

displayBar mainBar = displayBar(&mainDisplayItems, 0, 8);
vector<displayBar*> mainBars = { &mainBar };
displayBar playlistBar = displayBar(&mainDisplayItems, 2, 8);
vector<displayBar*> playlistBars = { &playlistBar };
displayBar settingsBar = displayBar(&mainDisplayItems, 3, 8);
vector<displayBar*> settingsBars = { &settingsBar };
displayBar playPlayerInputBar = displayBar(&mainDisplayItems, 1, 8);
displayBar playPlayerInputBar2 = displayBar(&playerDisplayItems, 1, 1);
vector<displayBar*> playPlayerInputBars = { &playPlayerInputBar, &playPlayerInputBar2 };
displayBar playTruthOrDareBar = displayBar(&mainDisplayItems, 1, 8);
displayBar playTruthOrDareBar2 = displayBar(&playerDisplayItems, 1, 1);
vector<displayBar*> playTruthOrDareBars = { &playTruthOrDareBar, &playTruthOrDareBar2 };

//Pages:
vector<visualElement*> mainElements = { 
	&str_mainPrompt, 
	&____space,
	&____line,
	&mainBar,
	&____line,
	&____space,
	&option_begin
};
page mainP = page("beginP", mainElements, mainBars);
page playlistP = page("playlistP", { }, playlistBars);
page playlistCreatorP = page("playlistCreatorP", { }, playlistBars);
page settingsP = page("settingsP", { }, settingsBars);
page playPlayerInputP = page("playPlayerInputP", { }, playPlayerInputBars);
page playTruthOrDareP = page("playTruthOrDareP", { }, playTruthOrDareBars);
vector<page*> pages = { &mainP, &playlistP, &settingsP, &playPlayerInputP, &playlistCreatorP, &playTruthOrDareP };

// Global Variables
int* pIndex = &playTruthOrDareBar2.displayIndex;
string playerInput;
string groupInput;


// Group3 ------------------------------------------------------------ Group3
// Class and Variables for playlist (sets of truths and dares)
class playlist {
	//Access specifier:
public:
	string name;
	vector<string> truths{};
	vector<string> dares{};

	//Default Constructor:
	playlist() {
	}

	//Parameterized Constructor:
	playlist(string name, vector<string> truths, vector<string> dares) {
		this->name = name;
		this->truths = truths;
		this->dares = dares;
	}

	//Methods:

};

//-Playlist truths:
vector<string> truths1{
	"When was the last time you lied?",
	"When was the last time you cried?",
	"What's your biggest fear?",
	"What's your biggest fantasy?"
};

//-Playlist dares:
vector<string> dares1{
	"Do 100 squats.",
	"Show the most embarrassing photo on your phone.",
	"Let the rest of the group DM someone from your Instagram account.",
	"Eat a banana without using your hands."
};

//Playlist:
playlist playlist1("Playlist1", truths1, dares1);
playlist playlist2("Playlist2", truths1, dares1);
playlist* gamePlaylist;
vector<playlist> playlists = { playlist1, playlist2 };

// Group4 ------------------------------------------------------------ Group4
// Class and Variables for player
class gameLog {
	// Access specifier:
public:
	vector<string> log;
	vector<vector<string>> playerLogs;
	vector<string> playerNames;
	bool inOrder;
	bool printFullTruths;

	// Default Constructor:
	gameLog() {
	}
	gameLog(bool inOrder, bool printFullTruths) {
		this->inOrder = inOrder;

	}

	// Methods:
	void space() {
		log.push_back("");
	}
	void line() {
		log.push_back("-----------------------------------------------------------------------------------------------");
	}
	void writePlaylist(playlist p) {
		line();
		log.push_back("Playlist name: " + p.name);
		space();
		log.push_back("Truths: ");
		for (int i = 0; i < p.truths.size(); i++) {
			log.push_back("-" + p.truths[i]);
		}
		space();
		log.push_back("Dares: ");
		for (int i = 0; i < p.dares.size(); i++) {
			log.push_back("-" + p.dares[i]);
		}
		line();
	}
	void writeLog(string playerName, string challengeType, string truthOrDare, string playerInput, string groupInput) {
		if (inOrder) {
			space();
			log.push_back("PlayerName: " + playerName);
			log.push_back(challengeType + ": " + truthOrDare);
			space();
			log.push_back(playerName + "'s respponse: " + playerInput);
			log.push_back("The group decided: ");
			space();
			line();
		}
	}



};

class player {
	//Access specifier:
public:
	string name;
	int points = 5;
	int turnIndex;
	//Default Constructor:
	player() {
	}
	//Parameterized Constructor:
	player(string name, int points, int turnIndex) {
		this->name = name;
		this->points = points;
		this->turnIndex = turnIndex;

	}

	//Methods:

};

// GroupMain ------------------------------------------------------ GroupMain
//
void activatePage(page p);

int main() {
	srand(time(NULL));
	activatePage(mainP);

	while (listenKeys) {
		char c = _getch();
		int ascii = c;
		//Use arrows to change currentOptionIndex.
		if (navigate(ascii)) {

		}
		if (ascii == 13) {
			//When enter is pressed, run the function of the currentOption.
			page currentPage = *pages[currentPageIndex];
			option currentOption = *currentPage.options[currentOptionIndex];
			currentOption.function();
		}
	}
}

// General
bool navigate(int asc) {

	if (asc == 72 && currentOptionIndex > 0) {
		currentOptionIndex--;
		(*pages[currentPageIndex]).printPage();
		return true;
	}
	if (asc == 80 && currentOptionIndex < currentOptionsLen - 1) {
		currentOptionIndex++;
		(*pages[currentPageIndex]).printPage();
		return true;
	}
	return false;
}
void reprintPage() {
	pages[currentPageIndex]->printPage();
}
void activatePage(page p) {
	for (int i = 0; i < pages.size(); i++) {
		if ((p.name).compare((*pages[i]).name) == 0) {
			currentPageIndex = i;
			currentOptionsLen = p.options.size();
			break;
		}
	}
	p.printPage();
}
void rotateTurn() {
	if (*pIndex < playerDisplayItems.size() - 1) {
		*pIndex = *pIndex + 1;
	}
	else {
		*pIndex = 1;
	}
	reprintPage();
}

// Option details
void detailTemp() {
	cout << "";
}
void detailPlaylist() {
	cout << "  Truths:\n";
	for (int i = 0; i < playlists[currentOptionIndex].truths.size(); i++) {
		string truth = playlists[currentOptionIndex].truths[i];
		cout << "  -" << truth << "\n";
	}
	cout << "\n  Dares:\n";
	for (int i = 0; i < playlists[currentOptionIndex].dares.size(); i++) {
		string dare = playlists[currentOptionIndex].dares[i];
		cout << "  -" << dare << "\n";
	}
	cout << "\n";
}
void detailAddTruth() {
	if (playlists[playlists.size() - 1].truths.size() > 0) {
		cout << "  Truths:\n";
		for (int i = 0; i < playlists[playlists.size() - 1].truths.size(); i++) {
			string truth = playlists[playlists.size() - 1].truths[i];
			cout << "  -" << truth << "\n";
		}
	}
}
void detailAddDare() {
	if (playlists[playlists.size() - 1].dares.size() > 0) {
		cout << "  Dare:\n";
		for (int i = 0; i < playlists[playlists.size() - 1].dares.size(); i++) {
			string dare = playlists[playlists.size() - 1].dares[i];
			cout << "  -" << dare << "\n";
		}
	}
}
void detailSetName() {
	if (playlists[playlists.size() - 1].name.length() > 0) {
		cout << "  Playlist Name: " << playlists[playlists.size() - 1].name << endl;
	}
}
// Option Functions
void functionTemp() {
	cout << "Option Pressed";
}
void functionPlaylistN() {
	gamePlaylist = &playlists[currentOptionIndex];
	mainDisplayItems[2] = gamePlaylist->name;
	activatePage(settingsP);
}
void functionBegin() {
	//playlistP.options.clear();
	//for (int i = 0; i < playlists.size(); i++) {
	//	playlistP.options.push_back(option());
	//	playlistP.options[i].name = playlists[i].name;
	//	playlistP.options[i].pageName = "playlistP";
	//	playlistP.options[i].function = &functionPlaylistN;
	//	playlistP.options[i].detail = &detailPlaylist;
	//}
	//playlistP.options.push_back(newO);
	activatePage(playlistP);
}
void functionNext() {
	activatePage(playPlayerInputP);
}
void functionAddPlayer() {
	string tempName;
	cout << "\nEnter player name: ";
	cin >> tempName;
	playerDisplayItems.push_back(tempName);
	reprintPage();
}
void functionFinished() {
	activatePage(playTruthOrDareP);
}
void functionNew() {
	playlists.push_back(playlist());
	activatePage(playlistCreatorP);

}
void functionSetName() {
	string tempSetName;
	cout << "\nEnter playlist name: ";
	cin >> tempSetName;
	playlists[playlists.size() - 1].name = tempSetName;
	reprintPage();

}
void functionAddDare() {
	string tempAddDare;
	cout << "\nEnter dare: ";
	cin >> tempAddDare;
	playlists[playlists.size() - 1].dares.push_back(tempAddDare);
	reprintPage();
}
void functionAddTruth() {
	string tempTruth;
	cout << "\nEnter Truth: ";
	cin >> tempTruth;
	playlists[playlists.size() - 1].truths.push_back(tempTruth);
	reprintPage();
}
void functionTruth() {
	int tIndex = rand() % gamePlaylist->truths.size();
	cout << "\nTruth: " << gamePlaylist->truths[tIndex] << endl;
	cout << "The rest of the group, vote now: did " << playerDisplayItems[*pIndex] << " answer truthfully?" << endl;
	cin >> groupInput;
	rotateTurn();
}
void functionDare() {
	int dIndex = rand() % gamePlaylist->dares.size();
	cout << "\nTruth: " << gamePlaylist->dares[dIndex] << endl;
	cout << "The rest of the group, vote now: how did " << playerDisplayItems[*pIndex] << " perform?" << endl;
	cin >> groupInput;
	rotateTurn();
}