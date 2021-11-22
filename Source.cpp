using namespace std;
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <time.h>
#include <conio.h>
#include <string>

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
	displayBar(string name, vector<string>* displayItems, int displayIndex, int space) {
		this->name = name;
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
			
			if (i < (*displayItems).size() - 1) {
				for (int x = 0; x < space; x++) {
					tempStr += " ";
				}
			}

			cout << tempStr;
		}
		cout << endl;
	}

};
//-Page displayBars:
vector<string> mainDisplayItems = { "Begin", "Play", "Playlist", "Settings" };
vector<string> playerDisplayItems = { "Players:" };

displayBar mainBar = displayBar("mainDisplayBar", &mainDisplayItems, 0, 10);
displayBar playersBar = displayBar("playersDisplayBar", &playerDisplayItems, -1, 1);
vector<displayBar*> globalBars = { &mainBar, &playersBar };

class option : public visualElement {
	//Access specifier:
public:
	string prefix = " ";
	bool isSelected = false;
	void (*function)();
	void (*detail)();
	//Default Constructor:
	option() {
		this->elementType = "option";
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
option option_new = option("New...", "playlistP", &functionNew, &detailTemp);    //New option "new"
option option_setName = option("Set Name", "playlistCreatorP", &functionSetName, &detailSetName);
option option_addTruth = option("Add Truth", "playlistCreatorP", &functionAddTruth, &detailAddTruth);
option option_addDare = option("Add Dare", "playlistCreatorP", &functionAddDare, &detailAddDare);
option option_createPlaylist = option("Create Playlist", "playlistCreatorP", &functionBegin, &detailTemp);
vector<option*> options_playlistCreator = { &option_setName, &option_addTruth, &option_addDare, &option_createPlaylist };

option option_begin = option("Begin", "beginP", &functionBegin, &detailTemp);
vector<option*> options_mainPage = { &option_begin };
vector<option*> options_playlistPage = { &option_new };
option option_next = option("Next", "settingsP", &functionNext, &detailTemp);
vector<option*> options_settingsPage = { &option_next };
option option_addPlayer = option("Add Player", "playPlayerInputP", &functionAddPlayer, &detailTemp);
option option_finished = option("Finished", "playPlayerInputP", &functionFinished, &detailTemp);
vector<option*> options_playerInputPage = { &option_addPlayer, &option_finished };
option option_truth = option("Truth", "playTruthOrDareP", &functionTruth, &detailTemp);
option option_dare = option("Dare", "playTruthOrDareP", &functionDare, &detailTemp);
vector<option*> options_playPage = { &option_truth, &option_dare };

vector<vector<option*>> globalOptions = { options_playlistPage, options_mainPage, options_playlistCreator, options_settingsPage, options_playerInputPage, options_playPage };

class sideBar {
	string cursorZeroZero = "\033[H";
	vector<vector<string>*> columns = { &column1, &column2 };
	//Access specifier:
public:
	vector<string> column1 = {};
	vector<string> column2 = {};
	
	
	//Default constructor:
	sideBar() {
	}
	//Parameterized constructor:
	sideBar(vector<string> column1, vector<string> column2) {
		this->column1 = column1;
		this->column2 = column2;
	}

	//Methods:
	void printSideBar() {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < columns[i]->size(); j++) {
				string cursorPos = cursorZeroZero + "\033[" + to_string(j + 1) + ";30H";
				cout << cursorPos << (*columns[i])[j];
			}
		}
	}

};
//-Page sideBars:
sideBar noSideBar = sideBar();

class page {
	//Access specifier:
public:
	string name;
	vector<visualElement*> elements;
	sideBar sideBarE;

	vector<visualElement*> postElements;
	vector<displayBar*> bars;
	vector<option*> options;
	//Default Constructor:
	page() {
	}

	//Parameterized Constructor:
	page(string name, vector<visualElement*> elements, sideBar sideBarE) {
		this->name = name;
		this->elements = elements;
		this->bars = bars;
		this->options = options;
		this->sideBarE = sideBarE;
		//Compare elements list vs global options and display bars. .push_back to the respective list.
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
			if (elements[i]->elementType == "displayBar") {
				for (int x = 0; x < globalBars.size(); x++) {
					if (elements[i]->name == globalBars[x]->name) {
						bars.push_back(globalBars[x]);
					}
				}
			}
		}
	}

	//Methods:
	void optionsToPostElements() {
		for (int i = 0; i < options.size(); i++) {
			postElements.push_back(options[i]);
		}
	}
	//Assign prefix for options as an indicator for user selection.
	void assignPrefixes() {
		int optionIndex = 0;
		int testIndex = 0;
		vector<vector<visualElement*>> allElements = { elements, postElements };
		for (int h = 0; h < allElements.size(); h++) {
			for (int i = 0; i < allElements[h].size(); i++) {
				if (allElements[h][i]->elementType == "option") {
					if (optionIndex == currentOptionIndex) {
						options[optionIndex]->prefix = ">";
						options[optionIndex]->isSelected = true;
					}
					else {
						options[optionIndex]->prefix = ":";
					}
					optionIndex++;
				}
				testIndex++;
			}
		}
	}
	void printPage() {
		system("CLS");
		assignPrefixes();
		for (int i = 0; i < elements.size(); i++) {
			elements[i]->printElement();
		}
		for (int i = 0; i < postElements.size(); i++) {
			postElements[i]->printElement();
		}
		sideBarE.printSideBar();
	}
};

//-Page prompts:
lineStr ____space("");
lineStr ____line("--------------------------------------------------------------------                ----------------------------------");
lineStr str_mainPrompt("Welcome to Truth or Dare! Use the arrows and enter to navigate.");
lineStr str_playlistPrompt("Playlist Selector. Choose an existing playlist or create a new set.");
lineStr str_settingsPrompt("Settings. Hit enter on an option to toggle.");
lineStr str_playerInputPrompt("Play - Character Input. Add players to the game.");
lineStr str_playlistCreatorPrompt("Playlist Creator. Create a your own playlist.");
lineStr str_playPagePrompt("Truth or Dare!");

//Pages:
vector<visualElement*> mainElements = { 
	& str_mainPrompt, 
	& ____space,
	& ____line,
	& mainBar,
	& ____line,
	& ____space,
	& option_begin
};
page mainP = page("beginP", mainElements, noSideBar);

vector<visualElement*> playlistElements = {
	& str_playlistPrompt,
	& ____space,
	& ____line,
	& mainBar,
	& ____line,
	& ____space,
};

page playlistP = page("playlistP", playlistElements, noSideBar);

vector<visualElement*> playlistCreatorElements = {
	& str_playlistCreatorPrompt,
	& ____space,
	& ____line,
	& mainBar,
	& ____line,
	& ____space,
	& option_setName,
	& option_addTruth,
	& option_addDare,
	& option_createPlaylist
};
page playlistCreatorP = page("playlistCreatorP", playlistCreatorElements, noSideBar);

vector<visualElement*> settingsElements = {
	& str_settingsPrompt,
	& ____space,
	& ____line,
	& mainBar,
	& ____line,
	& ____space,
	& option_next
};
page settingsP = page("settingsP", settingsElements, noSideBar);

vector<visualElement*> playerInputElements = {
	& str_playerInputPrompt,
	& ____space,
	& ____line,
	& playersBar,
	& ____line,
	& ____space,
	& option_addPlayer,
	& option_finished
};
page playerInputP = page("playPlayerInputP", playerInputElements, noSideBar);

vector<visualElement*> playElements = {
	& str_playPagePrompt,
	& ____space,
	& ____line,
	& mainBar,
	& ____line,
	& ____space,
	& option_truth,
	& option_dare
};
page playP = page("playTruthOrDareP", playElements, noSideBar);
vector<page*> pages = { &mainP, &playlistP, &settingsP, &playerInputP, &playlistCreatorP, &playP };

// Global Variables
int* pIndex = &playersBar.displayIndex;
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

// Group5 ------------------------------------------------------------ Group5
// Classes and Variables for game controlling
class gameController {
	//Access specifier:
public:
	int performersPerTurn = 1;
	bool turnRandom = false;
	bool tallyVoting = true;
	bool detailsPrompt = false;

	//Default constructor:
	gameController() {
	}
	//Parameterized constructor:
	gameController(int performersPerTurn, bool turnRandom, bool tallyVoting, bool detailsPrompt) {
		this->performersPerTurn = performersPerTurn;
		this->turnRandom = turnRandom;
		this->tallyVoting = tallyVoting;
		this->detailsPrompt = detailsPrompt;
	}
};
class cache {
	//Access specifier:
public:
	vector<player> players = {};
	vector<option> options = {};
	vector<playlist> playlists = {};
	vector<visualElement> elements = {};
	//Default Constructor:
	cache() {
	}
};
cache cache1 = cache();

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
	currentOptionIndex = 0;
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
	cout << "  Truths\n";
	for (int i = 0; i < playlists[currentOptionIndex].truths.size(); i++) {
		string truth = playlists[currentOptionIndex].truths[i];
		cout << "  -" << truth << "\n";
	}
	cout << "\n  Dares\n";
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
	mainBar.displayIndex = 3;
	gamePlaylist = &playlists[currentOptionIndex];
	mainDisplayItems[2] = gamePlaylist->name;
	activatePage(settingsP);
}
void functionBegin() {
	mainBar.displayIndex = 2;
	cache1.options.clear();
	for (int i = 0; i < playlists.size(); i++) {
		cache1.options.push_back(option());
		cache1.options[i].name = playlists[i].name;
		cache1.options[i].pageName = "playlistP";
		cache1.options[i].function = &functionPlaylistN;
		cache1.options[i].detail = &detailPlaylist;
	}
	playlistP.options.clear();
	for (int i = 0; i < cache1.options.size(); i++) {
		playlistP.options.push_back(&cache1.options[i]);
	}
	playlistP.options.push_back(&option_new);
	playlistP.postElements.clear();
	playlistP.optionsToPostElements();
	activatePage(playlistP);
}
void functionNext() {
	mainBar.displayIndex = 1;
	activatePage(playerInputP);
}
void functionAddPlayer() {
	string tempName;
	cout << "\nEnter player name: ";
	cin >> tempName;
	playerDisplayItems.push_back(tempName);
	reprintPage();
}
void functionFinished() {
	activatePage(playP);
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