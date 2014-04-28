//
// Catrap level parser
//
// concrete atelier 2014
// 

#include <Magick++.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

using namespace Magick;

enum Tile {bg, 
		   boy, 
		   breakable, 
		   ghost, 
		   girlshadow, 
		   ladder,
		   mummy,
		   muscle,
		   rock,
		   wall,
		   wizard};

char tiles[] = ".B%FX|MNO#W";

const int LegendSize = 11;
const int h_tiles    = 11;
const int v_tiles    = 8;
const int tilesize   = 16;

Image legend[LegendSize];

Image* buildLegend() {
	try {
		legend[bg].read("legend\\bg.png");
		legend[boy].read("legend\\boy.png");
		legend[breakable].read("legend\\breakable.png");
		legend[ghost].read("legend\\ghost.png");
		legend[girlshadow].read("legend\\girlshadow.png");
		legend[ladder].read("legend\\ladder.png");
		legend[mummy].read("legend\\mummy.png");
		legend[muscle].read("legend\\muscle.png");
		legend[rock].read("legend\\rock.png");
		legend[wall].read("legend\\wall.png");
		legend[wizard].read("legend\\wizard.png");
	}
	catch( exception &error_ )
    {
      cout << "Caught exception: " << error_.what() << endl;
    }
	return legend;
}

string parseLevel(string Level, Image* Legend) {
	Image level(Level);
	string result("#############\n#");

	int row = 0;
	int col = 0;

	while (row < v_tiles) {
		Image temp = level;
		int left = col*tilesize;
		int right = left + tilesize;
		int top = row*tilesize;
		int bottom = row*tilesize + tilesize;	

		temp.crop(Geometry(tilesize, tilesize, left, top));

		int bestmatch = 0;
		int bestvalue = 999999999;

		for (int i = 0; i < LegendSize; i++) {
			temp.compare(Legend[i]);

			if (temp.meanErrorPerPixel() < bestvalue) {
				bestvalue = temp.meanErrorPerPixel();
				bestmatch = i;
			}
		}

		result = result + tiles[bestmatch];

		col++;
		if (col == h_tiles) {
			col = 0;
			row++;
			result = result + "#\n";
			if (row == v_tiles) 
				result = result + "#############\n";
			else 
				result = result + "#";
		}
	}

	return result;
}

string AllLevels("");

string parseAllLevels(Image* Legend) {
	string base("levels\\levels_");
	string ext(".png");

	for (int i = 0; i < 100; i++) {
		stringstream ss;
		int lnum = i + 1;
		if (i < 9)
			ss << base << "0" << lnum << ext;
		else
			ss << base << lnum << ext;
		string level = ss.str();

		AllLevels = AllLevels + parseLevel(level, Legend) + "\n";
	}

	// Bonus level 60
	stringstream ss;
	ss << base << "60-2" << ext;
	AllLevels = AllLevels + parseLevel(ss.str(), Legend);

	return AllLevels;
}

int main( int /*argc*/, char ** argv)
{

  // Initialize ImageMagick install location for Windows
  InitializeMagick(*argv);

  Image* legend = buildLegend();

  try {

    string srcdir("");
    if(getenv("SRCDIR") != 0)
      srcdir = getenv("SRCDIR");

    //
    // Options
    //

    //
    // Magick++ operations
    //

	//string s = parseLevel("levels_99.png", legend);
	string s = parseAllLevels(legend);

	cout << s;
	//cin.get();

	// test_code.txt


  }
  catch( exception &error_ )
	{
		cout << "Caught exception: " << error_.what() << endl;
		return 1;
	}

  return 0;
}