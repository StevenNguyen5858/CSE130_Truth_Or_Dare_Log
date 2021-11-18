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
class displayBar {
	//Access specifier:
public:
	vector<string>* displayItems;
	string postString;
	int displayIndex = 0;
	int space;

	//Default Constructor:
	displayBar() {
	}

	//Parameterized Constructor:
	displayBar(vector<string>* displayItems, string postString, int displayIndex, int space) {
		this->displayItems = displayItems;
		this->postString = postString;
		this->displayIndex = displayIndex;
		this->space = space;
	}

	//Methods:
	void printBar() {
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
		cout << "\n" << postString << "\n";
	}

};

class option {
	//Access specifier:
public:
	string name;
	string pageName;
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
	}

	//Methods
	void printOption(string prefix) {
		cout << prefix << name << "\n";
	}

};

class page {
	//Access specifier:
public:
	string name;
	string prompt;
	vector<displayBar*> bars;
	vector<option> options;
	//Default Constructor:
	page() {
	}

	//Parameterized Constructor:
	page(string name, string prompt, vector<displayBar*> bars, vector<option> options) {
		this->name = name;
		this->prompt = prompt;
		this->bars = bars;
		this->options = options;
	}

	//Methods:
	void printOptions() {
		string prefix = " ";
		for (int i = 0; i < options.size(); i++) {
			if (i == currentOptionIndex) {
				prefix = ">";
			}
			else {
				prefix = " ";
			}
			options[i].printOption(prefix);
			if (i == currentOptionIndex) {
				options[i].detail();
			}
		}

	}

	void printPage() {
		system("CLS");
		cout << prompt << "\n\n";
		cout << "----------------------------------------------------------------\n";
		for (int i = 0; i < bars.size(); i++) {
			(*bars[i]).printBar();
		}
		cout << "\n";
		printOptions();

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


//-Page prompts:
string beginPrompt = "Welcome to Truth or Dare! Use the arrows and enter to navigate.";
string playlistPrompt = "Playlist Selector. Choose an existing playlist or create a new set.";
string settingsPrompt = "Settings. Hit enter on an option to toggle.";
string playPlayerInputPrompt = "Play - Character Input. Add players to the game.";
string playlistCreatorPrompt = "Playlist Creator. Create a your own playlist.";
string playTruthOrDarePrompt = "Would you like to answer a Truth, or face a Dare?";

//-Page displayBars:
vector<string> mainDisplayItems = { "Begin", "Play", "Playlist", "Settings" };
vector<string> playerDisplayItems = { "Players:" };
string mainDisplayPostString = "----------------------------------------------------------------";
string playerDisplayPostString = "";

displayBar mainBar = displayBar(&mainDisplayItems, mainDisplayPostString, 0, 8);
vector<displayBar*> mainBars = { &mainBar };
displayBar playlistBar = displayBar(&mainDisplayItems, mainDisplayPostString, 2, 8);
vector<displayBar*> playlistBars = { &playlistBar };
displayBar settingsBar = displayBar(&mainDisplayItems, mainDisplayPostString, 3, 8);
vector<displayBar*> settingsBars = { &settingsBar };
displayBar playPlayerInputBar = displayBar(&mainDisplayItems, mainDisplayPostString, 1, 8);
displayBar playPlayerInputBar2 = displayBar(&playerDisplayItems, playerDisplayPostString, 1, 1);
vector<displayBar*> playPlayerInputBars = { &playPlayerInputBar, &playPlayerInputBar2 };
displayBar playTruthOrDareBar = displayBar(&mainDisplayItems, mainDisplayPostString, 1, 8);
displayBar playTruthOrDareBar2 = displayBar(&playerDisplayItems, playerDisplayPostString, 1, 1);
vector<displayBar*> playTruthOrDareBars = { &playTruthOrDareBar, &playTruthOrDareBar2 };

//-Page options:
option newO = option("New...", "playlistP", &functionNew, &detailTemp);    //New option "new"
option setNameO = option("Set Name", "playlistCreatorP", &functionSetName, &detailSetName);
option addTruthO = option("Add Truth", "playlistCreatorP", &functionAddTruth, &detailAddTruth);
option addDareO = option("Add Dare", "playlistCreatorP", &functionAddDare, &detailAddDare);
option createPlaylistO = option("Create Playlist", "playlistCreatorP", &functionBegin, &detailTemp);
vector<option> playlistCreatorOptions = { setNameO, addTruthO, addDareO, createPlaylistO };

option beginO = option("Begin", "beginP", &functionBegin, &detailTemp);
vector<option> beginPageOptions = { beginO };
vector<option> playlistPageOptions = { newO };
option nextO = option("Next", "settingsP", &functionNext, &detailTemp);
vector<option> settingsPageOptions = { nextO };
option addPlayerO = option("Add Player", "playPlayerInputP", &functionAddPlayer, &detailTemp);
option finishedO = option("Finished", "playPlayerInputP", &functionFinished, &detailTemp);
vector<option> playPlayerInputOptions = { addPlayerO, finishedO };
option truthO = option("Truth", "playTruthOrDareP", &functionTruth, &detailTemp);
option dareO = option("Dare", "playTruthOrDareP", &functionDare, &detailTemp);
vector<option> playTruthOrDareOptions = { truthO, dareO };

//Pages:
page playlistCreatorP = page("playlistCreatorP", playlistCreatorPrompt, playlistBars, playlistCreatorOptions);

page beginP = page("beginP", beginPrompt, mainBars, beginPageOptions);
page playlistP = page("playlistP", playlistPrompt, playlistBars, playlistPageOptions);
page settingsP = page("settingsP", settingsPrompt, settingsBars, settingsPageOptions);
page playPlayerInputP = page("playPlayerInputP", playPlayerInputPrompt, playPlayerInputBars, playPlayerInputOptions);
page playTruthOrDareP = page("playTruthOrDareP", playTruthOrDarePrompt, playTruthOrDareBars, playTruthOrDareOptions);
vector<page*> pages = { &beginP, &playlistP, &settingsP, &playPlayerInputP, &playlistCreatorP, &playTruthOrDareP };

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
	activatePage(beginP);

	while (listenKeys) {
		char c = _getch();
		int ascii = c;
		//Use arrows to change currentOptionIndex.
		if (navigate(ascii)) {

		}
		if (ascii == 13) {
			//When enter is pressed, run the function of the currentOption.
			page currentPage = *pages[currentPageIndex];
			option currentOption = currentPage.options[currentOptionIndex];
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
	playlistP.options.clear();
	for (int i = 0; i < playlists.size(); i++) {
		playlistP.options.push_back(option());
		playlistP.options[i].name = playlists[i].name;
		playlistP.options[i].pageName = "playlistP";
		playlistP.options[i].function = &functionPlaylistN;
		playlistP.options[i].detail = &detailPlaylist;
	}
	playlistP.options.push_back(newO);
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

