#include "Music.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <tchar.h>

#define MAX_SONGS 50

void ListSongs() {
	int current=0;
	std::string aux;
	std::string songlist[MAX_SONGS];
	_WIN32_FIND_DATAA* music_data=new _WIN32_FIND_DATAA;
	HANDLE music_hnd;
	system("cls");
	music_hnd=FindFirstFileA(TEXT("C://Users//vrabi//source//repos//Project1//Music//*.wav"), music_data);
	std::wcout << current << ". " << music_data->cFileName<<"\n";
	aux = ".//Music//";
	aux.append(music_data->cFileName);
	songlist[current++] = aux;
	while (FindNextFile(music_hnd, music_data)) {
		std::wcout << current<<". "<<music_data->cFileName<<"\n";
		aux = ".//Music//";
		aux.append(music_data->cFileName);
		songlist[current++] = aux;
	}
	std::cout << "Choose a song (it will play in a loop): ";
	std::cin >> current;
	PlaySong(songlist[current]);
}

void PlaySong(std::string filename) {
	PlaySound(TEXT(filename.c_str()), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	system("cls");
}