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
	vector<displayBar> bars;
	vector<option> options;
	//Default Constructor:
	page() {
	}

	//Parameterized Constructor:
	page(string name, string prompt, vector<displayBar> bars, vector<option> options) {
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
			bars[i].printBar();
		}
		cout << "\n";
		printOptions();

	}
};

// Options function/detail prototypes:
void detailPlaylist();
void detailTemp();
void functionTemp();
void functionBegin();
void functionNext();
void functionAddPlayer();
void functionFinished();

//-Page prompts:
string beginPrompt = "Welcome to Truth or Dare! Use the arrows and enter to navigate.";
string playlistPrompt = "Playlist Selector. Choose an existing playlist or create a new set.";
string settingsPrompt = "Settings. Hit enter on an option to toggle.";
string playPlayerInputPrompt = "Play - Character Input. Add players to the game.";

//-Page displayBars:
vector<string> mainDisplayItems = { "Begin", "Play", "Playlist", "Settings" };
vector<string> playerDisplayItems = { "Players:" };
string mainDisplayPostString = "----------------------------------------------------------------";
string playerDisplayPostString = "";

displayBar mainBar = displayBar(&mainDisplayItems, mainDisplayPostString, 0, 8);
vector<displayBar> mainBars = { mainBar };
displayBar playlistBar = displayBar(&mainDisplayItems, mainDisplayPostString, 2, 8);
vector<displayBar> playlistBars = { playlistBar };
displayBar settingsBar = displayBar(&mainDisplayItems, mainDisplayPostString, 3, 8);
vector<displayBar> settingsBars = { settingsBar };
displayBar playPlayerInputBar = displayBar(&mainDisplayItems, mainDisplayPostString, 1, 8);
displayBar playPlayerInputBar2 = displayBar(&playerDisplayItems, playerDisplayPostString, 1, 1);
vector<displayBar> playPlayerInputBars = { playPlayerInputBar, playPlayerInputBar2 };


//-Page options:
option beginO = option("Begin", "beginP", &functionBegin, &detailTemp);
vector<option> beginPageOptions = { beginO };
vector<option> playlistPageOptions = { };
option nextO = option("Next", "settingsP", &functionNext, &detailTemp);
vector<option> settingsPageOptions = { nextO };
option addPlayerO = option("Add Player", "playPlayerInputP", &functionAddPlayer, &detailTemp);
option finishedO = option("Finished", "playPlayerInputP", &functionFinished, &detailTemp);
vector<option> playPlayerInputOptions = { addPlayerO, finishedO };

//Pages:
page beginP = page("beginP", beginPrompt, mainBars, beginPageOptions);
page playlistP = page("playlistP", playlistPrompt, playlistBars, playlistPageOptions);
page settingsP = page("settingsP", settingsPrompt, settingsBars, settingsPageOptions);
page playPlayerInputP = page("playPlayerInputP", playPlayerInputPrompt, playPlayerInputBars, playPlayerInputOptions);
vector<page*> pages = { &beginP, &playlistP, &settingsP, &playPlayerInputP };

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
	//Access specifier:
public:
	string playedPlaylist;
	vector<string> log;
	//Default Constructor:
	gameLog() {
	}
	gameLog(string playedPlaylist) {
		this->playedPlaylist = playedPlaylist;
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
	
}



