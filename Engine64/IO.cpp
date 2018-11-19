#include "IO.h"


using namespace std;

char * IO::loadTextFromFile(string filename) {
	fstream fs;
	int length;
	char *text;
	
	fs.open(filename, fstream::in);
	if (!fs) {
		cout << "Error reading " << filename << endl;
		return nullptr;
	}
	
	//Length of file
	fs.seekg(0, fs.end);
	length = fs.tellg();
	text = new char[length+1];
	
	//Read whole file
	fs.seekg(0, fs.beg);
	fs.read(text, length);
	fs.close();

	//Null terminator at end (This is why we do 'length+1' when calculating text array size.)
	text[length] = '\0';
	
	return text;
}


IO::IO()
{
}



IO::~IO()
{
}


void IO::readObj(std::string filepath, std::vector<Vector3>& pos, std::vector<int> &inds)
{
	
	string line, temp;
	ifstream file = ifstream(filepath);
	istringstream s;
	Vector3 tempvf;
	int tempi;

	if (!file) {
		cout << "Error reading file " << filepath << endl;
	}

	while (getline(file, line)) {
		
		temp = line.substr(0, 2);

		if (temp == "v ") {
			s = istringstream(line.substr(2));
			s >> tempvf[0] >> tempvf[1] >> tempvf[2];
			pos.push_back(tempvf);
		}
		if (temp == "f ") {
			s = istringstream(line.substr(2));
			for (int i = 0; i < 3; ++i) {
				s >> tempi;
				inds.push_back(tempi);
			}
		}
		else if (line[0] == '#') {/*do nothing*/}
		else {/*do nothing*/}

	}
	

}

void IO::readObj(std::string filepath, std::vector<Vector3>& pos, std::vector<Vector2>& tex, std::vector<Vector3>& norm, std::vector<Vector3i>& inds)
{

	string line, temp;
	ifstream file = ifstream(filepath);
	istringstream s;
	Vector3 tempvect;
	Vector2 temptex;
	Vector3 tempnorm;
	Vector3i tempinds;
	char slash;

	if (!file) {
		cout << "Error reading file " << filepath << endl;
	}

	while (getline(file, line)) {

		temp = line.substr(0, 2);

		if (temp == "v ") {
			s = istringstream(line.substr(2));
			s >> tempvect[0] >> tempvect[1] >> tempvect[2];
			pos.push_back(tempvect);
		}
		else if (temp == "vt") {
			s = istringstream(line.substr(2));
			s >> temptex[0] >> temptex[1];
			tex.push_back(temptex);
		}
		else if (temp == "vn") {
			s = istringstream(line.substr(2));
			s >> tempnorm[0] >> tempnorm[1] >> tempnorm[2];
			norm.push_back(tempnorm);
		}

		if (temp == "f ") {
			s = istringstream(line.substr(2));
			for (int i = 0; i < 3; ++i) {
				s >> tempinds[0] >> slash >> tempinds[1] >> slash >> tempinds[2];
				inds.push_back(tempinds);
			}
		}
		else if (line[0] == '#') {/*do nothing*/ }
		else {/*do nothing*/ }

	}
}
